#include "ctr-switch.h"

/*
Pulse Timer via CTR Switch

Frantsuzov Tseren
2021
*/

/*
Setup: there needs to be 
a working Serial.
*/

/*
Suppose you have a thing that you must not do too often...
*/
void do_not_call_me_often() {
	Serial.print("Do not call me often!\n");
}

/*
...so we want to ensure there are no less than min_delay_ms milliseconds between each
do_not_call_me_often() call.
*/
long min_delay_ms=500;

/*
Worry not! ctr_switch as a Pulse Timer can easily deal with that and 
spare you the hassle of working with time functions.
*/

/*
Create a ctr_switch_ms to avoid calling do_not_call_me_often() too often,
using functional_true as the function to ctr-switch.
This way we only feed true into our ctr_switch, thus making it act as a pulse timer.

We could also use the general form:
ctr_switch<functional_true, millis> pulse_timer(min_delay_ms);
*/
ctr_switch_ms<functional_true> pulse_timer(min_delay_ms);

void setup() {
	Serial.begin(9600);
}

void loop() {
	/*
	The pulse timer will now automagically take care of returning false 
	whenever there`s too little time since the last do_not_call_me_often() call.
	*/
	if(pulse_timer.get()) {
		//at least min_delay_ms since the last call
		do_not_call_me_often();
	}
	//do something else in the meantime
}