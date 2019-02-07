import 'dart:async';

import 'package:bolt_flutter/bluetoothConnection.dart';
import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:flutter_blue/flutter_blue.dart';

enum ConnectorState { connecting, failed }

class ConnectorMessage {
  final String message;
  final ConnectorState connectorState;

  ConnectorMessage(this.message, this.connectorState);
}

class ConnectorDialog extends StatefulWidget {
  @override
  _ConnectorDialogState createState() => _ConnectorDialogState();
}

class _ConnectorDialogState extends State<ConnectorDialog> {
  BluetoothConnector _bluetoothConnector = BluetoothConnector();
  String _dialogMessage;
  bool _connectingFailed = false;

  @override
  void initState() {
    super.initState();

    //Listen for messages from the connector
    _bluetoothConnector.messageStream.listen((message) {
      if (message.connectorState == ConnectorState.connecting) {
        setState(() {
          _dialogMessage = message.message;
        });
      } else {
        setState(() {
          _dialogMessage = message.message;
          _connectingFailed = true;
        });
      }
    });

    _bluetoothConnector.getConnection().then((result) {
      if (result != null) Navigator.pop(context, result);
    });
  }

  @override
  void dispose() {
    _bluetoothConnector.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      title: Text("Connecting to board"),
      content: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.center,
        children: <Widget>[
          _connectingFailed
              ? null
              : Padding(
                  child: CircularProgressIndicator(),
                  padding: EdgeInsets.all(10.0),
                ),
          _dialogMessage == null ? null : Flexible(child: Text(_dialogMessage)),
        ].where((element) => element != null).toList(),
      ),
      actions: <Widget>[
        _connectingFailed
            ? FlatButton(
                onPressed: () {
                  setState(() {
                    _connectingFailed = false;
                    _dialogMessage = null;
                  });
                  _bluetoothConnector.getConnection();
                },
                child: Text("Try Again"),
              )
            : null,
        FlatButton(
          onPressed: () => Navigator.pop(context),
          child: Text("Cancel"),
        ),
      ],
    );
  }
}

class BluetoothConnector {
  static const _MAC_ADDRESS = "D4:36:39:BF:82:AB";
  static const _SERVICE_UUID = "0000ffe0-0000-1000-8000-00805f9b34fb";
  static const _CHARACTERISTIC_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb";

  final StreamController<ConnectorMessage> messageStreamController = StreamController();

  //Stream of messages giving updates of the events
  Stream<ConnectorMessage> get messageStream => messageStreamController.stream;

  Future<BluetoothConnection> getConnection() async {
    //GET BLUETOOTH SERVICE
    messageStreamController.add(ConnectorMessage("Checking for Bluetooth availability...", ConnectorState.connecting));
    FlutterBlue bluetoothService = await getService(messageStreamController);
    if (bluetoothService == null) return null;

    //GET CORRECT DEVICE
    messageStreamController.add(ConnectorMessage("Scanning for board...", ConnectorState.connecting));
    BluetoothDevice device = await findDevice(bluetoothService);
    if (device == null) {
      messageStreamController.add(ConnectorMessage("Board not found.", ConnectorState.failed));
      return null;
    }

    //GET CONNECTION TO DEVICE
    messageStreamController.add(ConnectorMessage("Board found. Connecting...", ConnectorState.connecting));
    StreamSubscription connection = await createConnection(bluetoothService, device);
    if (connection == null) {
      messageStreamController.add(ConnectorMessage("Could not connect to board.", ConnectorState.failed));
      return null;
    }

    //GET DEVICE CHARACTERISTIC
    messageStreamController.add(ConnectorMessage("Connected. Discovering characteristic...", ConnectorState.connecting));
    BluetoothCharacteristic characteristic = await findCharacteristic(device);
    if (characteristic == null){
      messageStreamController.add(ConnectorMessage("Could not find characteristic.", ConnectorState.failed));
      connection.cancel();
      return null;
    }


    return BluetoothConnection(device, connection, characteristic);
  }

  void dispose() {
    messageStreamController.close();
  }

  static Future<FlutterBlue> getService(StreamController<ConnectorMessage> messageStreamController) async {
    FlutterBlue service = FlutterBlue.instance;

    if (!await service.isAvailable){
      messageStreamController.add(ConnectorMessage("Bluetooth Low Energy is not supported on this device.", ConnectorState.failed));
      return null;
    }

    if (!await service.isOn){
      messageStreamController.add(ConnectorMessage("Please turn on Bluetooth.", ConnectorState.failed));
      return null;
    }

    return service;
  }

  static Future<BluetoothDevice> findDevice(
      FlutterBlue bluetoothService) async {
    ScanResult scanResult = await bluetoothService
        .scan(timeout: Duration(seconds: 10))
        .firstWhere((result) => result.device.id.id == _MAC_ADDRESS,
            orElse: () => null);

    return scanResult?.device;
  }

  static Future<StreamSubscription> createConnection(FlutterBlue bluetoothService, BluetoothDevice device) {
    final Completer<StreamSubscription> completer = new Completer();
    StreamSubscription<BluetoothDeviceState> connection;

    connection = bluetoothService
        .connect(device, timeout: Duration(seconds: 10))
        .listen((BluetoothDeviceState result) {
      if (result == BluetoothDeviceState.connected) {
        completer.complete(connection);
      }
    });

    connection.onDone(() => completer.complete(null));

    return completer.future;
  }

  static Future<BluetoothCharacteristic> findCharacteristic(BluetoothDevice device) async {
    BluetoothCharacteristic characteristic = (await device.discoverServices())
        .singleWhere((service) => service.uuid == Guid(_SERVICE_UUID),
            orElse: () => null)
        ?.characteristics
        ?.singleWhere((chara) => chara.uuid == Guid(_CHARACTERISTIC_UUID),
            orElse: () => null);

    if (characteristic != null) {
      //Setup on notify for change
      await device.setNotifyValue(characteristic, true);
    }

    return characteristic;
  }
}
