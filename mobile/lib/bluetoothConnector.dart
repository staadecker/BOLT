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

  CancelableOperation<dynamic> currentOperation;

  /// Connects to the device and returns a [BluetoothConnection] or null if connecting failed
  CancelableOperation<BtTransmitter> connect() {
    CancelableCompleter<BtTransmitter> mainOperation =
        CancelableCompleter(onCancel: () => currentOperation.cancel());

    _runOperations().then((result) => mainOperation.complete(result));

    return mainOperation.operation;
  }

  Future<BtTransmitter> _runOperations() async {
    //GET BLUETOOTH SERVICE
    currentOperation = CancelableOperation.fromFuture(_getService());
    FlutterBlue btService = await currentOperation.value;
    if (btService == null) return null;

    //GET CORRECT DEVICE
    currentOperation =
        CancelableOperation.fromFuture(_findAndConnectDevice(btService));
    BluetoothDevice device = await currentOperation.value;
    if (device == null) return null;

    //GET DEVICE CHARACTERISTIC
    currentOperation =
        CancelableOperation.fromFuture(_findCharacteristic(device));
    BluetoothCharacteristic characteristic = await currentOperation.value;
    if (characteristic == null) return null;

    //WAIT FOR BOARD TO GO ONLINE
    BtTransmitter btTransmitter = BtTransmitter(device, characteristic);

    currentOperation =
        CancelableOperation.fromFuture(_waitForBoardToGoOnline(btTransmitter));
    bool success = await currentOperation.value;
    if (success)
      return btTransmitter;
    else
      return null;
  }

  void cancel() {
    currentOperation.cancel();
  }

  /// Helper method to post to the updateStream
  void _postUpdate(String message) {
    _updatesStreamController.add(message);
  }

  /// Returns the [FlutterBlue] service if available. The [FlutterBlue] service is used to access bluetooth.
  Future<FlutterBlue> _getService() async {
    _postUpdate("Checking for Bluetooth availability...");

    FlutterBlue service = FlutterBlue.instance;

    if (!await service.isAvailable) {
      _postUpdate("Bluetooth Low Energy is not supported on this device.");
      return null;
    } else if (!await service.isOn) {
      _postUpdate("Please turn on Bluetooth.");
      return null;
    }

    return service;
  }

  /// Returns the device matching the [_TARGET_DEVICE_ID] if it is found.
  Future<BluetoothDevice> _findAndConnectDevice(FlutterBlue btService) async {
    _postUpdate("Scanning for board...");
    List<BluetoothDevice> connectedDevices = await btService.connectedDevices;

    for (BluetoothDevice device in connectedDevices) {
      if (device.id == _TARGET_DEVICE_ID) {
        return device;
      }
    }

    BluetoothDevice device =
        (await btService.scan(timeout: Duration(seconds: 10)).firstWhere(
      (result) => result.device.id == _TARGET_DEVICE_ID,
      orElse: () {
        _postUpdate("Board not found.");
        return null;
      },
    ))
            ?.device;

    await btService.stopScan();

    if (device == null) return null;

    _postUpdate("Connecting to board...");
    await device.connect(timeout: Duration(seconds: 10));
    return device;
  }

  /// Finds the correct characteristic of the [device].
  Future<BluetoothCharacteristic> _findCharacteristic(
      BluetoothDevice device) async {
    _postUpdate("Discovering characteristics...");
    BluetoothCharacteristic characteristic = (await device.discoverServices())
        .singleWhere((service) => service.uuid.toString() == _SERVICE_UUID,
            orElse: () => null)
        ?.characteristics
        ?.singleWhere((chara) => chara.uuid.toString() == _CHARACTERISTIC_UUID,
            orElse: () => null);

    if (characteristic == null) {
      _postUpdate("Could not find characteristic.");
      return null;
    }

    //Setup on notify for change
    await characteristic.setNotifyValue(true);

    return characteristic;
  }

  /// Sends begin packet to board and waits for acknowledge.
  /// If acknowledge received returns true
  Future<bool> _waitForBoardToGoOnline(BtTransmitter btTransmitter) async {
    _postUpdate("Wainting for board to respond...");

    btTransmitter.writePacket(BtMessage.begin);

    return btTransmitter.acknowledgeStream
        .map((element) => true)
        .timeout(Duration(seconds: 8), onTimeout: (sink) {
      _postUpdate("Board did not respond.");
      sink.add(false);
    }).first;
  }
}
