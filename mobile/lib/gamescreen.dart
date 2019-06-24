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
  StreamSubscription firstButtonPressSubscription;

  _StatefulGamePageState(this._btTransmitter);

  @override
  void initState() {
    super.initState();

    game = SimpleGame(_btTransmitter);

    WidgetsBinding.instance.addPostFrameCallback(
      (_) => firstButtonPressSubscription = _btTransmitter.buttonPresses.listen((_) => _startGame()),
    );
  }

  _startGame() async {
    firstButtonPressSubscription.cancel();

    Future<Duration> gameResult = game.start();

    setState(() {
      isGameRunning = true;
    });

    tickTimer = new Timer.periodic(Duration(milliseconds: 30), tick);

    Duration result = await gameResult;

    tickTimer.cancel();

    if (result != null) {
      await showDialog(
        context: context,
        barrierDismissible: false,
        builder: (BuildContext content) {
          return AlertDialog(
            title: Text("Result"),
            content: Text(
              "Your average reaction speed was " +
                  result.inMilliseconds.toString() +
                  "ms per button.",
            ),
            actions: <Widget>[
               FlatButton(
                child: Text("Ok"),
                onPressed: () {
                  Navigator.of(context).pop();
                },
              ),
            ],
          );
        },
      );
    }

    game = SimpleGame(_btTransmitter);
    firstButtonPressSubscription = _btTransmitter.buttonPresses.listen((_) => _startGame());

    setState(() {
      isGameRunning = false;
      time = "0.00";
    });
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
          !isGameRunning
              ? Text(
                  "Press any button to start!",
                  style: TextStyle(fontSize: 16.0),
                )
              : null
        ].where((element) => element != null).toList(),
      ),
    );
  }

  @override
  void dispose() {
    tickTimer?.cancel();
    game?.cancelGame();
    firstButtonPressSubscription?.cancel();

    super.dispose();
  }
}
