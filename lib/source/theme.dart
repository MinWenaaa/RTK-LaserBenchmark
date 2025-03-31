import 'dart:ui';

import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';

class IndSoft {
  IndSoft._internal();
  static final IndSoft _instance = IndSoft._internal();
  static IndSoft get instance => _instance;

  ButtonStyle get buttonStyle => ElevatedButton.styleFrom(
    minimumSize: Size(160, 40),
    shape: RoundedRectangleBorder(
      borderRadius: BorderRadius.circular(8)
    )
  );

  TextStyle get standardText => TextStyle(
    color: Colors.black87, fontSize: 16,
  );

  InputDecoration inputTheme(String hintText) => InputDecoration(
    isDense: true,
    hintText: hintText,
    border: OutlineInputBorder(
      borderRadius: BorderRadius.circular(8),
      borderSide: BorderSide(color: Colors.grey.shade400),
    )
  );
}