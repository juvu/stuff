// Copyright 2018 The Flutter team. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'package:flutter/material.dart';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return new MaterialApp(
      title: 'Bear Beer App',
      home: new HomePage(),
      theme: new ThemeData(primarySwatch: Colors.blue),
    );
  }
}

class HomePage extends StatefulWidget {
  @override
  State<StatefulWidget> createState() {
    return new HomePageState();
  }
}

class HomePageState extends State<HomePage> {
  var ansStr = "44";
  var textController = new TextEditingController();
  var canOneController = new TextEditingController(text: '0');
  var canTwoController = new TextEditingController(text: '0');
  var canThreeController = new TextEditingController(text: '0');
  var canFourController = new TextEditingController(text: '0');
  var priceController = new TextEditingController(text: '0');
  @override
  Widget build(BuildContext context) {
    return new Scaffold(
      backgroundColor: Colors.white,
      body: new Container(
        padding: const EdgeInsets.all(20.0),
        child: new Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text('Wight Bear Ber App'),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.center,
              children: <Widget>[
                Expanded( // wrap your Column in Expanded
                  child: Column(
                    children: <Widget>[
                      Container(child: TextField(
                        decoration: new InputDecoration(labelText: "Can 1 (g)"),
                        keyboardType: TextInputType.number,
                        controller: canOneController,
                      )),
                    ],
                  ),
                ),
                Expanded( // wrap your Column in Expanded
                  child: Column(
                    children: <Widget>[
                      Container(child: TextField(
                        decoration: new InputDecoration(labelText: "Can 2 (g)"),
                        keyboardType: TextInputType.number,
                        controller: canTwoController,
                      )),
                    ],
                  ),
                ),
              ],
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.center,
              children: <Widget>[
                Expanded( // wrap your Column in Expanded
                  child: Column(
                    children: <Widget>[
                      Container(child: TextField(
                        decoration: new InputDecoration(labelText: "Can 3 (g)"),
                        keyboardType: TextInputType.number,
                        controller: canThreeController,
                      )),
                    ],
                  ),
                ),
                Expanded( // wrap your Column in Expanded
                  child: Column(
                    children: <Widget>[
                      Container(child: TextField(
                        decoration: new InputDecoration(labelText: "Can 4 (g)"),
                        keyboardType: TextInputType.number,
                        controller: canFourController,
                      )),
                    ],
                  ),
                ),
              ],
            ),
            TextField(
              decoration: new InputDecoration(labelText: "Price per Pint (Pence)"),
              keyboardType: TextInputType.number,
              controller: priceController,
            ),
            TextField(
              decoration: new InputDecoration(labelText: "Cost (Pence)"),
              style: TextStyle(
                fontSize: 24,
                fontWeight: FontWeight.bold,
              ),
              controller: textController,
            ),
            RaisedButton(
                child: Text(
                    'Calculate Price',
                    style: TextStyle(fontSize: 20)
                ),
                onPressed: () {
                  setState(() {
                    var canOne = double.parse(canOneController.text);
                    assert (canOne is double);
                    var canTwo = double.parse(canTwoController.text);
                    assert (canTwo is double);
                    var canThree = double.parse(canThreeController.text);
                    assert (canThree is double);
                    var canFour = double.parse(canFourController.text);
                    assert (canFour is double);
                    var price = double.parse(priceController.text);
                    assert (price is double);
                    var cost = double.parse(canOneController.text);
                    assert (cost is double);
                    cost = 0.0;
                    if (canOne > 23.0) {
                      cost =
                          cost + ((price * 1.76) * (((canOne - 23.0) * 0.001)));
                    }
                    if (canTwo > 23.0) {
                      cost =
                          cost + ((price * 1.76) * (((canTwo - 23.0) * 0.001)));
                    }
                    if (canThree > 23.0) {
                      cost =
                          cost + ((price * 1.76) * (((canThree - 23.0) * 0.001)));
                    }
                    if (canFour > 23.0) {
                      cost =
                          cost + ((price * 1.76) * (((canFour - 23.0) * 0.001)));
                    }
                    if (cost > 1.0) {
                        cost = cost + 0.5 ;
                      }
                    var intPrice = cost.toInt();
                    String myText = intPrice.toString();
                    textController.text = myText;
                  });
                }
            ),
          ],
        ),

      ),
    );
  }
}
