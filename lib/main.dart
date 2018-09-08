import 'package:flutter/material.dart';

import 'game.dart';

void main() => runApp(new MyApp());

const String BUILD_NUMBER = "1";

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
            child: Text("Connect"),
            onPressed: () {
              Navigator.push(
                  context, MaterialPageRoute(builder: (context) => GamePage()));
            },
          ),
          Text("Build: $BUILD_NUMBER")
        ],
      ),
    );
  }
}
