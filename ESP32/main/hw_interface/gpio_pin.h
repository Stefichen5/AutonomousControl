//
// Created by stefan on 29.12.19.
//

#ifndef IMPORT_CMAKE_LIB_GPIO_PIN_H
#define IMPORT_CMAKE_LIB_GPIO_PIN_H

#include <cstdint>

#include "driver/gpio.h"

class gpio_pin {
public:
	enum gpio_pin_state{
		off = 0,
		on = 1
	};

	gpio_pin(gpio_num_t const pin);
	~gpio_pin();

	gpio_pin_state get_state() const;
	bool set_state(gpio_pin_state const state);

	bool set_mode(gpio_mode_t const mode);

	gpio_num_t get_nr() const;

private:
	gpio_num_t pin_nr = (gpio_num_t)0;
	gpio_mode_t mode = GPIO_MODE_DISABLE;
};


#endif //IMPORT_CMAKE_LIB_GPIO_PIN_H
