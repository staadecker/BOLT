import 'package:flutter/material.dart';

import 'bluetooth.dart';
import 'bluetoothConnector.dart';
import 'gamescreen.dart';

/// The home screen of the app.
///
/// A page with a "Play" button.
class HomePageApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        title: 'Bolt',
        theme: ThemeData(
          primarySwatch: Colors.red,
          primaryColor: Colors.red,
          accentColor: Colors.redAccent,
        ),
        home: Scaffold(
            appBar: AppBar(
              title: new Text("Bolt"),
            ),
            body: HomePageAppBody()));
  }
}

class HomePageAppBody extends StatefulWidget {
  @override
  _HomePageAppBodyState createState() => _HomePageAppBodyState();
}

class _HomePageAppBodyState extends State<HomePageAppBody> {
  BtTransmitter btTransmitter;

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          RaisedButton(
            child: Text("Play"),
            onPressed: onPlay,
          ),
        ],
      ),
    );
  }

  void onPlay() async {
    //Create dialog that will generate a [BluetoothTransmitter] Object.
    if (btTransmitter == null) {
      btTransmitter = await showDialog<BtTransmitter>(
        context: context,
        barrierDismissible: false,
        builder: (_) => BtConnectorDialog(),
      );
    }

    if (btTransmitter != null) {
      Navigator.push(
        context,
        MaterialPageRoute(builder: (_) => GamePage(btTransmitter)),
      );
    }
  }

  @override
  void dispose() {
    print("Closing bt transmitter:" + (btTransmitter == null).toString());
    btTransmitter?.close();

    super.dispose();
  }
}
