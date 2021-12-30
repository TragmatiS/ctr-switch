#include "ctr-switch.h"

/*
Using the signle-activation CTR Switch

NOTE: identical to as-debouncer except for the switch used; read it before this

Frantsuzov Tseren
2021
*/

/*
Setup: there needs to be 
a button on pin 4 such that pin 4 is HIGH when pressed and LOW otherwise
and a working Serial.
*/

//button pin
int button_pin=4;

/*
Specify a function which checks whether the button is pressed.
We will later feed it into our ctr_switch
*/
bool is_pressed() {
	return digitalRead(button_pin);
}

/*
Create a ctr_switch to debounce our button using
is_pressed as the function to ctr-switch.
It will only activate once when the button is first pressed.
*/
ctr_switch_single<is_pressed> debouncer{};

void setup() {
	pinMode(button_pin, INPUT);
	Serial.begin(9600);
}

void loop() {
	/*
	Check if it`s the peak our debouncer gets us.
    Note that it will only activate once; any subsequent presses will be discarded.
	*/
	if(debouncer.get()) {
		Serial.print("The button`s pressed now!\n");  
	}
}