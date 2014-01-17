Noise-meter using Arduino and Philips Hue
=========================================

A noise meter using Arduino and [Philips Hue](http://www.meethue.com/). A microphone is connected to the A0 port of the Arduino board to meassure the noise level (some normalization is done to the analog values because I used a cheap microphone, so yor mileage may vary :-/). The average noise level for 1 second is calculated and transformed into hue values to change the light bulb colors (in this case I'm using 5 levels: blue, green, yellow, orange and red). 
