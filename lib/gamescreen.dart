import 'dart:async';

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
              return ConnectingDialog();
            });

    //Canceled connection go back to home screen
    if (bluetoothConnection == null) Navigator.pop(context);

    bluetoothTransmitter = BluetoothTransmitter(bluetoothConnection);
    buttonPressSubscription =
        bluetoothTransmitter.buttonPresses.listen(firstButtonPressed);
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
    if (stopwatch.elapsed > gameDuration) tickTimer.cancel();
    else {
      setState(() {
        time = (gameDuration - stopwatch.elapsed).inMilliseconds
            .toString();
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

class ConnectingDialog extends StatefulWidget {
  @override
  _ConnectingDialogState createState() => _ConnectingDialogState();
}

class _ConnectingDialogState extends State<ConnectingDialog> {
  BluetoothConnector _bluetoothConnector = BluetoothConnector();
  String _currentMessage = "";
  bool connectingFailed = false;

  _ConnectingDialogState();

  @override
  void initState() {
    super.initState();

    _bluetoothConnector.connect();

    _bluetoothConnector.statusStream.listen((ConnectorUpdate update) {
      switch (update.state) {
        case ConnectorStatus.connected:
          Navigator.pop(context, _bluetoothConnector.bluetoothConnection);
          break;
        case ConnectorStatus.connecting:
          setState(() {
            _currentMessage = update.message;
            connectingFailed = false;
          });
          break;
        case ConnectorStatus.failed:
          setState(() {
            _currentMessage = update.message;
            connectingFailed = true;
          });
      }
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
            connectingFailed
                ? null
                : Padding(
                    child: CircularProgressIndicator(),
                    padding: EdgeInsets.all(10.0),
                  ),
            Flexible(child: Text(_currentMessage))
          ].where((element) => element != null).toList(),
        ),
        actions: <Widget>[
          connectingFailed
              ? FlatButton(
                  onPressed: () => _bluetoothConnector.connect(),
                  child: Text("Try Again"),
                )
              : null,
          FlatButton(
              onPressed: () => Navigator.pop(context), child: Text("Cancel"))
        ]);
  }
}
