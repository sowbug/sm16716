sm16716
=======

An Arduino controller for LED strings based on the SM16716 controller.

By [Mike Tsao](github.com/sowbug). Copyright 2012, all rights reserved.
 
You might see these being sold as WS2801 strands, and you're wondering why your off-the-shelf WS2801 code isn't working. This sketch will restore your confidence that your lights aren't broken.
 
This code isn't the fastest, but it is just about the simplest you'll find. For performance, investigate code that uses hardware SPI.
 
Beware that everyone seems to use different wiring color schemes. I found the order of the wires is more reliable. Looking at the IC side of the board, you'll find from left to right: V+, Data, Clock, Ground.
