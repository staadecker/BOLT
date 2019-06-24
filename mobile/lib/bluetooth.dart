import 'dart:async';

import 'package:collection/collection.dart';
import 'package:flutter_blue/flutter_blue.dart';

class BtTransmitter {
  final BluetoothDevice _device;
  final BluetoothCharacteristic _btCharacteristic;

  final StreamController<int> _buttonPressStreamController =
      new StreamController.broadcast();

  Stream<int> get buttonPresses => _buttonPressStreamController.stream;

  final StreamController<Null> _acknowledgeStreamController =
      new StreamController.broadcast();

  Stream<Null> get acknowledgeStream => _acknowledgeStreamController.stream;

  BtTransmitter(this._device, this._btCharacteristic) {
    //Register device for connection changes
    _device.state.listen(_deviceStateChanged);
    _btCharacteristic.value.listen(_valueChanged);
  }

  void _deviceStateChanged(BluetoothDeviceState state) {
    print("State changed to: " + state.toString());
  }

  void _valueChanged(List<int> values) {
    if (values.length == 0) return;
    if (ListEquality().equals(values, BtMessage.acknowledge.value))
      _acknowledgeStreamController.add(null);
    else if (values[0] == BtMessage.startCode &&
        values[values.length - 1] == BtMessage.endCode) {
      switch (values[1]) {
        case 80:
          int buttonNumber = int.parse(String.fromCharCodes(values, 2, 4));
          print("Received button pressed : $buttonNumber");
          _buttonPressStreamController.add(buttonNumber);
          break;
        default:
          print("Unknown packet content received : " + values.toString());
      }
      writePacket(BtMessage.acknowledge);
    } else
      print("Value changed : " + values.toString());
  }

  Future<void> writePacket(BtMessage message) async {
    print("Sending content: $message");
    return await _btCharacteristic.write(message.value);
  }

  void close() {
    _device.disconnect();
    writePacket(BtMessage.end);
    _buttonPressStreamController.close();
    _acknowledgeStreamController.close();
  }
}

class BtMessage {
  final List<int> _value;

  List<int> get value => _value;

  const BtMessage(this._value);

  static const BtMessage acknowledge = BtMessage([6]);
  static const startCode = 0x01;
  static const endCode = 0x04;

  static final BtMessage begin = _createPacket("B");
  static final BtMessage end = _createPacket("E");
  static final BtMessage shiftOut = _createPacket("S");

  static BtMessage _createPacket(String packetContent) =>
      BtMessage([startCode] + packetContent.codeUnits + [endCode]);

  static BtMessage turnLedOn(int ledNumber) =>
      _createPacket("O" + ledNumber.toString().padLeft(2, "0"));

  static BtMessage turnLedOff(ledNumber) =>
      _createPacket("I" + ledNumber.toString().padLeft(2, "0"));

  static BtMessage mergePackets(List<BtMessage> packets) =>
      BtMessage(packets.expand((x) => x.value).toList());

  @override
  String toString() {
    return _value.toString();
  }
}
