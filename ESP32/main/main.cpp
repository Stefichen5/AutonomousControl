#include <iostream>
#include "hw_interface/uart_device.h"
#include "app/autonomous_controller.h"

extern "C" void app_main(void)
{
	std::cout << "Hello world" << std::endl;

	uart_device uart(UART_NUM_2, 9600, 23, 22);
	autonomous_controller controller(&uart);

	controller.get_height();
	controller.send_button(0);
	vTaskDelay(50 / portTICK_PERIOD_MS);
	controller.send_button(controller.button_2);
	vTaskDelay(50 / portTICK_PERIOD_MS);

	while (1){
		controller.send_button(0);
		vTaskDelay(50 / portTICK_PERIOD_MS);
		//fprintf(stdout, "height: %i\n",controller.get_height());
		//controller.get_height();
		//controller.send_button(controller.button_up);
		//vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}