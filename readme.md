ofxUIGraph
==========

Introduction
------------
This openFrameworks addon manages complex UI hierarchy within the of-app asynchronously. Here every object has its own draw/update/input functions, and you can add or change them in any place within app.

The features include:
* Hierarchies of any complexity, managable in a simplest way possible
* Your widgets and views are animatable with custom easing functions, the default one is also provided
* You can use animation engine to animate your own properties of practically any kind effectively
* All the app input is streamlined in order to support multiple platforms, be it a touch-enabled device or a computer with mouse and keyboard. It'll use as little platform code as possible.
* Optional constraint solver enables you to easily manage flexible UI layouts
* Event system provides an efficient way to program complex widget behaviours
* You can use Gesture Recognizers to decode and use basic input patterns (like dragging or pinching) easily

More documentation (and examples) is coming soon.

Dependencies:
-------------
ofxLiquidEvent