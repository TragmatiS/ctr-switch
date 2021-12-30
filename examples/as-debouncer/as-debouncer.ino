#include "ctr-switch.h"

/*
CTR Switch as a debouncer

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

//how long to consider the button "bouncing"?
long debounce_threshold_ms=200;

/*
Specify a function which checks whether the button is pressed.
We will later feed it into our ctr_switch
*/
bool is_pressed() {
	return digitalRead(button_pin);
}

/*
Create a ctr_switch to debounce our button using
is_pressed as the function to ctr-switch and
millis as the source of time.

We could also use a shortcut specifically designed for millis:
ctr_switch_ms<is_pressed> debouncer(debounce_threshold_ms);
*/
ctr_switch<is_pressed, millis> debouncer(debounce_threshold_ms);

void setup() {
	pinMode(button_pin, INPUT);
	Serial.begin(9600);
}

void loop() {
	/*
	Check if it`s the peak our debouncer gets us.
	*/
	if(debouncer.get()) {
		Serial.print("The button`s pressed now!\n");  
	}
}
