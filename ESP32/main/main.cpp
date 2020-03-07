#include <iostream>
#include "hw_interface/uart_device.h"
#include "app/autonomous_controller.h"

extern "C" void app_main(void)
{
	std::cout << "Hello world" << std::endl;

	uart_device uart(UART_NUM_2, 9600, 23, 22);
	autonomous_controller controller(&uart);

	__uint8_t data[5] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};

	//std::cout << uart.read_data(data, 5, 1000 / portTICK_RATE_MS) << std::endl;

	//uart.send_data((char*)&data[0], 5);

	while (1){
		fprintf(stdout, "height: %i\n",controller.get_height());
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}