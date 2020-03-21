#include <iostream>
#include <driver/gpio.h>
#include "hw_interface/uart_device.h"
#include "hw_interface/gpio_pin.h"
#include "app/autonomous_controller.h"

extern "C" void app_main(void)
{
	std::cout << "Hello world" << std::endl;

 	uart_device uart(UART_NUM_2, 9600, 23, 22);
	autonomous_controller controller(&uart);

	gpio_pin up_button(GPIO_NUM_32);
	gpio_pin down_button(GPIO_NUM_27);

	up_button.set_direction(GPIO_MODE_INPUT);
	down_button.set_direction(GPIO_MODE_INPUT);
	up_button.enable_pullup(true);
	down_button.enable_pullup(true);

	//controller.get_height();
	//controller.go_to_height(0x4D);

	while (true){
		vTaskDelay(200 / portTICK_PERIOD_MS);
		if(up_button.get_state() == gpio_pin::off){
			controller.go_up();
			//std::cout << "up" << std::endl;
		} else if(down_button.get_state() == gpio_pin::off){
			//std::cout << "down" << std::endl;
			controller.go_down();
		}
		std::cout << controller.get_height_inch() << std::endl;
	}
}