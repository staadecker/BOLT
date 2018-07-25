import 'dart:async';

import 'package:flutter_blue/flutter_blue.dart';

class BluetoothManager {
  static const VERSION = 5;
  static const MAC_ADDRESS = "D4:36:39:BF:82:AB";
  static const SERVICE_UUID = "0000ffe0-0000-1000-8000-00805f9b34fb";
  static const CHARACTERISTIC_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb";

  BluetoothCharacteristic bluetoothCharacteristic;

  bool connecting = false;
  bool connected = false;

  void connect(void updateStatus(String state)) async {
    if (connecting || connected)
      return; //If already connecting/connected do nothing

    connecting = true;

    updateStatus("Version: $VERSION");

    await new Future.delayed(const Duration(seconds: 1));

    var flutterBlue = FlutterBlue.instance;

    var bluetoothState =
        await flutterBlue.state; //Get the current state of the bluetooth

    //If bluetooth adapter not on abort
    if (bluetoothState != BluetoothState.on) {
      switch (bluetoothState) {
        case BluetoothState.off:
          updateStatus("Turn on bluetooth");
          break;
        case BluetoothState.unavailable:
          updateStatus("Bluetooth LE is not available on this device");
          break;
        default:
          updateStatus("Bluetooth LE adapter is unkown");
      }

      connecting = false;
      return;
    }

    updateStatus("Scanning for board...");

    //Scan for the board
    //Catch for error board not found
    ScanResult scanResult = await flutterBlue
        .scan(timeout: Duration(seconds: 10))
        .firstWhere((result) => result.device.id.id == MAC_ADDRESS)
        .catchError((error) {
      updateStatus("Board not found.");
      connecting = false;
    }, test: (e) => e is StateError);

    if (!connecting) return;

    updateStatus("Board found. Connecting...");

    //Connect to the device
    //Abort if can't connect
    flutterBlue
        .connect(scanResult.device, timeout: Duration(seconds: 10))
        .listen((result) async {
      print("Result ${result.toString()}");
      if (result == BluetoothDeviceState.connected) {
        print("Is connected");
        print((await scanResult.device.state));

        if (!connecting) return;

        updateStatus("Connected. Discovering services...");
        print((await scanResult.device.state));

        bluetoothCharacteristic = (await scanResult.device.discoverServices())
            .singleWhere((service) => service.uuid == Guid(SERVICE_UUID))
            .characteristics
            .singleWhere((chara) => chara.uuid == Guid(CHARACTERISTIC_UUID));

        updateStatus("Done connecting.");
        connected = true;
      }
    });
  }
}
