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
    print("HomePage: didChangeDependencies called!");
    super.didChangeDependencies();
    final state = Provider.of<ConnectionProvider>(context).state;
    if (state == AncherConnectionState.error) {
      WidgetsBinding.instance.addPostFrameCallback((_) {
        showModalBottomSheet(
          context: context,
          builder: (context) => ConnectionPage(), backgroundColor: Colors.white
        );
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Column(
          children: [
            Padding(
              padding: const EdgeInsets.all(8.0),
              child: AspectRatio(
                aspectRatio: 1,
                child: Image(image: AssetImage("asset/place_holder.webp"), fit: BoxFit.cover,),
              ),
            ),
            Expanded(
              child: Row(
                children: [
                  Flexible(
                    flex: 2,
                    child: Column(
                      children: [
                        Text("x: 39.145878", style: IndSoft.instance.standardText,),
                        Text("y: 40.148101", style: IndSoft.instance.standardText,),
                        Text("z: 1.239011", style: IndSoft.instance.standardText,)
                      ],
                    ),
                  ),
                  Flexible(
                    flex: 3,
                    child: Column(
                      children: [
                        Expanded(
                          child: Selector<ConnectionProvider, String?>(
                            selector: (context, provider) => provider.response,
                            builder: (context, str, child) => Text(str??"等待连接", style: IndSoft.instance.standardText,)),
                        ),
                        OutlinedButton(onPressed: (){},
                          child: Text("测量", style: IndSoft.instance.standardText,),
                          style: IndSoft.instance.buttonStyle,
                        )
                      ],
                    ),
                  )
                ],
              ),
            ),
          ],
        );

  }
}

