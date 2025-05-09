import 'dart:ui';

import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';

class IndSoft {
  IndSoft._internal();
  static final IndSoft _instance = IndSoft._internal();
  static IndSoft get instance => _instance;

  ButtonStyle get primButton => ElevatedButton.styleFrom(
    minimumSize: const Size(160, 40),
    backgroundColor: primary,
    foregroundColor: Colors.white,
    textStyle: const TextStyle(fontSize: 14, fontWeight: FontWeight.w500),
    shape: RoundedRectangleBorder(
      borderRadius: BorderRadius.circular(4)
    )
  );

  TextStyle get standardText => const TextStyle(
    color: Colors.black87, fontSize: 14,
  );

  InputDecoration inputTheme(String hintText) => InputDecoration(
    isDense: true,
    hintText: hintText,
    hintStyle: TextStyle(color: border, fontSize: 14, fontWeight: FontWeight.w500),
    border: OutlineInputBorder(
      borderRadius: BorderRadius.circular(8),
      borderSide: BorderSide(color: text3, width: 1),
    ),
    enabledBorder: OutlineInputBorder(
      borderRadius: BorderRadius.circular(8),
      borderSide: BorderSide(color: border, width: 1), 
    ),
    focusedBorder: OutlineInputBorder(
      borderRadius: BorderRadius.circular(8),
      borderSide: BorderSide(color: text3, width: 1),
  ),
  );

  Color get primary => const Color(0xFF2563EB);
  Color get deepBack => const Color(0xFFF9FAFB);
  Color get text1 => const Color(0xFF1F2937);
  Color get text2 => const Color(0xFF374151);
  Color get text3 => const Color(0xFF6B7280);
  Color get icon => const Color(0xFF9CA3AF);
  Color get border => const Color(0xFFD1D5DB);
  Color get error => const Color(0xFFF43F5E);
  Color get warning => const Color(0xFFFFA500);
  Color get success => const Color(0xFF22C55E);
}