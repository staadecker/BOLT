import 'dart:async';

import 'package:flutter_blue/flutter_blue.dart';

class BluetoothConnection {
  final BluetoothDevice device;
  final StreamSubscription<BluetoothDeviceState> connection;
  final BluetoothCharacteristic btCharacteristic;

  BluetoothConnection(this.device, this.connection, this.btCharacteristic);

  void close() {
    connection.cancel();
  }
}
