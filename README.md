# Arduino-based-alarm-system ⏰ 
this is a simple arduino based project whose working revolves around the rtc time module.

## compnents required
1. 1x 16*2 lcd screen
2. 1x arduino nano
3. 3x momentary switch/buttons
4. 1x of breadboard for testing (use can use veroboard if you are confident with soldering).
5. 1x buzzer
6. 1x varible resitor

## wiring of the lcd
the rs, en, d4, d5, d6, d7, d8 pins are connected to d2, d3, d4, d5, d6, d7 of the arduino pins respectively

## buzzzer wiring
the buzzer is used to make sound as you interact with the system such as pressing button it is also used to sound the alarm, the vcc pin(positive)+ of 
the buzzer connected to the d8 pin of the arduino board and the ground pin(negative)- is connected to the any gnd pin of the arduino.

## workings of the momentary switch/buttons 
there are three button used to set and control the alarm system the first one is the left button, the config button, and the right button.
the config button is used to set the system mode such as cancelling/stoping an active alarm ⏰ and setting the alarm time.
the left and right buttons are used to increment and decrement values respectively such as the alarm hour, minites, and seconds.
when the system is in set alarm time the config button is used to switch betwsen setting the hour minites and seconds of the desired time you want the alarm to sound.
### left button connection 
the left button is connected to the d9 of the arduino board and the second pin is connected to the ground.
### config button connection
the congig button is connected to the d10 pin and the second pin is connected to the ground.
### the right button connection
the right button is connected to the d11 pin and the second pin is conected to the ground.
