#include <CapacitiveSensor.h>

CapacitiveSensor cs = CapacitiveSensor(4,8);        // 10 megohm resistor between pins 4 & 8, pin 4 is sensor pin, add wire, foil



void setup() {

    cs.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
    Serial.begin(9600);


}

void loop() {

    long sensing_value =  cs.capacitiveSensor(30);


    Serial.print("Value is: ");
    Serial.println(sensing_value);                  // print sensor output 1

    delay(50);                             // arbitrary delay to limit data to serial port 

}
