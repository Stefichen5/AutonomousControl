//
// Created by stefan on 06.03.20.
//

#include <cstring>
#include <iostream>
#include "uart_device.h"

uart_device::uart_device(uart_port_t uart_device, int baudrate, int pin_tx, int pin_rx,
		uart_word_length_t databits, uart_parity_t parity,
                         uart_stop_bits_t stop_bit, uart_hw_flowcontrol_t flowctrl, __uint8_t flowctrl_thresh) {
	device = uart_device;

	uart_config_t uart_config;
	bzero(&uart_config, sizeof(uart_config));

	uart_config.baud_rate = baudrate;
	uart_config.data_bits = databits;
	uart_config.parity = parity;
	uart_config.stop_bits = stop_bit;
	uart_config.flow_ctrl = flowctrl;
	uart_config.rx_flow_ctrl_thresh = flowctrl_thresh;

	uart_param_config(device, &uart_config);
	uart_set_pin(device, pin_tx, pin_rx, 18, UART_PIN_NO_CHANGE);
	uart_driver_install(device, BUF_SIZE * 2, 0, 0, NULL, 0);

	uart_set_mode(device, UART_MODE_RS485_HALF_DUPLEX);
}

bool uart_device::send_data(char const *data_to_send, __uint32_t const len) {
	if (data_to_send == nullptr){
		return false;
	}

	uart_write_bytes(device, data_to_send, len);

	//if we don't wait, we will send some crap and nothing works properly
	return uart_wait_tx_done(device, 100) == ESP_OK;
}

uart_device::~uart_device() {
	uart_driver_delete(device);
}

void uart_device::set_baudrate(__uint32_t baudrate) {
	uart_set_baudrate(device, baudrate);
}

__uint32_t uart_device::get_baudrate() const {
	__uint32_t baudrate = 0;

	uart_get_baudrate(device, &baudrate);

	return baudrate;
}

void uart_device::set_word_length(uart_word_length_t wordLength) {
	uart_set_word_length(device, wordLength);
}

void uart_device::set_stop_bits(uart_stop_bits_t stopBits) {
	uart_set_stop_bits(device, stopBits);
}

uart_word_length_t uart_device::get_word_length() const{
	uart_word_length_t wordLength;

	uart_get_word_length(device, &wordLength);

	return wordLength;
}

uart_stop_bits_t uart_device::get_stop_bits() const {
	uart_stop_bits_t stopBits;

	uart_get_stop_bits(device, &stopBits);

	return stopBits;
}

int uart_device::read_data(__uint8_t *data, __uint32_t buf_size, TickType_t wait_time) {
	if (data == nullptr) {return -1;}

	return uart_read_bytes(device, data, buf_size, wait_time);
}

void uart_device::flush() {
	if (uart_flush(device) != ESP_OK){
		std::cerr << "error flushing" << std::endl;
	}
}

size_t uart_device::get_buffered_data_size() const {
	size_t size = 0;

	uart_get_buffered_data_len(device, &size);

	return size;
}

