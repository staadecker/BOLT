import 'package:flutter/material.dart';

import 'gamescreen.dart';

class BoltApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Bolt',
      theme: ThemeData(
          primarySwatch: Colors.red,
          primaryColor: Colors.red,
          accentColor: Colors.redAccent
      ),
      home: Scaffold(
          appBar: AppBar(
            title: new Text("Bolt"),
          ),
          body: BoltStatefulApp()),
    );
  }
}

class BoltStatefulApp extends StatefulWidget {
  @override
  _BoltStatefulApp createState() => _BoltStatefulApp();
}

class _BoltStatefulApp extends State<BoltStatefulApp> {
  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          RaisedButton(
            child: Text("Play"),
            onPressed: () {
              Navigator.push(context, MaterialPageRoute(builder: (_) => GamePage()));
            },
          ),
        ],
      ),
    );
  }
}
