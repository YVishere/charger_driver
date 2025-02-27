Firmware code to be implemented on nucleo L432KC using Arduino libraries through PlatformIO.  
Part of Directed Study with Professor Hoffman to build a circuit board to implement efficient charging algorithms on lipo batteries.   
The State Machine:  
IDLE --> Waits for batteries to be connected to the board. We check the open circuit voltage of uncharged batteries and if it is over a threshold, we move on to the next state. Based on the last pin to read a value over the threshold we will detemine if we are charging 2s/3s/4s. All other states will work based on the settings defined.
VOC_MEASURE --> Measures the open circuit voltage and stores outside voltage for each cell being charged. Moves on to Constant Current the moment these readings are stored.
CONST_CURR --> Measures
