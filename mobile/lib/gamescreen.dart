import 'dart:async';

import 'package:flutter/material.dart';
import 'package:intl/intl.dart';

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
  static final NumberFormat formatter = NumberFormat("##0.00");

  final BtTransmitter _btTransmitter;

  String time = _formatTime(Duration.zero);
  Timer tickTimer;
  SimpleGame game;
  bool isGameRunning = false;

  _StatefulGamePageState(this._btTransmitter);

  @override
  void initState() {
    super.initState();

    game = SimpleGame(_btTransmitter);

    WidgetsBinding.instance.addPostFrameCallback(
      (_) => _btTransmitter.buttonPresses.first.then((_) => _startGame()),
    );
  }

  void tick(Timer timer) {
    setState(() {
      time = _formatTime(game.stopwatch.elapsed);
    });
  }

  _startGame() async {
    if (mounted) {
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
          builder: (_) => ResultDialog(result),
        );
      }

      game = SimpleGame(_btTransmitter);

      if (mounted) {
        _btTransmitter.buttonPresses.first.then((_) => _startGame());

        setState(() {
          isGameRunning = false;
          time = "0.00";
        });
      }
    }
  }

  static String _formatTime(Duration elapsedTime) {
    return formatter.format((elapsedTime.inMilliseconds) / 1000);
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

    super.dispose();
  }
}

class ResultDialog extends StatelessWidget {
  final Duration result;

  ResultDialog(this.result);

  @override
  Widget build(BuildContext context) {
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
  }
}
