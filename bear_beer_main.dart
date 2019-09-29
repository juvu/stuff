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
  var textController = new TextEditingController(text: '0');
  var canOneController = new TextEditingController(text: '0');
  var canTwoController = new TextEditingController(text: '0');
  var canThreeController = new TextEditingController(text: '0');
  var canFourController = new TextEditingController(text: '0');
  var priceController = new TextEditingController(text: '0');
  var takeawayController = new TextEditingController(text: '0');
  var pintsController = new TextEditingController(text: '0');
  var chargedController = new TextEditingController(text: '0');
  var equivController = new TextEditingController(text: '0');
  var canController = new TextEditingController(text: '20');

  @override
  Widget build(BuildContext context) {
    return new Material(
      child: new Container(
        padding: const EdgeInsets.all(30.0),
        child: new SingleChildScrollView(
          child: new ConstrainedBox(
            constraints: new BoxConstraints(),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.start,
              children: <Widget>[
                Text('Wight Bear Beer App'),
                Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  crossAxisAlignment: CrossAxisAlignment.center,
                  children: <Widget>[
                    Expanded(
                      child: Column(
                        children: <Widget>[
                          Container(
                              child: TextField(
                            decoration:
                                new InputDecoration(labelText: "Can 1 (g)"),
                            keyboardType: TextInputType.number,
                            controller: canOneController,
                          )),
                        ],
                      ),
                    ),
                    Expanded(
                      child: Column(
                        children: <Widget>[
                          Container(
                              child: TextField(
                            decoration:
                                new InputDecoration(labelText: "Can 2 (g)"),
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
                    Expanded(
                      // wrap your Column in Expanded
                      child: Column(
                        children: <Widget>[
                          Container(
                              child: TextField(
                            decoration:
                                new InputDecoration(labelText: "Can 3 (g)"),
                            keyboardType: TextInputType.number,
                            controller: canThreeController,
                          )),
                        ],
                      ),
                    ),
                    Expanded(
                      // wrap your Column in Expanded
                      child: Column(
                        children: <Widget>[
                          Container(
                              child: TextField(
                            decoration:
                                new InputDecoration(labelText: "Can 4 (g)"),
                            keyboardType: TextInputType.number,
                            controller: canFourController,
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
                    Expanded(
                      // wrap your Column in Expanded
                      child: Column(
                        children: <Widget>[
                          Container(
                              child: TextField(
                            decoration: new InputDecoration(
                                labelText: "Price per pint (p)"),
                            keyboardType: TextInputType.number,
                            controller: priceController,
                            onChanged: (text) {
                              var price = double.parse(priceController.text);
                              price = price * 1.0224;
                              var intPrice = price.toInt();
                              String takeawayText = intPrice.toString();
                              takeawayController.text = takeawayText;
                            },
                          )),
                        ],
                      ),
                    ),
                    Expanded(
                      // wrap your Column in Expanded
                      child: Column(
                        children: <Widget>[
                          Container(
                              child: TextField(
                            decoration: new InputDecoration(
                                labelText: "Takeaway per pint (p)"),
                            enabled: false,
                            keyboardType: TextInputType.number,
                            controller: takeawayController,
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
                    Expanded(
                      // wrap your Column in Expanded
                      child: Column(
                        children: <Widget>[
                          Container(
                              child: TextField(
                            decoration:
                                new InputDecoration(labelText: "Cost (£)"),
                            enabled: false,
                            style: TextStyle(
                              fontSize: 24,
                              fontWeight: FontWeight.bold,
                            ),
                            controller: textController,
                          )),
                        ],
                      ),
                    ),
                    Expanded(
                      // wrap your Column in Expanded
                      child: Column(
                        children: <Widget>[
                          Container(
                              child: TextField(
                            decoration:
                                new InputDecoration(labelText: "Num Pints"),
                            enabled: false,
                            style: TextStyle(
                              fontSize: 24,
                              fontWeight: FontWeight.bold,
                            ),
                            controller: pintsController,
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
                    Expanded(
                      // wrap your Column in Expanded
                      child: Column(
                        children: <Widget>[
                          Container(
                              child: TextField(
                            decoration:
                                new InputDecoration(labelText: "Charged (£)"),
                            enabled: true,
                            style: TextStyle(
                              fontSize: 24,
                              fontWeight: FontWeight.bold,
                            ),
                            controller: chargedController,
                            keyboardType: TextInputType.number,
                            onChanged: (text) {
                              var numPints = double.parse(pintsController.text);
                              var charged =
                                  double.parse(chargedController.text);
                              var equiv = charged / numPints;
                              if (equiv >= 10.0) {
                                String equivText = equiv.toStringAsPrecision(4);
                                equivController.text = equivText;
                              } else {
                                String equivText = equiv.toStringAsPrecision(3);
                                equivController.text = equivText;
                              }
                            },
                          )),
                        ],
                      ),
                    ),
                    Expanded(
                      // wrap your Column in Expanded
                      child: Column(
                        children: <Widget>[
                          Container(
                              child: TextField(
                            decoration:
                                new InputDecoration(labelText: "Effective (£)"),
                            enabled: false,
                            style: TextStyle(
                              fontSize: 24,
                              fontWeight: FontWeight.bold,
                            ),
                            controller: equivController,
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
                    Expanded(
                      // wrap your Column in Expanded
                      child: Column(
                        children: <Widget>[
                          Container(
                              child: TextField(
                            decoration:
                                new InputDecoration(labelText: "Can Weight(g)"),
                            enabled: true,
                            controller: canController,
                                keyboardType: TextInputType.number,
                          )),
                        ],
                      ),
                    ),
                  ],
                ),
                RaisedButton(
                    child:
                        Text('Calculate', style: TextStyle(fontSize: 20)),
                    onPressed: () {
                      setState(() {
                        var numCans = int.parse(priceController.text);
                        var canWeight = double.parse(canController.text);
                        numCans = 0;
                        var canOne = double.parse(canOneController.text);
                        //assert(canOne is double);
                        var canTwo = double.parse(canTwoController.text);
                        //assert(canTwo is double);
                        var canThree = double.parse(canThreeController.text);
                        //assert(canThree is double);
                        var canFour = double.parse(canFourController.text);
                        //assert(canFour is double);
                        var price = double.parse(priceController.text);
                        //assert(price is double);
                        var cost = double.parse(canOneController.text);
                        //assert(cost is double);
                        var numPints = double.parse(priceController.text);
                        //assert(numPints is double);
                        cost = 0.0;
                        // 1.0224 is the takeaway surcharge. 500cc costs 90% of 1 pint
                        // but 90% of 568 is 511.2 so the surcharge is 511.2/500 = 1.0224
                        if (canOne > 50) {
                          cost = cost +
                              ((price * 1.76) *
                                  1.0224 *
                                  (((canOne - canWeight) * 0.001)));
                          numCans = numCans + 1;
                        }
                        if (canTwo > 50) {
                          cost = cost +
                              ((price * 1.76) *
                                  1.0224 *
                                  (((canTwo - canWeight) * 0.001)));
                          numCans = numCans + 1;
                        }
                        if (canThree > 50) {
                          cost = cost +
                              ((price * 1.76) *
                                  1.0224 *
                                  (((canThree - canWeight) * 0.001)));
                          numCans = numCans + 1;
                        }
                        if (canFour > 50) {
                          cost = cost +
                              ((price * 1.76) *
                                  1.0224 *
                                  (((canFour - canWeight) * 0.001)));
                          numCans = numCans + 1;
                        }
                        var charged = 0.0;
                        if (canOne > 600.0) {
                          charged = charged + (price * 1.8 *0.01);
                        }
                        else if (canOne > 50.0) {
                          charged = charged + (price * 0.9 *0.01);
                        }
                        if (canTwo > 600.0) {
                          charged = charged + (price * 1.8 *0.01);
                        }
                        else if (canTwo > 50.0) {
                          charged = charged + (price * 0.9 *0.01);
                        }
                        if (canThree > 600.0) {
                          charged = charged + (price * 1.8 *0.01);
                        }
                        else if (canThree > 50.0) {
                          charged = charged + (price * 0.9 *0.01);
                        }
                        if (canFour > 600.0) {
                          charged = charged + (price * 1.8 *0.01);
                        }
                        else if (canFour > 50.0) {
                          charged = charged + (price * 0.9 *0.01);
                        }
                        if (charged >= 10.0) {
                          String chargeText = charged.toStringAsPrecision(4);
                          chargedController.text = chargeText;
                        } else {
                          String chargeText = charged.toStringAsPrecision(3);
                          chargedController.text = chargeText;
                        }
                        numPints = cost / (price * 1.0224);
                        String pintsText = numPints.toStringAsPrecision(3);
                        pintsController.text = pintsText;
                        cost = cost * 0.01;
                        if (cost >= 10.0) {
                          String myText = cost.toStringAsPrecision(4);
                          textController.text = myText;
                        } else {
                          String myText = cost.toStringAsPrecision(3);
                          textController.text = myText;
                        }
                        var equiv = charged / numPints;
                        if (equiv >= 10.0) {
                          String equivText = equiv.toStringAsPrecision(4);
                          equivController.text = equivText;
                        } else {
                          String equivText = equiv.toStringAsPrecision(3);
                          equivController.text = equivText;
                        }
                      });
                    }),
              ],
            ),
          ),
        ),
      ),
    );
  }
}

