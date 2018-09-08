import 'package:flutter/material.dart';
import 'bluetooth.dart';

class GamePage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Game"),
      ),
      body: StatefulGamePage(),
    );
  }
}

class StatefulGamePage extends StatefulWidget {
  @override
  _StatefulGamePageState createState() => _StatefulGamePageState();
}

class _StatefulGamePageState extends State<StatefulGamePage> {
  BluetoothTransmitter bluetoothManager;

  @override
  void initState() {
    super.initState();

    WidgetsBinding.instance
        .addPostFrameCallback((_) => _showConnectingDialog());
  }

  _showConnectingDialog() async {
    BluetoothConnection bluetoothConnection = await showDialog<BluetoothConnection>(
        context: context,
        barrierDismissible: false,
        builder: (BuildContext context) {
          return ConnectingDialog();
        });

    if (bluetoothConnection == null) Navigator.pop(context);

    bluetoothManager = BluetoothTransmitter(bluetoothConnection);
  }

  @override
  void dispose() {
    bluetoothManager?.disconnect();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        children: <Widget>[
          Text("Time"),
          RaisedButton(
              onPressed: () => bluetoothManager?.doAction(),
              child: Text("Do action"))
        ],
      ),
    );
  }
}

class ConnectingDialog extends StatefulWidget {
  @override
  _ConnectingDialogState createState() =>
      _ConnectingDialogState();
}

class _ConnectingDialogState extends State<ConnectingDialog> {
  BluetoothConnector _bluetoothConnector = BluetoothConnector();
  String _currentMessage = "";
  bool connectingFailed = false;

  _ConnectingDialogState();

  @override
  void initState() {
    super.initState();

    _bluetoothConnector.connect();

    _bluetoothConnector.statusStream.listen((ConnectorUpdate update) {
      switch (update.state) {
        case ConnectorStatus.connected:
          Navigator.pop(context, _bluetoothConnector.bluetoothConnection);
          break;
        case ConnectorStatus.connecting:
          setState(() {
            _currentMessage = update.message;
            connectingFailed = false;
          });
          break;
        case ConnectorStatus.failed:
          setState(() {
            _currentMessage = update.message;
            connectingFailed = true;
          });
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
        title: Text("Connecting to board"),
        content: Row(
          mainAxisAlignment: MainAxisAlignment.center,
          crossAxisAlignment: CrossAxisAlignment.center,
          children: <Widget>[
            connectingFailed
                ? null
                : Padding(
                    child: CircularProgressIndicator(),
                    padding: EdgeInsets.all(10.0),
                  ),
            Flexible(child: Text(_currentMessage))
          ].where((element) => element != null).toList(),
        ),
        actions: <Widget>[
          connectingFailed
              ? FlatButton(
                  onPressed: () => _bluetoothConnector.connect(),
                  child: Text("Try Again"),
                )
              : null,
          FlatButton(
              onPressed: () => Navigator.pop(context),
              child: Text("Cancel"))
        ]);
  }
}
