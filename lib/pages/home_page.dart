import 'dart:developer';
import 'package:flutter/material.dart';
import 'package:geo_mobile/pages/connection_page.dart';
import 'package:geo_mobile/source/global_model.dart';
import 'package:geo_mobile/source/theme.dart';
import 'package:provider/provider.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  bool _isBottomSheetOpen = false;
  TextEditingController _nameController = TextEditingController();

  @override
  void didChangeDependencies() {
    //log("HomePage: didChangeDependencies called!");
    super.didChangeDependencies();
    final state = Provider.of<ConnectionProvider>(context).state;
    if (state == AncherConnectionState.error && !_isBottomSheetOpen) {
      _isBottomSheetOpen = true;
      WidgetsBinding.instance.addPostFrameCallback((_) {
        showModalBottomSheet(
          context: context,
          builder: (context) => const ConnectionPage(), 
          backgroundColor: Colors.white
        ).whenComplete(() {
          _isBottomSheetOpen = false;
        });
      });
    } else if (state == AncherConnectionState.connected) {
      _isBottomSheetOpen = false;
      //log("HomePage: connected!");
    }
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        _bitmapViewer(),
        _measureDiv(),
        const SizedBox(height: 10),
        Selector<ConnectionProvider, String?>(
          selector: (context, provider) => provider.response,
          builder: (context, response, child) {
            return Text(response ?? "等待连接",
              maxLines: 1, // 限制为一行
              overflow: TextOverflow.ellipsis, 
            );
          },
        ),
        Expanded(
          child: _historyList()
        )
        // Selector<ConnectionProvider, String?>(
        //   selector: (context, provider) => provider.response,
        //   builder: (context, str, child) => Text(str??"等待连接", style: IndSoft.instance.standardText,)),   
      ],
    );
  }

  Widget _bitmapViewer() {
    return AspectRatio(
      aspectRatio: 4.0/3.0,
      child: Stack(
        children: [
          Consumer<ConnectionProvider>(
          builder: (context, provider, child) {
            if (provider.bitmapImage.isNotEmpty) {
              // 渲染图像数据
              return Transform(
                alignment: Alignment.center,
                transform: Matrix4.rotationX(3.14159), // 旋转 180 度（倒置图片）
                child: Image.memory(
                  provider.bitmapImage,
                  fit: BoxFit.cover,
                  //gaplessPlayback: true,
                )
              );
            } else {
              return SizedBox.expand(
                child: ColoredBox(color: IndSoft.instance.text1),
              );
            }
          },
        ),
          const Positioned(
            bottom: 10, right: 10,
            child: Row(
              children: [
                Icon(Icons.camera_alt, color: Colors.white, size: 16),
                SizedBox(width: 5),
                Text("实时画面",
                  style: TextStyle(color: Colors.white, fontSize: 12),
                ),
              ],
            ),
          ),
        ]
      )
    );
  }

  Widget _measureDiv() {
    return Container(
      width: double.infinity,
      color: Colors.white,
      child: Column(
        children: [
          Container(
            alignment: Alignment.center,
            width: double.infinity, 
            margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 12),
            padding: const EdgeInsets.symmetric(vertical: 16),
            decoration: BoxDecoration(
              color: IndSoft.instance.deepBack,
              borderRadius: BorderRadius.circular(8),
            ),
            child: Selector<ConnectionProvider, List>(
              selector: (context, provider) => provider.currentCoord,
              builder: (context, coord, child) {
                return Row(
                  mainAxisAlignment: MainAxisAlignment.spaceAround,
                  children: [
                    _coord(axis: "X", value: coord[0]),
                    _coord(axis:"Y", value: coord[1]),
                    _coord(axis:"Z", value: coord[2]),
                  ],
                );
              }
            ),
          ),
          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 4),
            child: SizedBox(
              width: double.infinity, height: 48,
              child: TextField(
                controller: _nameController,
                decoration: IndSoft.instance.inputTheme("请输入测量点名称"),
                onChanged: (value) {
                  // Handle input change
                },
              )
            )
          ),
          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 4),
            child: ElevatedButton(
              onPressed: (){
                final name = _nameController.text.trim();
                  if (name.isNotEmpty) {
                    Provider.of<ConnectionProvider>(context, listen: false)
                      .measureCommand(name).then((_) {
                        _nameController.clear();
                      }).catchError((error) {
                        log("测量点记录失败: $error");
                      });
                  } else {
                    log("测量点名称不能为空");
                  }
              }, 
              style: IndSoft.instance.primButton,
              child: Container(
                width: double.infinity, height: 44,
                alignment: Alignment.center,
                child: const Text("记录当前坐标", style: TextStyle(color: Colors.white, fontSize: 14))
              ),
            )
          )
        ],
      )
    );
  }

  Widget _coord({required String axis, required double value}) {
    return Column(
      children: [
        Text("$axis 轴", style: TextStyle(color: IndSoft.instance.text3, fontSize: 16)),
        const SizedBox(height: 2),
        Text(value.toStringAsFixed(3).padLeft(8, ' '), style: TextStyle(color: IndSoft.instance.primary, fontSize: 20, fontWeight: FontWeight.bold)),
      ],
    );
  }

  Widget _historyList() {
    return ValueListenableBuilder<List<MeasurePoint>>(
    valueListenable: Provider.of<ConnectionProvider>(context, listen: false).measurePointsNotifier,
    builder: (context, measurePoints, child) {
      return Container(
        margin: const EdgeInsets.symmetric(horizontal: 12),
        color: Colors.white,
        child: ListView.builder(
          itemCount: measurePoints.length,
          itemBuilder: (context, index) {
            final point = measurePoints[index];
            return ListTile(
              title: Text(point.name),
              subtitle: Text(
                "坐标: (${point.x.toStringAsFixed(3)}, ${point.y.toStringAsFixed(3)}, ${point.z.toStringAsFixed(3)})",
              ),
              trailing: IconButton(
                icon: const Icon(Icons.delete, color: Colors.red),
                onPressed: () {
                  Provider.of<ConnectionProvider>(context, listen: false)
                      .removeMeasurePoint(index);
                },
              ),
            );
          },
        ),
      );
    },
  );
  }
}

