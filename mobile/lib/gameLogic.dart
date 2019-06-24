import 'dart:async';
import 'dart:math';

import 'bluetooth.dart';

const int _NUMBER_OF_LEDS = 64;

class SimpleGame {
  static const Duration GAME_DURATION = Duration(seconds: 30);

  final BtTransmitter _btTransmitter;
  final Stopwatch stopwatch = Stopwatch();
  final Completer<Duration> _gameComplete = new Completer();
  int _buttonsPressed = 0;
  int ledOn;
  StreamSubscription buttonPressSubscription;
  Random random = Random();

  SimpleGame(this._btTransmitter);

  Future<Duration> start() async {
    ledOn = random.nextInt(_NUMBER_OF_LEDS);

    Timer(GAME_DURATION, _endGame);
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

      ledOn = random.nextInt(_NUMBER_OF_LEDS);

      _btTransmitter.writePacket(
        BtMessage.mergePackets(
            [turnOffMessage, BtMessage.turnLedOn(ledOn), BtMessage.shiftOut]),
      );
    }
  }

  void _endGame() {
    if (!_gameComplete.isCompleted) {
      stopwatch.stop();
      _btTransmitter.writePacket(BtMessage.mergePackets(
          [BtMessage.turnLedOff(ledOn), BtMessage.shiftOut]));

      buttonPressSubscription.cancel();

      if (_buttonsPressed == 0)
        _gameComplete.complete(null);
      else
        _gameComplete.complete(GAME_DURATION ~/ _buttonsPressed);
    }
  }

  void cancelGame() {
    stopwatch.stop();

    if (ledOn != null) {
      _btTransmitter.writePacket(BtMessage.mergePackets(
          [BtMessage.turnLedOff(ledOn), BtMessage.shiftOut]));
    }

    buttonPressSubscription?.cancel();

    _gameComplete.complete(null);
  }
}
