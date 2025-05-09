import 'package:flutter/material.dart';
import 'package:geo_mobile/source/theme.dart';
import 'package:provider/provider.dart';

import '../source/global_model.dart';


class MeasurementDialog extends StatefulWidget {
  const MeasurementDialog({super.key});

  @override
  State<MeasurementDialog> createState() => _MeasurementDialogState();
}

class _MeasurementDialogState extends State<MeasurementDialog> {
  TextEditingController _nameControl = TextEditingController();

  @override
  Widget build(BuildContext context) {
    return Dialog(
      child: Container(
        padding: EdgeInsets.all(20),
        width: 360, height: 280,
        color: Colors.white,
        child: Column (
          children: [
            Text("开始测量", style: IndSoft.instance.standardText),
            const SizedBox(height: 30),
            TextField(
              controller: _nameControl,
              decoration: IndSoft.instance.inputTheme("点名"),
            ),
            const SizedBox(height: 20),
            OutlinedButton(
              onPressed: () {
                Provider.of<ConnectionProvider>(context, listen: false).sendMessage(_nameControl.text);
                Navigator.pop(context);
              },
              child: Text("确定", style: IndSoft.instance.standardText,),
              style: IndSoft.instance.primButton,
            )
          ]
        )
      )
    );
  }
}
