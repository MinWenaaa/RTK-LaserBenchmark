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

  BytesBuilder _bitmapImageBuilder = BytesBuilder();
  Uint8List _receiveBuffer = Uint8List(0);  

  void _handleIncomingData(List<int> data) {
    _receiveBuffer = Uint8List.fromList([..._receiveBuffer.buffer.asUint8List(), ... data]);
    log("Received data: ${data.length} bytes, now data is ${_receiveBuffer.lengthInBytes} bytes");

    while (_receiveBuffer.lengthInBytes >= 5) {
      final int type = _receiveBuffer[0];

      log("data to process: ${_receiveBuffer.lengthInBytes} bytes, type: $type");
      if (type == 1) {
        // 位图数据
        if (_receiveBuffer.lengthInBytes < 5) break; // 确保至少有头部数据
        final int batchSize = _receiveBuffer[1]; // 总切片数
        final int currentBatch = _receiveBuffer[2]; // 当前切片编号
        final int chunkSize = ByteData.sublistView(_receiveBuffer, 3, 5).getUint16(0, Endian.little); // 当前切片大小

        if (_receiveBuffer.lengthInBytes < 5 + chunkSize) break;

        _bitmapImageBuilder.add(_receiveBuffer.sublist(5, 5 + chunkSize));

        log("Chunk $currentBatch/$batchSize received, size: $chunkSize bytes");

        _receiveBuffer = _receiveBuffer.sublist(5 + chunkSize);

        if (currentBatch + 1 == batchSize) {
          _bitmapImage = _bitmapImageBuilder.takeBytes();
          _bitmapImageBuilder = BytesBuilder(); // 重置
          log("Full image received, total size: ${_bitmapImage.length} bytes");
          notifyListeners();
        }

      } else if(type ==2){
        // 坐标数据流
        currentCoord[0] = ByteData.sublistView(_receiveBuffer, 1, 5).getFloat32(0, Endian.little); 
        currentCoord[1] = ByteData.sublistView(_receiveBuffer, 5, 9).getFloat32(0, Endian.little); 
        currentCoord[2] = ByteData.sublistView(_receiveBuffer, 9, 13).getFloat32(0, Endian.little); 
        _receiveBuffer = _receiveBuffer.sublist(13);
        
        //_response = "Coordinate data received";
        notifyListeners();
      } else if (type==3) {
        // 测量数据
        final int nameLen = _receiveBuffer[1]; 
        final String name = String.fromCharCodes(_receiveBuffer.sublist(2, 2 + nameLen)); 
        final int start = 2 + nameLen;
        final double x = ByteData.sublistView(_receiveBuffer, start, start+8).getFloat64(0, Endian.little); 
        final double y = ByteData.sublistView(_receiveBuffer, start+8, start+16).getFloat64(0, Endian.little);
        final double z = ByteData.sublistView(_receiveBuffer, start+16, start+24).getFloat64(0, Endian.little); 

        measurePointsNotifier.value = [...measurePointsNotifier.value, MeasurePoint(name: name, x: x, y: y, z: z)];
        _receiveBuffer = _receiveBuffer.sublist(2 + nameLen + 24);
        _response = "Measurement data received.";
        
        log("Measurement data received: $name, x=$x, y=$y, z=$z");
        notifyListeners();
      } else if (type == 4) {
        // 图片刷新
        _bitmapImage = Uint8List(0);
        log("now data is ${_receiveBuffer.lengthInBytes} bytes");
        _receiveBuffer = _receiveBuffer.sublist(2);
        
      } else {
        log("now data is ${_receiveBuffer.lengthInBytes} bytes");
        notifyListeners();
        break; 
      }
      log("now data is ${_receiveBuffer.lengthInBytes} bytes");
    } 
  }

}