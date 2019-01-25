import 'package:flutter/material.dart';

import 'gamescreen.dart';

void main() => runApp(new MyApp());

const String BUILD_NUMBER = "1";

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
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
          body: StatefulApp()),
    );
  }
}

class StatefulApp extends StatefulWidget {
  @override
  _StatefulAppState createState() => _StatefulAppState();
}

class _StatefulAppState extends State<StatefulApp> {
  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          RaisedButton(
            child: Text("Play"),
            onPressed: () {
              Navigator.push(
                  context, MaterialPageRoute(builder: (context) => GamePage()));
            },
          ),
          Text("Build number: $BUILD_NUMBER")
        ],
      ),
    );
  }
}
