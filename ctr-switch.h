#ifndef CTR_SWITCH_H
#define CTR_SWITCH_H

/*
CTR Switch for Arduino

Frantsuzov Tseren
2021
*/

#include "timer.h"

/*
The CTR Switch
Transforms a boolean function, switching it to false (after a true) for a specifiable duration
*/
template<bool (*input_fn)(), time_source_function F>
class ctr_switch {
public:
	/*
	Initialize the switch.
	Setting switch_thrs to 0 basically defeats the switch
	*/	
	ctr_switch(long switch_thrs);
	
	/*
	Set a time threshold for the switch.
	Takes effect after the next time get() returns true
	*/
	void set_switch_thrs(long switch_thrs);

	/*
	Returns time since the last event.
	You shouldn`t really have any logic dependent on this;
	it`s included mainly for debug purposes.
	*/
	long get_time_since_last_event() const;

	/*
	Compute and return the switch-controlled function.
	NOTE: may or may not invoke input_fn, potentially causing its side effects.
		input_fn should better be "pure" and relatively lightweight
	NOTE: does not distinguish between callers. Either use multiple CTR switches or 
		notify all the callers simultaneously
	*/
	bool get();
private:
	timer<F> time_since_last_event;
	long switch_thrs;	
};

/*
Common arguments for F
*/
template<bool (*input_fn)()>
using ctr_switch_ms=ctr_switch<input_fn, millis>;

template<bool (*input_fn)()>
using ctr_switch_mcs=ctr_switch<input_fn, micros>;

/*
Always returns true
Useful when making a pulse timer out of a ctr_switch:
ctr_switch<functional_true, ...> ...
*/
bool functional_true();

/*
A CTR Switch with a switch_thrs of effectively positive infinity:
it will only activate once.
Intended as a "plug" for debug purposes. The template signature is deliberately left different
NOTE: does not provide time since last activation since there is no point in storing it
*/
template<bool (*input_fn)()>
class ctr_switch_single {
public:
	ctr_switch_single();

	/*
	The same get() as in the CTR Switch
	*/
	bool get();
private:
	bool had_activated;
};

/*
Same-file definitions
*/

template<bool (*input_fn)(), time_source_function F>
ctr_switch<input_fn, F>::ctr_switch(long switch_thrs) :
	time_since_last_event(switch_thrs+1),
	switch_thrs(switch_thrs)
{}

template<bool (*input_fn)(), time_source_function F>
void ctr_switch<input_fn, F>::set_switch_thrs(long switch_thrs) {
	//correct the elapsed time to avoid inconsistencies
	time_since_last_event.reset(
		time_since_last_event+switch_thrs- this->switch_thrs
	);
	this->switch_thrs=switch_thrs;
}

template<bool (*input_fn)(), time_source_function F>
long ctr_switch<input_fn, F>::get_time_since_last_event() const {
	return time_since_last_event;
}

template<bool (*input_fn)(), time_source_function F>
bool ctr_switch<input_fn, F>::get() {
	if(time_since_last_event<switch_thrs) {
		return false;
	}
	bool current_r=input_fn();
	if(!current_r) {
		return false;
	}
	time_since_last_event.reset();
	return true;
}

template<bool (*input_fn)()>
ctr_switch_single<input_fn>::ctr_switch_single() :
	had_activated(false)
{}

template<bool (*input_fn)()>
bool ctr_switch_single<input_fn>::get() {
	if(had_activated) {
		return false;
	}
	if(!input_fn()) {
		return false;
	}
	had_activated=true;
	return true;
}

#endif //include guard clause