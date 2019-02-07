import 'dart:async';

import 'package:bolt_flutter/bluetoothConnection.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'package:collection/collection.dart';

class BluetoothTransmitter {
  BluetoothConnection _bluetoothConnection;

  final StreamController<int> _buttonPressStreamController = new StreamController();

  Stream<int> get buttonPresses => _buttonPressStreamController.stream;

  BluetoothTransmitter(this._bluetoothConnection) {
    //Register device for connection changes
    _bluetoothConnection.device.onStateChanged().listen(deviceStateChanged);
    _bluetoothConnection.device
        .onValueChanged(_bluetoothConnection.btCharacteristic)
        .listen(valueChanged);
  }

  void deviceStateChanged(BluetoothDeviceState state) {
    print("State changed to: " + state.toString());
  }

  void valueChanged(List<int> values) {
    if (ListEquality().equals(values, _BluetoothMessage.acknowledge.value))
      print("Received acknowledge");
    else if (values[0] == _BluetoothMessage.startCode &&
        values[values.length - 1] == _BluetoothMessage.endCode) {
      switch (values[1]) {
        case 80:
          int buttonNumber = int.parse(String.fromCharCodes(values, 2, 3));
          print("Received button pressed : $buttonNumber");
          _buttonPressStreamController.add(buttonNumber);
          break;
        default:
          print("Unknown packet content received");
      }
      writePacket(_BluetoothMessage.acknowledge);
    } else
      print("Value changed : " + values.toString());
  }

  void writePacket(_BluetoothMessage packet) async {
    print("Sending content: $packet");
    await _bluetoothConnection.device.writeCharacteristic(
        _bluetoothConnection.btCharacteristic, packet.value);
  }

  void disconnect() {
    writePacket(_BluetoothMessage.end);
    _bluetoothConnection.close();
    _buttonPressStreamController.close();
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

  @override
  String toString() {
    return _value.toString();
  }
}
