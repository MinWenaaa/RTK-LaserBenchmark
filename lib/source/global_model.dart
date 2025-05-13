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

  Future<void> measureCommand(String name) async {
    if (_socket == null || _state != AncherConnectionState.connected) {
      log("Socket is not connected");
      throw Exception("Socket is not connected");
    }

    try {
      final List<int> data = [1]; 
      data.addAll(name.codeUnits); 
      _socket!.add(data);
      log("Measurement command sent: $name");
    } catch (e) {
      log("Error sending measurement command: $e");
      throw Exception("Error sending measurement command: $e");
    }
  }

  Future<void> removeMeasurePoint(int index) async {
    String name = measurePointsNotifier.value[index].name;
    measurePointsNotifier.value = List.from(measurePointsNotifier.value)..removeAt(index);

    try {
      final List<int> data = [2]; 
      data.addAll(name.codeUnits); 
      _socket!.add(data);
      log("Measurement point removed: $name");
    } catch (e) {
      log("Error removing measurement point: $e");
      throw Exception("Error removing measurement point: $e");
    }
  }

  // 测量数据
  List<double> currentCoord = [0, 0, 0]; 
  final ValueNotifier<List<MeasurePoint>> measurePointsNotifier = ValueNotifier([]);
  late Uint8List _bitmapImage = Uint8List(0);
  Uint8List get bitmapImage => _bitmapImage;

  void _handleIncomingData(List<int> data) {
    try {
      final ByteData byteData = ByteData.sublistView(Uint8List.fromList(data));
      final int type = byteData.getUint8(0);
      log("command $type received");
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
        //_response = "Coordinate data received";
        notifyListeners();
      } else if (type==3) {
        // 测量数据
        final int nameLen = byteData.getUint8(1); 
        final String name = String.fromCharCodes(data.sublist(2, 2 + nameLen)); 
        final double x = byteData.getFloat64(2 + nameLen, Endian.little); 
        final double y = byteData.getFloat64(2 + nameLen + 8, Endian.little);
        final double z = byteData.getFloat64(2 + nameLen + 16, Endian.little); 

        measurePointsNotifier.value = [...measurePointsNotifier.value, MeasurePoint(name: name, x: x, y: y, z: z)];
        _response = "Measurement data received.";
        log("Measurement data received: $name, x=$x, y=$y, z=$z");
        notifyListeners();
      } else {
        log("${data.length} bytes received");
        notifyListeners();
      }
    } catch (e) {
      log("Error handling incoming data: $e");
    }
  }

}