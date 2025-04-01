import 'dart:convert';
import 'dart:io';
import 'package:flutter/cupertino.dart';

enum AncherConnectionState {
  connecting, connected, error
}

class ConnectionProvider with ChangeNotifier {
  AncherConnectionState _state = AncherConnectionState.error;
  String? _response;
  Socket? _socket;

  AncherConnectionState get state => _state;
  String? get response => _response;

  Future<void> connectToServer(String host, int port) async {
    print("ConnectionProvider: start connection!");
    try {
      _state = AncherConnectionState.connecting;
      _socket = await Socket.connect(host, port);
      _socket?.listen((List<int> data) {
          _response = utf8.decode(data);
          notifyListeners();
        },
        onError: (error) {
          _response = 'Error: $error';
          notifyListeners();
        },
        onDone: (){
          _state = AncherConnectionState.error;
          notifyListeners();
        }
      );
    } catch (e) {
      _state = AncherConnectionState.error;
      _response = "Error: $e";
      print("ConnectionProvider: connection error: $e");
    }
    _state = AncherConnectionState.connected;
    notifyListeners();
  }

  Future<void> sendMessage(String message) async {
    if (_socket == null || _state != AncherConnectionState.connected) {
      throw Exception("Socket is not connected");
    }

    try {
      _socket!.write(message);
      print("Message sent: $message");
    } catch (e) {
      _state = AncherConnectionState.error;
      _response = "Error sending message: $e";
      notifyListeners();
      print("Error sending message: $e");
      rethrow;
    }
  }
}