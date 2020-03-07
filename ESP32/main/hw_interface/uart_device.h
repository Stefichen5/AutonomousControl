//
// Created by stefan on 06.03.20.
//

#ifndef ESP_AUTONOMOUS_UART_DEVICE_H
#define ESP_AUTONOMOUS_UART_DEVICE_H


#include <stdint.h>
#include <driver/uart.h>

#define BUF_SIZE 127

class uart_device {
public:
	uart_device(uart_port_t  uart_device, int baudrate,
			int pin_tx, int pin_rx,
		    uart_word_length_t  databits = UART_DATA_8_BITS,
		    uart_parity_t  parity = UART_PARITY_DISABLE,
			uart_stop_bits_t stop_bit = UART_STOP_BITS_1,
			uart_hw_flowcontrol_t flowctrl = UART_HW_FLOWCTRL_DISABLE,
			__uint8_t flowctrl_thresh = 122);
	~uart_device();

	bool send_data(char const * data, __uint32_t const len);
	int read_data(__uint8_t * data, __uint32_t buf_size, TickType_t wait_time);

	void set_baudrate (__uint32_t baudrate);
	__uint32_t get_baudrate() const;

	void set_word_length(uart_word_length_t wordLength);
	uart_word_length_t get_word_length() const;

	void set_stop_bits(uart_stop_bits_t stopBits);
	uart_stop_bits_t get_stop_bits() const;

	size_t get_buffered_data_size() const;
	void flush();
private:
	__uint8_t data[BUF_SIZE] = {0};
	uart_port_t device;
};


#endif //ESP_AUTONOMOUS_UART_DEVICE_H
