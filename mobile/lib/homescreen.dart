import 'package:flutter/material.dart';

import 'gamescreen.dart';

/// The home screen of the app.
///
/// A page with a "Play" button.
class BoltApp extends StatelessWidget {
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
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              RaisedButton(
                child: Text("Play"),
                onPressed: () {
                  Navigator.push(
                      context, MaterialPageRoute(builder: (_) => GamePage()));
                },
              ),
            ],
          ),
        ),
      ),
    );
  }
}
