/*
Library examples for TM1638.

Copyright (C) 2011 Ricardo Batista <rjbatista at gmail dot com>

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <TM1638.h>
#include <bithacks.h>

// define a module on data pin 8, clock pin 9 and strobe pin 7
TM1638 module(8, 9, 7);

void setup() {
   Serial.begin(115200);

}

int index = 0;

byte button_toggle = 0;

void loop() {

  byte keys = module.getButtons();

  if(keys > 0 && button_toggle == 0){
    
      button_toggle = 5;
    
      if B_IS_SET(keys, 0){
        index += 1;
      } else if  B_IS_SET(keys, 1){
        index -= 1;
      } else if B_IS_SET(keys, 2){
        button_toggle = 0;
        index += 10;
      } else if B_IS_SET(keys, 3){
        button_toggle = 0;
        index -= 10;
      }

      index = index < 0 ? 0 : index;
      
  } else if (keys > 0 && button_toggle > 0){
      // Do nothing until button is released
      button_toggle--;
  } else {
      button_toggle = 0;
  }

  module.setDisplayToDecNumber(index, 0, true);

  module.setLEDs(0);
  module.setLED(1, index % 8);

  delay(100);
  
}
