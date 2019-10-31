// Copyright 2018 The Flutter team. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'package:flutter/material.dart';

void main() => runApp(MyApp());


int hoopVal(String hoop) {
  if (hoop == "1") {
    return 0;
  }
  else if (hoop == "2") {
    return 1;
  }
  else if (hoop == "3") {
    return 2;
  }
  else if (hoop == "4") {
    return 3;
  }
  else if (hoop == "5") {
    return 4;
  }
  else if (hoop == "6") {
    return 5;
  }
  else if (hoop == "1 Back") {
    return 6;
  }
  else if (hoop == "2 Back") {
    return 7;
  }
  else if (hoop == "3 Back") {
    return 8;
  }
  else if (hoop == "4 Back") {
    return 9;
  }
  else if (hoop == "Penult") {
    return 10;
  }
  else if (hoop == "Rover") {
    return 11;
  }
  else if (hoop == "Peg") {
    return 12;
  }
  else if (hoop == "Box") {
    return 13;
  }
  return 14;
}


class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return new MaterialApp(
      title: 'Croquet Time App',
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
  List<String> clipPositions = [
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '1 Back',
    '2 Back',
    '3 Back',
    '4 Back',
    'Penult',
    'Rover',
    'Peg',
    'Box'
  ];
  String redLocation = '1';
  String yellowLocation = '5';
  String blueLocation = '3';
  String blackLocation = '2 Back';

  var redClipController = new TextEditingController(text: 'Red Clip');
  var yellowClipController = new TextEditingController(text: 'Yellow Clip');
  var blueClipController = new TextEditingController(text: 'Blue Clip');
  var blackClipController = new TextEditingController(text: 'Black Clip');

  var resultsController =
      new TextEditingController(text: 'Awaiting Results\nasdf');

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
                Text('Croquet Time App'),
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
                              border: InputBorder.none,
                            ),
                            enabled: false,
                            style: TextStyle(
                              fontSize: 14,
                              fontWeight: FontWeight.bold,
                            ),
                            textAlign: TextAlign.right,
                            controller: redClipController,
                          )),
                        ],
                      ),
                    ),
                    Expanded(
                      child: Column(
                        children: <Widget>[
                          Container(
                            child: DropdownButton(
                                value: redLocation,
                                onChanged: (newValue) {
                                  setState(() {
                                    redLocation = newValue;
                                  });
                                },
                                items: clipPositions.map((location) {
                                  return DropdownMenuItem(
                                    child: new Text(location),
                                    value: location,
                                  );
                                }).toList()),
                          )
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
                              border: InputBorder.none,
                            ),
                            enabled: false,
                            style: TextStyle(
                              fontSize: 14,
                              fontWeight: FontWeight.bold,
                            ),
                            textAlign: TextAlign.right,
                            controller: yellowClipController,
                          )),
                        ],
                      ),
                    ),
                    Expanded(
                      child: Column(
                        children: <Widget>[
                          Container(
                            child: DropdownButton(
                                value: yellowLocation,
                                onChanged: (newValue) {
                                  setState(() {
                                    yellowLocation = newValue;
                                  });
                                },
                                items: clipPositions.map((location) {
                                  return DropdownMenuItem(
                                    child: new Text(location),
                                    value: location,
                                  );
                                }).toList()),
                          )
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
                              border: InputBorder.none,
                            ),
                            enabled: false,
                            style: TextStyle(
                              fontSize: 14,
                              fontWeight: FontWeight.bold,
                            ),
                            textAlign: TextAlign.right,
                            controller: blueClipController,
                          )),
                        ],
                      ),
                    ),
                    Expanded(
                      child: Column(
                        children: <Widget>[
                          Container(
                            child: DropdownButton(
                                value: blueLocation,
                                onChanged: (newValue) {
                                  setState(() {
                                    blueLocation = newValue;
                                  });
                                },
                                items: clipPositions.map((location) {
                                  return DropdownMenuItem(
                                    child: new Text(location),
                                    value: location,
                                  );
                                }).toList()),
                          )
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
                              border: InputBorder.none,
                            ),
                            enabled: false,
                            style: TextStyle(
                              fontSize: 14,
                              fontWeight: FontWeight.bold,
                            ),
                            textAlign: TextAlign.right,
                            controller: blackClipController,
                          )),
                        ],
                      ),
                    ),
                    Expanded(
                      child: Column(
                        children: <Widget>[
                          Container(
                            child: DropdownButton(
                                hint: Text('Black Clip'),
                                value: blackLocation,
                                onChanged: (newValue) {
                                  setState(() {
                                    blackLocation = newValue;
                                  });
                                },
                                items: clipPositions.map((location) {
                                  return DropdownMenuItem(
                                    child: new Text(location),
                                    value: location,
                                  );
                                }).toList()),
                          )
                        ],
                      ),
                    ),
                  ],
                ),
                RaisedButton(
                    child: Text('Calculate', style: TextStyle(fontSize: 20)),
                    onPressed: () {
                      setState(() {
                        var redValue = hoopVal(redLocation);
                        resultsController.text = redValue.toString();
                      });
                    }),
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
                            maxLines: 10,
                            decoration:
                                new InputDecoration(labelText: "Results"),
                            enabled: false,
                            controller: resultsController,
                          )),
                        ],
                      ),
                    ),
                  ],
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}

