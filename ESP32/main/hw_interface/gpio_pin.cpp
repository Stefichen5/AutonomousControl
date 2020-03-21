//
// Created by stefan on 29.12.19.
//

#include "gpio_pin.h"

gpio_pin::gpio_pin(const gpio_num_t pin) {
	pin_nr = pin;
}

gpio_pin::gpio_pin_state gpio_pin::get_state() const {
	auto level = gpio_get_level(pin_nr);

	if (level == 0) {
		return off;
	} else {
		return on;
	}
}

bool gpio_pin::set_state(const gpio_pin::gpio_pin_state state) {
	auto retval = gpio_set_level(pin_nr, (uint32_t)state);

	return retval == ESP_OK;

}

gpio_pin::~gpio_pin() {
	gpio_set_level(pin_nr, off);
}

bool gpio_pin::set_mode(gpio_mode_t const mode) {
	auto retval = gpio_set_direction(pin_nr, mode);

	return retval == ESP_OK;
}

gpio_num_t gpio_pin::get_nr() const {
	return pin_nr;
}
