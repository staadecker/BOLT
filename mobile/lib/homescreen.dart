import 'package:flutter/material.dart';

import 'gamescreen.dart';

class BoltApp extends StatefulWidget {
  @override
  _BoltAppState createState() => _BoltAppState();
}

class _BoltAppState extends State<BoltApp> {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Bolt',
      theme: ThemeData(
          primarySwatch: Colors.red,
          primaryColor: Colors.red,
          accentColor: Colors.redAccent),
      home: Scaffold(
        appBar: AppBar(
          title: new Text("Bolt"),
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              RaisedButton(
                child: Text("Start game"),
                onPressed: () {
                  Navigator.push(context,
                      MaterialPageRoute(builder: (context) => GamePage()));
                },
              ),
            ],
          ),
        ),
      ),
    );
  }
}
