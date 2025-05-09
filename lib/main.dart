import 'package:flutter/material.dart';
import 'package:geo_mobile/source/global_model.dart';
import 'package:geo_mobile/source/theme.dart';
import 'pages/home_page.dart';
import 'package:provider/provider.dart';

// flutter build apk --split-per-abi

void main() {
  runApp(ChangeNotifierProvider(
      create: (_) => ConnectionProvider(),
      child: const MyApp())
  );
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Laser Tracker Mobile',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: IndSoft.instance.primary),
        useMaterial3: true,
      ),
      home: Scaffold(
        appBar: AppBar(
          toolbarHeight: 52,
          title: const Text("Laser Tracker Mobile", 
            style: TextStyle(color: Colors.white, fontSize: 18, fontWeight: FontWeight.bold),),
          backgroundColor: IndSoft.instance.primary,
        ),
        body: Container(
            color: IndSoft.instance.deepBack,
            child: const HomePage()),
      )
    );
  }
}


