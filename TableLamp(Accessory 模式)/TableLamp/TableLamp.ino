/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2022 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/

#include "HomeSpan.h"         // include the HomeSpan library

struct TableLamp : Service::LightBulb{

  int lampPin;                               // store the pin number connected to a hypothetical relay that turns the Table Lamp on/off
  SpanCharacteristic *lampPower;             // store a reference to the On Characteristic
  
  TableLamp(int lampPin) : Service::LightBulb(){       // constructor() method for TableLamp defined with one parameter.  Note we also call the constructor() method for the LightBulb Service.

    lampPower=new Characteristic::On();      // instantiate the On Characteristic and save it as lampPower
    this->lampPin=lampPin;                   // save the pin number for the hypothetical relay
    pinMode(lampPin,OUTPUT);                 // configure the pin as an output using the standard Arduino pinMode function                       
    lampPower->setVal(1); 
    digitalWrite(lampPin, HIGH);
  } // end constructor()
  
  boolean update(){                          // update() method
    Serial.print("New status request");
    digitalWrite(lampPin,lampPower->getNewVal());      // use standard Arduino digitalWrite function to change the ledPin to either high or low based on the value requested by HomeKit
   
    return(true);                            // return true to let HomeKit (and the Home App Client) know the update was successful
  
  } // end update()
  
};

void setup() {     
 
  Serial.begin(115200);       // start the Serial interface
  
  homeSpan.begin();           // initialize HomeSpan

  new SpanAccessory();           // Table Lamp Accessory
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service  
      new Characteristic::Name("WallCharger");      // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories                                                    
      new Characteristic::Manufacturer("HomeSpan");   // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("123-ABC");    // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("120-Volt Lamp");     // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision("0.9");
      new Characteristic::Identify();               // HAP requires the Accessory Information Service to include the Identify Characteristic
        
    new TableLamp(1);                              // instantiate the TableLamp Service (defined below) with lampPin set to 17
  
} // end of setup()

void loop(){

 homeSpan.poll(); 

} // end of loop()
