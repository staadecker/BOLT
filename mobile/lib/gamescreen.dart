import 'dart:async';

import 'package:flutter/material.dart';

import 'bluetooth.dart';
import 'gameLogic.dart';

class GamePage extends StatelessWidget {
  final BtTransmitter _btTransmitter;

  GamePage(this._btTransmitter);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Game"),
      ),
      body: StatefulGamePage(_btTransmitter),
    );
  }
}

class StatefulGamePage extends StatefulWidget {
  final BtTransmitter _btTransmitter;

  StatefulGamePage(this._btTransmitter);

  @override
  _StatefulGamePageState createState() =>
      _StatefulGamePageState(_btTransmitter);
}

class _StatefulGamePageState extends State<StatefulGamePage> {
  final BtTransmitter _btTransmitter;

  String time = "0.00";
  Timer tickTimer;
  SimpleGame game;
  bool isGameRunning = false;

  _StatefulGamePageState(this._btTransmitter);

  @override
  void initState() {
    super.initState();

    game = SimpleGame(_btTransmitter);

    WidgetsBinding.instance.addPostFrameCallback((_) => _postBuildSetup());
  }

  _postBuildSetup() async {
    Future<double> gameResult = game.start();

    setState(() {
      isGameRunning = true;
    });

    tickTimer = new Timer.periodic(Duration(milliseconds: 30), tick);

    double result = await gameResult;

    tickTimer.cancel();

    print("Result is: " + result.toString());
  }

  void tick(Timer timer) {
    setState(() {
      time = game.stopwatch.elapsedMilliseconds.toString();
    });
  }

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          Text(time, style: TextStyle(fontSize: 96.0)),
          !isGameRunning ? Text(
            "Press any button to start!",
            style: TextStyle(fontSize: 16.0),
          ): null
        ].where((element) => element != null).toList(),
      ),
    );
  }
}
