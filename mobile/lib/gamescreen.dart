import 'dart:async';

import 'package:bolt_flutter/bluetoothConnection.dart';
import 'package:bolt_flutter/bluetoothConnector.dart';
import 'package:flutter/material.dart';
import 'bluetooth.dart';

class GamePage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Game"),
      ),
      body: StatefulGamePage(),
    );
  }
}

class StatefulGamePage extends StatefulWidget {
  @override
  _StatefulGamePageState createState() => _StatefulGamePageState();
}

class _StatefulGamePageState extends State<StatefulGamePage> {
  BluetoothTransmitter bluetoothTransmitter;
  StreamSubscription buttonPressSubscription;
  String time = "30.00";
  Timer tickTimer;
  Stopwatch stopwatch = Stopwatch();
  final Duration gameDuration = Duration(seconds: 30);

  @override
  void initState() {
    super.initState();

    WidgetsBinding.instance
        .addPostFrameCallback((_) => _showConnectingDialog());
  }

  _showConnectingDialog() async {
    BluetoothConnection bluetoothConnection =
        await showDialog<BluetoothConnection>(
            context: context,
            barrierDismissible: false,
            builder: (BuildContext context) {
              return ConnectorDialog();
            });

    //Canceled trying to connect. Go back to home screen
    if (bluetoothConnection == null) Navigator.pop(context);
    else {
      bluetoothTransmitter = BluetoothTransmitter(bluetoothConnection);
      buttonPressSubscription =
          bluetoothTransmitter.buttonPresses.listen(firstButtonPressed);
    }
  }

  void firstButtonPressed(int buttonNumber) {
    buttonPressSubscription.cancel();
    stopwatch.start();
    tickTimer = new Timer.periodic(Duration(milliseconds: 30), tick);
  }

  @override
  void dispose() {
    bluetoothTransmitter?.disconnect();
    super.dispose();
  }

  void tick(Timer timer) {
    if (stopwatch.elapsed > gameDuration)
      tickTimer.cancel();
    else {
      setState(() {
        time = (gameDuration - stopwatch.elapsed).inMilliseconds.toString();
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          Text(time, style: TextStyle(fontSize: 96.0)),
          Text(
            "Press any button to start!",
            style: TextStyle(fontSize: 16.0),
          )
        ],
      ),
    );
  }
}
