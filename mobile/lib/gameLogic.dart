import 'dart:async';
import 'dart:math';

import 'bluetooth.dart';

const int _NUMBER_OF_LEDS = 64;

class SimpleGame {
  static const Duration GAME_DURATION = Duration(seconds: 30);

  final BtTransmitter _btTransmitter;
  final Stopwatch stopwatch = Stopwatch();
  final Completer<double> _gameComplete = new Completer();
  int _buttonsPressed = 0;
  int ledOn;
  StreamSubscription buttonPressSubscription;
  Random random = Random();

  SimpleGame(this._btTransmitter);

  Future<double> start() async {
    ledOn = random.nextInt(_NUMBER_OF_LEDS);

    stopwatch.start();
    _btTransmitter.writePacket(BtMessage.mergePackets(
        [BtMessage.turnLedOn(ledOn), BtMessage.shiftOut]));

    buttonPressSubscription =
        _btTransmitter.buttonPresses.listen(_onButtonPress);

    return _gameComplete.future;
  }

  void _onButtonPress(int buttonNumber) {
    if (buttonNumber == ledOn) {
      _buttonsPressed++;

      BtMessage turnOffMessage = BtMessage.turnLedOff(ledOn);

      if (stopwatch.elapsed > GAME_DURATION) {
        _btTransmitter.writePacket(turnOffMessage);
        _endGame();
      } else {
        ledOn = random.nextInt(_NUMBER_OF_LEDS);

        _btTransmitter.writePacket(BtMessage.mergePackets(
            [turnOffMessage, BtMessage.turnLedOn(ledOn), BtMessage.shiftOut]));
      }
    }

    if (stopwatch.elapsed > GAME_DURATION) {
      _endGame();
    }
  }

  void _endGame() {
    stopwatch.stop();

    if (_buttonsPressed == 0) {
      buttonPressSubscription.cancel();
      _gameComplete.complete(null);
    } else {
      buttonPressSubscription.cancel();
      _gameComplete.complete(stopwatch.elapsedMilliseconds / _buttonsPressed);
    }
  }
}
