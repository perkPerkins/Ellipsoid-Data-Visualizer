# Ellipsoid-Data-Visualizer
Displays multidimensional data by drawing ellipsoids and finding intersection points between ellipsoids. 
INPUT:
Reads EllipseDim.txt, each value (1 per line) as follows:
  ellipse center X coordinate
  ellipse center Y coordinate
  ellipse radius X 
  ellipse radius Y

Reads Values.txt
  each line is a different data point
  each attribute is separated by a comma
  the last attribute is the class

Handles 3-10 dimensions and up to 10 classes
Maximum data points is unknown (at least 150), uses vector to store points 

CONTROLS:
Mouse:
  left click on sector to invert
  right click on sector to select for swap, right click on another sector to invoke swapping with first

Keyboard:
  1-9 - randomize color of respective class 
  0 - randomize color of class 10
  h - hide red/blue ellipses
