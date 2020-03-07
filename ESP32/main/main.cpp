#include <iostream>
#include "hw_interface/uart_device.h"
#include "app/autonomous_controller.h"

extern "C" void app_main(void)
{
	std::cout << "Hello world" << std::endl;

	uart_device uart(UART_NUM_2, 9600, 23, 22);
	autonomous_controller controller(&uart);

	controller.get_height();
	controller.go_to_height(0x4D);
}