import 'dart:async';
import 'dart:math';

import 'bluetooth.dart';

const int _NUMBER_OF_LEDS = 64;

class SimpleGame {
  static const Duration GAME_DURATION = Duration(seconds: 30);
  static final Random _random = Random();

  final BtTransmitter _btTransmitter;
  final Stopwatch stopwatch = Stopwatch();
  final Completer<Duration> _gameComplete = new Completer();

  Timer _endGameTimer;
  int _buttonsPressed = 0;
  int _ledOn;
  StreamSubscription _buttonPressSubscription;

  SimpleGame(this._btTransmitter);

  Future<Duration> start() async {
    _ledOn = _random.nextInt(_NUMBER_OF_LEDS);

    _endGameTimer = Timer(GAME_DURATION, () => _endGame());

    stopwatch.start();

    _writeLedPacket(ledOn: _ledOn);

    _buttonPressSubscription =
        _btTransmitter.buttonPresses.listen(_onButtonPress);

    return _gameComplete.future;
  }

  void _writeLedPacket({int ledOn, int ledOff}) {
    List<BtMessage> messages = [];

    if (ledOn != null) {
      messages.add(BtMessage.turnLedOn(ledOn));
    }

    if (ledOff != null) {
      messages.add(BtMessage.turnLedOff(ledOff));
    }

    messages.add(BtMessage.shiftOut);

    _btTransmitter.writePacket(BtMessage.mergePackets(messages));
  }

  void _onButtonPress(int buttonNumber) {
    if (buttonNumber == _ledOn) {
      _buttonsPressed++;

      int previousLed = _ledOn;
      _ledOn = _random.nextInt(_NUMBER_OF_LEDS);

      _writeLedPacket(ledOn: _ledOn, ledOff: previousLed);
    }
  }

  void _endGame({bool isCanceled = false}) {
    stopwatch.stop();

    if (_ledOn != null) {
      _writeLedPacket(ledOff: _ledOn);
    }

    _buttonPressSubscription?.cancel();

    if (_buttonsPressed == 0 || isCanceled)
      _gameComplete.complete(null);
    else
      _gameComplete.complete(GAME_DURATION ~/ _buttonsPressed);
  }

  void cancelGame() {
    _endGameTimer?.cancel();
    _endGame(isCanceled: true);
  }
}
