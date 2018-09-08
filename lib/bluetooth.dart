import 'dart:async';

import 'package:flutter_blue/flutter_blue.dart';
import 'package:collection/collection.dart';

class BluetoothConnection {
  BluetoothCharacteristic bluetoothCharacteristic;
  StreamSubscription<BluetoothDeviceState> connection;
  BluetoothDevice device;

  void disconnect() {
    connection?.cancel();
  }
}

enum ConnectorStatus { connecting, failed, connected }

class ConnectorUpdate {
  final String message;
  final ConnectorStatus state;

  ConnectorUpdate(this.message, this.state);
}

class BluetoothConnector {
  static const _MAC_ADDRESS = "D4:36:39:BF:82:AB";
  static const _SERVICE_UUID = "0000ffe0-0000-1000-8000-00805f9b34fb";
  static const _CHARACTERISTIC_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb";

  BluetoothConnection bluetoothConnection = BluetoothConnection();

  final StreamController<ConnectorUpdate> _statusUpdatesController =
      StreamController();

  //Stream of events when connecting to board
  Stream<ConnectorUpdate> get statusStream => _statusUpdatesController.stream;

  void connect() async {
    FlutterBlue flutterBlue = FlutterBlue.instance;

    //Send message depending on adapter state
    //Only continue if adapter is on
    switch (await flutterBlue.state) {
      case BluetoothState.on:
        _statusUpdatesController.add(ConnectorUpdate(
            "Scanning for board...", ConnectorStatus.connecting));
        break;
      case BluetoothState.off:
        _statusUpdatesController
            .add(ConnectorUpdate("Turn on bluetooth", ConnectorStatus.failed));
        return;
      case BluetoothState.unavailable:
        _statusUpdatesController.add(ConnectorUpdate(
            "Bluetooth LE is not available on this device",
            ConnectorStatus.failed));
        return;
      default:
        _statusUpdatesController.add(ConnectorUpdate(
            "Bluetooth LE adapter is unkown", ConnectorStatus.failed));
    }

    //Scan for the board
    //Catch for error board not found
    bool noDeviceFound = false;

    bluetoothConnection.device = (await flutterBlue
            .scan(timeout: Duration(seconds: 10))
            .firstWhere((result) => result.device.id.id == _MAC_ADDRESS)
            .catchError((error) {
      _statusUpdatesController
          .add(ConnectorUpdate("Board not found.", ConnectorStatus.failed));
      noDeviceFound = true;
    }))
        .device;

    if (noDeviceFound) return;

    _statusUpdatesController.add(ConnectorUpdate(
        "Board found. Connecting...", ConnectorStatus.connecting));

    //Connect to the device
    //Should not close stream until we want to disconnect
    bluetoothConnection.connection = flutterBlue
        .connect(bluetoothConnection.device, timeout: Duration(seconds: 10))
        .listen((result) async {
      if (result == BluetoothDeviceState.connected) {
        _statusUpdatesController.add(ConnectorUpdate(
            "Connected. Discovering services...", ConnectorStatus.connecting));

        //When connected get characteristic
        bluetoothConnection.bluetoothCharacteristic = (await bluetoothConnection
                .device
                .discoverServices())
            .singleWhere((service) => service.uuid == Guid(_SERVICE_UUID))
            .characteristics
            .singleWhere((chara) => chara.uuid == Guid(_CHARACTERISTIC_UUID));

        //Setup on notify for change
        await bluetoothConnection.device
            .setNotifyValue(bluetoothConnection.bluetoothCharacteristic, true);

        _statusUpdatesController.add(
            ConnectorUpdate("Done connecting.", ConnectorStatus.connected));
      }
    });

    bluetoothConnection.connection.onDone(() {
      _statusUpdatesController
          .add(ConnectorUpdate("Could not connect", ConnectorStatus.failed));
    });
  }
}

class BluetoothTransmitter {
  BluetoothConnection _bluetoothConnection;

  BluetoothTransmitter(this._bluetoothConnection) {
    //Register device for connection changes
    _bluetoothConnection.device.onStateChanged().listen(deviceStateChanged);

    _bluetoothConnection.device
        .onValueChanged(_bluetoothConnection.bluetoothCharacteristic)
        .listen(valueChanged);
  }

  void deviceStateChanged(BluetoothDeviceState state) {
    print("State changed to: " + state.toString());
  }

  void valueChanged(List<int> values) {
    if (ListEquality().equals(values, _BluetoothMessage.acknowledge.value))
      print("Received acknowledge");
    else if (values[0] == _BluetoothMessage.startCode) {
      print("Received packet: " +
          String.fromCharCodes(values, 1, values.length - 1));
      writePacket(_BluetoothMessage.acknowledge);
    } else
      print("Value changed : " + values.toString());
  }

  void writePacket(_BluetoothMessage packet) async {
    print(packet.value);
    await _bluetoothConnection.device.writeCharacteristic(
        _bluetoothConnection.bluetoothCharacteristic, packet.value);
  }

  void disconnect() {
    writePacket(_BluetoothMessage.end);
    _bluetoothConnection.disconnect();
  }

  void doAction() {
    writePacket(_BluetoothMessage.begin);
    writePacket(_BluetoothMessage.turnLedOff(31));
  }
}

class _BluetoothMessage {
  final List<int> _value;

  List<int> get value => _value;

  const _BluetoothMessage(this._value);

  static const _BluetoothMessage acknowledge = _BluetoothMessage([6]);
  static const startCode = 2;
  static const endCode = 3;

  static final _BluetoothMessage begin = createPacket("B");
  static final _BluetoothMessage end = createPacket("E");
  static final _BluetoothMessage shiftOut = createPacket("S");

  static _BluetoothMessage createPacket(String packetContent) =>
      _BluetoothMessage([startCode] + packetContent.codeUnits + [endCode]);

  static _BluetoothMessage turnLedOn(int ledNumber) =>
      createPacket("O" + ledNumber.toString());

  static _BluetoothMessage turnLedOff(ledNumber) =>
      createPacket("I" + ledNumber.toString());
}
