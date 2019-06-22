import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:flutter_blue/flutter_blue.dart';

import 'bluetooth.dart';

/// A simple dialog that when created will connect to the device and display the progress in the connection
/// Returns a [BluetoothConnection] or null if the user canceled the action
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

  @override
  void initState() {
    super.initState();

    // Listen for updates from the connector
    _btConnector.updates.listen((message) {
      setState(() {
        _latestMessage = message;
      });
    });

    // Start connecting to device and pass result to _onConnectionReceived
    _btConnector.connect().then(_onConnectionReceived);
  }

  @override
  void dispose() {
    _btConnector.dispose();
    super.dispose();
  }

  /// Called when bluetooth connector is done connecting
  ///
  /// Will be null if connector failed connecting.
  /// If connection not null, dialog is done so pop and return the connection (as an argument)
  void _onConnectionReceived(BtTransmitter connection) {
    if (connection == null) {
      setState(() {
        _hasConnectorFailed = true;
      });
    } else
      Navigator.pop(context, connection);
  }

  void _onTryAgainPressed() {
    setState(() {
      _hasConnectorFailed = false;
      _latestMessage = null;
    });
    _btConnector.connect().then(_onConnectionReceived);
  }

  void _onCancelPressed() {
    Navigator.pop(context, null);
  }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      title: Text(
          _hasConnectorFailed ? "Failed to connect" : "Connecting to board..."),
      content: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          !_hasConnectorFailed ? _buildCircularProgressIndicator() : null,
          _latestMessage != null ? _buildMessageWidget() : null,
        ].where(_isElementNotNull).toList(),
      ),
      actions: <Widget>[
        _hasConnectorFailed ? _buildTryAgainButton() : null,
        FlatButton(
          onPressed: _onCancelPressed,
          child: Text("Cancel"),
        ),
      ],
    );
  }

  Widget _buildMessageWidget() {
    return Flexible(child: Text(_latestMessage));
  }

  FlatButton _buildTryAgainButton() {
    return FlatButton(
      onPressed: _onTryAgainPressed,
      child: Text("Try Again"),
    );
  }

  static Widget _buildCircularProgressIndicator() {
    return Padding(
      child: CircularProgressIndicator(),
      padding: EdgeInsets.all(10.0),
    );
  }

  static bool _isElementNotNull(element) => element != null;
}

/// Class that is a service that creates a connection to the device.
///
/// Fully backend no UI.
/// The service posts update message to [updates] to notify listeners of progress.
class BtConnector {
  // TODO : Do not hard code mac address
  static const _MAC_ADDRESS = "D4:36:39:BF:82:AB";
  static const _SERVICE_UUID = "0000ffe0-0000-1000-8000-00805f9b34fb";
  static const _CHARACTERISTIC_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb";

  final StreamController<String> _updatesStreamController = StreamController();

  /// Stream of messages giving updates of the connection process
  Stream<String> get updates => _updatesStreamController.stream;

  /// Connects to the device and returns a [BluetoothConnection] or null if connecting failed
  Future<BtTransmitter> connect() async {
    //GET BLUETOOTH SERVICE
    _postUpdate("Checking for Bluetooth availability...");
    FlutterBlue btService = await _getService();
    if (btService == null)
      return null; //_getService will call _postUpdate with failure message

    //GET CORRECT DEVICE
    _postUpdate("Scanning for board...");
    BluetoothDevice device = await _findDevice(btService);
    if (device == null) {
      _postUpdate("Board not found.");
      return null;
    }

    //GET CONNECTION TO DEVICE
    _postUpdate("Board found. Connecting...");
    StreamSubscription connection = await _createConnection(btService, device);
    if (connection == null) {
      _postUpdate("Could not connect to board.");
      return null;
    }

    //GET DEVICE CHARACTERISTIC
    _postUpdate("Connected. Discovering characteristic...");
    BluetoothCharacteristic characteristic = await _findCharacteristic(device);
    if (characteristic == null) {
      _postUpdate("Could not find characteristic.");
      connection.cancel();
      return null;
    }

    //WAIT FOR BOARD TO GO ONLINE
    BtTransmitter btTransmitter =
        BtTransmitter(device, connection, characteristic);

    _postUpdate("Wainting for board to respond...");
    bool success = await _waitForBoardToGoOnline(btTransmitter);
    if (!success) {
      _postUpdate("Board did not respond.");
      btTransmitter.close();
      return null;
    }

    return btTransmitter;
  }

  /// Should be called before disposing the object to close the streams.
  void dispose() {
    _updatesStreamController.close();
  }

  /// Helper method to post to the updateStream
  void _postUpdate(String message) {
    if (!_updatesStreamController.isClosed) {
      _updatesStreamController.add(message);
    }
  }

  /// Returns the [FlutterBlue] service if available. The [FlutterBlue] service is used to access bluetooth.
  Future<FlutterBlue> _getService() async {
    FlutterBlue service = FlutterBlue.instance;

    if (!await service.isAvailable) {
      _postUpdate("Bluetooth Low Energy is not supported on this device.");
      return null;
    }

    if (!await service.isOn) {
      _postUpdate("Please turn on Bluetooth.");
      return null;
    }

    return service;
  }

  /// Returns the device matching the [_MAC_ADDRESS] if it is found.
  static Future<BluetoothDevice> _findDevice(FlutterBlue btService) async {
    ScanResult scanResult = await btService
        .scan(timeout: Duration(seconds: 10))
        .firstWhere((result) => result.device.id.id == _MAC_ADDRESS,
            orElse: () => null);

    return scanResult?.device;
  }

  /// Connects to [device] and returns the connection.
  static Future<StreamSubscription> _createConnection(
      FlutterBlue btService, BluetoothDevice device) {
    final Completer<StreamSubscription> completer = new Completer();
    StreamSubscription<BluetoothDeviceState> connection;

    connection = btService
        .connect(device, timeout: Duration(seconds: 10))
        .listen((BluetoothDeviceState result) {
      print("Bluetooth device state changed to:" + result.toString());
      if (result == BluetoothDeviceState.connected) {
        completer.complete(connection);
      }
    });

    connection.onDone(() {
      if (!completer.isCompleted) completer.complete(null);
    });

    return completer.future;
  }

  /// Finds the correct characteristic of the [device].
  static Future<BluetoothCharacteristic> _findCharacteristic(
      BluetoothDevice device) async {
    BluetoothCharacteristic characteristic = (await device.discoverServices())
        .singleWhere((service) => service.uuid.toString() == _SERVICE_UUID,
            orElse: () => null)
        ?.characteristics
        ?.singleWhere((chara) => chara.uuid.toString() == _CHARACTERISTIC_UUID,
            orElse: () => null);

    if (characteristic != null) {
      //Setup on notify for change
      await device.setNotifyValue(characteristic, true);
    }

    return characteristic;
  }

  /// Sends begin packet to board and waits for acknowledge.
  /// If acknowledge received returns true
  static Future<bool> _waitForBoardToGoOnline(BtTransmitter btTransmitter) {
    Completer<bool> completer = new Completer();

    StreamSubscription firstAcknowledge;

    firstAcknowledge = btTransmitter.acknowledgeStream.listen((_) {
      firstAcknowledge.cancel();
      completer.complete(true);
    });

    btTransmitter.writePacket(BtMessage.begin);

    new Timer(Duration(seconds: 8), () {
      if (!completer.isCompleted) {
        completer.complete(false);
        firstAcknowledge.cancel();
      }
    });

    return completer.future;
  }
}
