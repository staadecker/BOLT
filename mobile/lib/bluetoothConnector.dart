import 'dart:async';

import 'package:async/async.dart';
import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:flutter_blue/flutter_blue.dart';

import 'bluetooth.dart';

/// A simple dialog that when created will connect to the device and display the progress in the connection
/// Returns a [BluetoothTransmitter] or null if the user canceled the action
class BtConnectorDialog extends StatefulWidget {
  @override
  _BtConnectorDialogState createState() => _BtConnectorDialogState();
}

class _BtConnectorDialogState extends State<BtConnectorDialog> {
  /// The backend connector
  final BtConnector _btConnector = BtConnector();

  /// The message to display if there is one
  String _latestMessage;

  /// Indicates whether the bluetoothConnector failed to connect
  bool _hasConnectorFailed = false;

  StreamSubscription updatesSubscription;

  CancelableOperation<BtTransmitter> connectionOperation;

  @override
  void initState() {
    super.initState();

    updatesSubscription = _btConnector.updates.listen(_onUpdateReceived);
    _startConnecting();
  }

  void _startConnecting() {
    connectionOperation = _btConnector.connect();
    connectionOperation.then(_onConnectionResult);
  }

  void _onUpdateReceived(String message) {
    //Don't need to check for mounted since subscription is cancelled onDispose
    setState(() {
      _latestMessage = message;
    });
  }

  /// Called when bluetooth connector is done connecting
  ///
  /// Will be null if connector failed connecting.
  /// If connection not null, dialog is done so pop and return the connection (as an argument)
  BtTransmitter _onConnectionResult(BtTransmitter connection) {
    if (connection == null)
      setState(() {
        _hasConnectorFailed = true;
      });
    else
      Navigator.pop(context, connection);

    return null;
  }

  //Button only available when _hasConnectorFailed is true
  //_hasConnectorFailed is true only after operation completes
  void _onTryAgainPressed() {
    setState(() {
      _hasConnectorFailed = false;
      _latestMessage = null;
    });

    _startConnecting();
  }

  void _onCancelPressed() {
    connectionOperation.cancel();
    Navigator.pop(context, null);
  }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      title: Text(
          _hasConnectorFailed ? "Failed to connect" : "Connecting to board..."),
      content: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        children: _buildDialogContent(),
      ),
      actions: _buildActionButtons(),
    );
  }

  List<Widget> _buildActionButtons() {
    return <Widget>[
      _hasConnectorFailed
          ? FlatButton(onPressed: _onTryAgainPressed, child: Text("Try Again"))
          : null,
      FlatButton(onPressed: _onCancelPressed, child: Text("Cancel")),
    ].where((element) => element != null).toList();
  }

  List<Widget> _buildDialogContent() {
    return <Widget>[
      !_hasConnectorFailed
          ? Padding(
              child: CircularProgressIndicator(),
              padding: EdgeInsets.all(10.0),
            )
          : null,
      _latestMessage != null ? Flexible(child: Text(_latestMessage)) : null,
    ].where((element) => element != null).toList();
  }

  //Called after Navigator.pop()
  //Navigator.pop() called after result or cancel
  @override
  void dispose() {
    updatesSubscription.cancel();

    super.dispose();
  }
}

/// Class that is a service that creates a connection to the device.
///
/// Fully backend no UI.
/// The service posts update message to [updates] to notify listeners of progress.
class BtConnector {
  // TODO : Do not hard code mac address
  static const _TARGET_DEVICE_ID = DeviceIdentifier("D4:36:39:BF:82:AB");
  static const _SERVICE_UUID = "0000ffe0-0000-1000-8000-00805f9b34fb";
  static const _CHARACTERISTIC_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb";

  final StreamController<String> _updatesStreamController = StreamController();

  /// Stream of messages giving updates of the connection process
  Stream<String> get updates => _updatesStreamController.stream;

  CancelableOperation currentOperation;
  CancelableCompleter<BtTransmitter> mainOperation;

  /// Connects to the device and returns a [BluetoothConnection] or null if connecting failed
  CancelableOperation<BtTransmitter> connect() {
    mainOperation =
        CancelableCompleter(onCancel: () => currentOperation.cancel());

    _checkBtAvailability(FlutterBlue.instance); // Start the chain of operations

    return mainOperation.operation;
  }

  void _checkBtAvailability(FlutterBlue btService) => _createOperation(
        btService.isAvailable,
        onComplete: (isAvailable) {
          if (isAvailable)
            _checkBtIsOn(btService);
          else {
            _postUpdate(
                "Bluetooth Low Energy is not supported on this device.");
            mainOperation.complete(null);
          }
        },
        message: "Checking for Bluetooth availability...",
      );

  void _checkBtIsOn(FlutterBlue btService) => _createOperation(
        btService.isOn,
        message: "Checking if Bluetooth is on...",
        onComplete: (isOn) {
          if (isOn)
            _checkAlreadyConnectedDevices(btService);
          else {
            _postUpdate("Please turn on Bluetooth.");
            mainOperation.complete(null);
          }
        },
      );

  void _checkAlreadyConnectedDevices(FlutterBlue btService) => _createOperation(
        btService.connectedDevices,
        message: "Checking connected device...",
        onComplete: (connectedDevices) {
          for (BluetoothDevice device in connectedDevices) {
            if (device.id == _TARGET_DEVICE_ID) {
              _getDeviceCharacteristic(btService, device);
              return;
            }
          }
          _scanForDevice(btService);
        },
      );

  void _scanForDevice(FlutterBlue btService) => _createOperation(
        btService.scan(timeout: Duration(seconds: 10)).firstWhere(
            (result) => result.device.id == _TARGET_DEVICE_ID,
            orElse: () => null),
        message: "Scanning for board...",
        onComplete: (result) {
          if (result == null) {
            _postUpdate("Board not found.");
            mainOperation.complete(null);
          } else {
            btService.stopScan();
            _connectToBoard(btService, result.device);
          }
        },
        onCancel: () => btService.stopScan(),
      );

  void _connectToBoard(FlutterBlue btService, BluetoothDevice device) =>
      _createOperation(
        device.connect(timeout: Duration(seconds: 10)),
        message: "Connecting to board...",
        onComplete: (_) => _getDeviceCharacteristic(btService, device),
      );

  void _getDeviceCharacteristic(
          FlutterBlue btService, BluetoothDevice device) =>
      _createOperation(
        device.discoverServices(),
        message: "Discovering characteristics...",
        onComplete: (result) {
          BluetoothCharacteristic characteristic = result
              .singleWhere(
                  (service) => service.uuid.toString() == _SERVICE_UUID,
                  orElse: () => null)
              ?.characteristics
              ?.singleWhere(
                  (chara) => chara.uuid.toString() == _CHARACTERISTIC_UUID,
                  orElse: () => null);

          if (characteristic == null) {
            _postUpdate("Could not find characteristic.");
            mainOperation.complete(null);
          } else {
            _setOnNotify(btService, device, characteristic);
          }
        },
      );

  void _setOnNotify(FlutterBlue btService, BluetoothDevice device,
          BluetoothCharacteristic characteristic) =>
      _createOperation(
        characteristic.setNotifyValue(true),
        message: "Registering for notification",
        onComplete: (_) =>
            _waitForBoardToGoOnline(btService, device, characteristic),
      );

  void _waitForBoardToGoOnline(FlutterBlue btService, BluetoothDevice device,
      BluetoothCharacteristic characteristic) {
    BtTransmitter btTransmitter = BtTransmitter(device, characteristic);

    _createOperation(
      btTransmitter.acknowledgeStream
          .map((element) => true)
          .timeout(Duration(seconds: 8), onTimeout: (sink) {
        _postUpdate("Board did not respond.");
        sink.add(false);
      }).first,
      message: "Waiting for board to respond...",
      onComplete: (receivedAcknowledge) {
        if (receivedAcknowledge) {
          mainOperation.complete(btTransmitter);
        } else {
          mainOperation.complete(null);
        }
      },
    );

    btTransmitter.writePacket(BtMessage.begin);
  }

  void _postUpdate(String message) => _updatesStreamController.add(message);

  void _createOperation<T, R>(Future<T> operation,
      {FutureOr<R> Function(T) onComplete, FutureOr<R> Function() onCancel, String message}) {
    currentOperation = CancelableOperation.fromFuture(operation);
    currentOperation.then(onComplete, onCancel: onCancel);
    _postUpdate(message);
  }
}