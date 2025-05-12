import 'dart:developer';
import 'dart:ffi';
import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/cupertino.dart';

enum AncherConnectionState {
  connecting, connected, error
}

class MeasurePoint {
  String name;
  num x, y, z;

  MeasurePoint({required this.name, required this.x, required this.y, required this.z});
}

class ConnectionProvider with ChangeNotifier {
  // 服务器状态
  AncherConnectionState _state = AncherConnectionState.error;
  String? _response;
  Socket? _socket;

  AncherConnectionState get state => _state;
  String? get response => _response;

  Future<void> connectToServer(String host, int port) async {
    log("ConnectionProvider: start connection!");
    try {
      _state = AncherConnectionState.connecting;
      notifyListeners();

      _socket = await Socket.connect(host, port);
      _state = AncherConnectionState.connected;
      notifyListeners();

      _socket?.listen((List<int> data) {
          _handleIncomingData(data);
        }, onError: (error) {
          _response = 'Error';
          notifyListeners();
        }, onDone: (){
          _state = AncherConnectionState.error;
          notifyListeners();
        }
      );
    } catch (e) {
      _state = AncherConnectionState.error;
      _response = "Error: $e";
      log("ConnectionProvider: connection error: $e");
      notifyListeners();
    }
  }

  Future<void> sendMessage(String message) async {
    if (_socket == null || _state != AncherConnectionState.connected) {
      throw Exception("Socket is not connected");
    }

    try {
      _socket!.write(message);
      log("Message sent: $message");
    } catch (e) {
      _state = AncherConnectionState.error;
      _response = "Error sending message: $e";
      notifyListeners();
      log("Error sending message: $e");
      rethrow;
    }
  }



  // 测量数据
  List<double> currentCoord = [0, 0, 0]; 
  List<MeasurePoint> _measurePoints = [];

  late Uint8List _bitmapImage = Uint8List(0);
  Uint8List get bitmapImage => _bitmapImage;

  void _handleIncomingData(List<int> data) {
    try {
      final ByteData byteData = ByteData.sublistView(Uint8List.fromList(data));
      final int type = byteData.getUint8(0);

      if (type == 1) {
        // 位图数据
        final int bitmapDataSize = byteData.getUint32(4, Endian.little);
        _bitmapImage = Uint8List.fromList(data.sublist(8, 8 + bitmapDataSize));
        _response = "Bitmap data received";
        notifyListeners();
      } else if(type ==2){
        // 坐标数据流
        currentCoord[0] = byteData.getFloat32(1, Endian.little); 
        currentCoord[1] = byteData.getFloat32(5, Endian.little); 
        currentCoord[2] = byteData.getFloat32(9, Endian.little); 
        _response = "Coordinate data received";
        notifyListeners();
      } else {
        _response = "${data.length} bytes received";
        notifyListeners();
      }
    } catch (e) {
      log("Error handling incoming data: $e");
    }
  }

}