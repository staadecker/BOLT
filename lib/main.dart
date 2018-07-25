import 'package:flutter/material.dart';
import 'bluetooth.dart';

void main() => runApp(new MyApp());

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: Scaffold(
          appBar: AppBar(
            title: new Text("Bolt"),
          ),
          body: StatefulApp()),
    );
  }
}

class StatefulApp extends StatefulWidget {
  @override
  _ConnectionStateState createState() => _ConnectionStateState();
}

class _ConnectionStateState extends State<StatefulApp> {
  String state = "";

  BluetoothManager bluetoothManager = BluetoothManager();

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          Padding(
              padding: EdgeInsets.all(10.0),
              child: RaisedButton(
                child: Text("Connect"),
                onPressed: () {
                  bluetoothManager.connect(_onStateUpdate);
                },
              )),
          Text(
            state,
            textAlign: TextAlign.center,
          )
        ],
      ),
    );
  }

  void _onStateUpdate(String message) {
    setState(() {
      state = message;
    });
  }
}
