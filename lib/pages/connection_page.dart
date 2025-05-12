import 'package:flutter/material.dart';
import 'package:geo_mobile/source/global_model.dart';
import 'package:geo_mobile/source/theme.dart';
import 'package:provider/provider.dart';

class ConnectionPage extends StatefulWidget {
  const ConnectionPage({super.key});

  @override
  State<ConnectionPage> createState() => _ConnectionPageState();
}

class _ConnectionPageState extends State<ConnectionPage> {

  TextEditingController _hostControler = TextEditingController();
  TextEditingController _portControler = TextEditingController();


  static const List<Color> colors = [Colors.black26, Colors.green, Colors.blue];
  static const List<String> text = ["连接中...", "连接成功", "连接"];
  @override
  Widget build(BuildContext context) {
    return Container(
      height: 400,
      padding: EdgeInsets.all(16.0),
      child: Column(
        children: [
          Text("连接服务器", style: IndSoft.instance.standardText,),
          const SizedBox(height: 20),
          Padding(
            padding: const EdgeInsets.all(8.0),
            child: TextField(
              controller: _hostControler,
              decoration: IndSoft.instance.inputTheme("IP Address"),
            ),
          ),
          Padding(
            padding: const EdgeInsets.all(8.0),
            child: TextField(
              controller: _portControler,
              decoration: IndSoft.instance.inputTheme("port"),
            ),
          ),
          const SizedBox(height: 15),
          Selector<ConnectionProvider, AncherConnectionState>(
            selector: (context, provider) => provider.state,
            builder: (context, state, child) => OutlinedButton(
              onPressed: () => Provider.of<ConnectionProvider>(context, listen: false).connectToServer(_hostControler.text, int.parse(_portControler.text)),
              child: Text(text[state.index], style: IndSoft.instance.standardText,),
              style: OutlinedButton.styleFrom(
                minimumSize: Size(160, 40),
                shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8)
                ),
                backgroundColor: colors[state.index]
              ),
            )
          )
        ],
      ),
    );
  }
}

