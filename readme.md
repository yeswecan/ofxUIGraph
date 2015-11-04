ofxUIGraph
==========

Introduction
------------
This addon manages complex UI hierarchy within the of-app asynchronously. Here every object has its own draw/update/touch functions, and you can add or change them in any place within app. 

Todo:
-----
Focus is a method inside the object that adds it to some privileged list to the top of its hierarchy. Every callback cycle (e.g. draw, update, touch etc) has its way to call these "privileged" objects first. The object can add itself to the focused list, and if it thinks it deserves to capture the input (like touch events) in front of others, it gets the opportunity by the callback cycle (that makes the call to the privileged first and foremost). After some time the object can remove itself from the "focused" list by calling another method.

Dependencies:
-------------
ofxJSON
ofxXmlSettings