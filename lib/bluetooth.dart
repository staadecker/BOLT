import 'dart:async';

import 'package:flutter_blue/flutter_blue.dart';
import 'package:collection/collection.dart';

class BluetoothManager {
  static const VERSION = 2;

  static const MAC_ADDRESS = "D4:36:39:BF:82:AB";
  static const SERVICE_UUID = "0000ffe0-0000-1000-8000-00805f9b34fb";
  static const CHARACTERISTIC_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb";

  BluetoothCharacteristic bluetoothCharacteristic;
  StreamSubscription<BluetoothDeviceState> connection;
  BluetoothDevice device;

  void connect(void updateStatus(String state)) async {
    //TODO Remove. Only used for debugging
    updateStatus("Version: $VERSION");
    await new Future.delayed(const Duration(seconds: 1));

    var flutterBlue = FlutterBlue.instance;

    //Send message depending on adapter state
    //Return if adapter not on
    switch (await flutterBlue.state) {
      case BluetoothState.on:
        updateStatus("Scanning for board...");
        break;
      case BluetoothState.off:
        updateStatus("Turn on bluetooth");
        return;
      case BluetoothState.unavailable:
        updateStatus("Bluetooth LE is not available on this device");
        return;
      default:
        updateStatus("Bluetooth LE adapter is unkown");
        return;
    }

    //Scan for the board
    //Catch for error board not found
    bool noDeviceFound = false;

    device = (await flutterBlue
            .scan(timeout: Duration(seconds: 10))
            .firstWhere((result) => result.device.id.id == MAC_ADDRESS)
            .catchError((error) {
      updateStatus("Board not found.");
      noDeviceFound = true;
    }, test: (e) => e is StateError))
        .device;

    if (noDeviceFound) return;

    device.onStateChanged().listen(deviceStateChanged);

    updateStatus("Board found. Connecting...");

    //Connect to the device
    connection = flutterBlue
        .connect(device, timeout: Duration(seconds: 10))
        .listen((result) async {
      if (result == BluetoothDeviceState.connected) {
        updateStatus("Connected. Discovering services...");

        //When connected get characteristic
        bluetoothCharacteristic = (await device.discoverServices())
            .singleWhere((service) => service.uuid == Guid(SERVICE_UUID))
            .characteristics
            .singleWhere((chara) => chara.uuid == Guid(CHARACTERISTIC_UUID));

        //Setup on notify for change
        await device.setNotifyValue(bluetoothCharacteristic, true);
        device.onValueChanged(bluetoothCharacteristic).listen(valueChanged);

        updateStatus("Done connecting.");
      }
    });
  }

  void deviceStateChanged(BluetoothDeviceState state){
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
    await device?.writeCharacteristic(bluetoothCharacteristic, packet.value);
  }

  void disconnect() {
    writePacket(_BluetoothMessage.end);
    connection?.cancel();
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
