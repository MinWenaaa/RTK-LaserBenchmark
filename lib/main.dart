import 'package:flutter/material.dart';
import 'package:geo_mobile/source/global_model.dart';
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
      title: 'Flutter Demo',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: Scaffold(
        body: Container(
            color: Colors.white,
            child: const HomePage()),
      )
    );
  }
}


