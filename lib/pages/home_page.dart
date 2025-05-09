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
  @override
  void didChangeDependencies() {
    log("HomePage: didChangeDependencies called!");
    super.didChangeDependencies();
    final state = Provider.of<ConnectionProvider>(context).state;
    if (state == AncherConnectionState.error) {
      WidgetsBinding.instance.addPostFrameCallback((_) {
        showModalBottomSheet(
          context: context,
          builder: (context) => const ConnectionPage(), backgroundColor: Colors.white
        );
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        _bitmapViewer(),
        _measureDiv(),
        const SizedBox(height: 10),
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
      aspectRatio: 1,
      child: Stack(
        children: [
          Consumer<ConnectionProvider>(
            builder: (context, provider, child) {
              if (provider.bitmapImage.isNotEmpty) {
                return Image.memory(provider.bitmapImage);
              } else {
                return SizedBox.expand(
                  child: ColoredBox(color: IndSoft.instance.text1));
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
            margin: const EdgeInsets.symmetric(horizontal: 12, vertical: 12),
            padding: const EdgeInsets.symmetric(vertical: 12),
            decoration: BoxDecoration(
              color: IndSoft.instance.deepBack,
              borderRadius: BorderRadius.circular(8),
            ),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceAround,
              children: [
                _coord(axis: "X", value: 43.12),
                _coord(axis:"Y", value: 19.46),
                _coord(axis:"Z", value: 12.34),
              ],)
            ),
          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 4),
            child: SizedBox(
              width: double.infinity, height: 48,
              child: TextField(
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
              onPressed: (){}, 
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
        Text("$value", style: TextStyle(color: IndSoft.instance.primary, fontSize: 20, fontWeight: FontWeight.bold)),
      ],
    );
  }

  Widget _historyList() {
    return Container(
      margin: const EdgeInsets.symmetric(horizontal: 12),
        color: Colors.white,
        child: ListView.builder(
          itemCount: 10,
          itemBuilder: (context, index) {
            return ListTile(
              title: Text("测量点 $index"),
              subtitle: Text("坐标: (x, y, z)"),
              trailing: IconButton(
                icon: const Icon(Icons.delete),
                onPressed: () {
                  // Handle delete action
                },
              ),
            );
          },
        ),
    
    );
  }
}

