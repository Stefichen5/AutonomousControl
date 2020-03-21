//
// Created by stefan on 06.03.20.
//

#include <iostream>
#include "autonomous_controller.h"

autonomous_controller::autonomous_controller(uart_device *uart) {
	device = uart;

	if(uart == nullptr){ return;}

	uart->set_baudrate(controller_baudrate);
	uart->set_stop_bits(controller_stop_bits);
	uart->set_word_length(controller_word_length);
}

__uint8_t autonomous_controller::get_height_raw() {
	__uint8_t buf[12] = {0};

	//discard all data read before, we don't need it
	device->flush();
	device->read_data(buf, 12, 80 / portTICK_PERIOD_MS);

	return get_height_from_buffer(buf, 12);
}

__uint8_t autonomous_controller::get_height_from_buffer(__uint8_t *buf, __uint32_t buf_size) const {
	//find pattern 0x98 0x98 0x00 0x00

	__uint32_t pos = 0;
	while (pos < buf_size-1){
		//check if first 2 bytes are 0x98
		if(pos < buf_size-1 && is_valid_receive_header_first_stage(&buf[pos])){
			pos+=2;
			//check if the following 2 bytes are 0x00
			if(pos < buf_size-1 && is_valid_receive_header_second_stage(&buf[pos])){
				pos+=2;

				//check if we still have 2 bytes left to check
				if(pos < buf_size - 1){
					if(buf[pos] == buf[pos+1]){
						//success, we have possible height
						if (is_height_value_in_boundaries(buf[pos])){
							//todo: maybe convert height into something usable?
							return buf[pos];
						} else{
							return 0;
						}
					}
				}
			}
		}

		pos++;
	}

	return 0;
}

bool autonomous_controller::is_valid_receive_header_first_stage(__uint8_t *first_byte) const {
	if (first_byte == nullptr) { return false;}

	//first and second byte need to be the same
	//and they have to be 0x98
	return (*first_byte == rec_header_first) && (*first_byte == *(first_byte+1));
}

bool autonomous_controller::is_valid_receive_header_second_stage(__uint8_t *first_byte) const {
	if (first_byte == nullptr) { return false;}

	//first and second byte need to be the same
	//and they have to be 0x00
	return ((*first_byte == rec_header_second) || (*first_byte == rec_header_second_alt))
			&& (*first_byte == *(first_byte+1));
}

bool autonomous_controller::is_height_value_in_boundaries(__uint8_t val) const {
	return val >= height_min && val <= height_max;
}

__uint8_t autonomous_controller::get_height() {
	__uint8_t height_raw = get_height_raw();

	if (height_raw == 0) { return height_raw;}
	return height_raw-height_min + 1;
}

bool autonomous_controller::send_button(__uint8_t button_combination) {
	__uint8_t msg[SND_MSG_LEN] = {0};

	fill_send_message_template(msg, button_combination);

	return send_message(msg);
}

void autonomous_controller::fill_send_message_template(__uint8_t *buf, t_button buttons) {
	if (buf == nullptr) {return;}

	buf[0] = snd_header_first;
	buf[1] = snd_header_first;
	buf[2] = snd_header_second;
	buf[3] = buttons;
	buf[4] = buttons;
}

bool autonomous_controller::send_message(__uint8_t *msg) {
	bool response = device->send_data((char*)msg, SND_MSG_LEN);

	return response;
}

bool autonomous_controller::go_up() {
	return send_button(button_up);
}

bool autonomous_controller::go_down() {
	return send_button(button_down);
}

bool autonomous_controller::go_to_preset(autonomous_controller::t_button preset) {
	if (preset != button_1 && preset != button_2 && preset != button_3 && preset != button_4){
		std::cout << "invalid button" << std::endl;
		return false;
	}

	//prepare height message
	__uint8_t msg[SND_MSG_LEN] = {0};
	fill_send_message_template(msg, preset);

	//send empty message to start
	send_message((__uint8_t*)snd_msg_empty);

	bool done = false;
	__uint8_t last_height = 0;
	__uint8_t same_height_ctr = 0;

	//loop until height reached
	while (!done){
		vTaskDelay(50 / portTICK_PERIOD_MS);
		__uint8_t cur_height = get_height_raw();
		send_message(msg);
		//if  height does not change X times, we assume that we reached our final height
		if (cur_height == last_height){
			same_height_ctr++;

			if (same_height_ctr >= height_done_threshold){
				done = true;
			}
		} else{
			//height changed, start counting again
			last_height = cur_height;
			same_height_ctr = 0;
		}
	}

	return true;
}

bool autonomous_controller::go_to_height(__uint8_t height) {
	if (height < height_min || height > height_max){
		std::cout << "invalid height" << (__uint32_t)height << "given. has to be between"
		<< (__uint32_t)height_min << " and " << (__uint32_t)height_max << std::endl;
		return false;
	}

	__uint8_t current_height = 0;

	send_message((uint8_t*)snd_msg_empty);

	while (current_height != height){
		__uint8_t invalid_ctr = 0;

		vTaskDelay(50 / portTICK_PERIOD_MS);
		current_height = get_height_raw();

		if (current_height == 0) {
			//invalid reading
			invalid_ctr++;

			//assume some error happened. don't stay in this loop forever
			if(invalid_ctr > 10){
				return false;
			}

			continue;
		} else if (current_height < height){
			go_up();
		} else if (current_height > height) {
			go_down();
		}
	}

	return true;
}

float autonomous_controller::get_height_inch() {
	float height = 0.0;

	//1 = 29.5"
	//2 = 29.9"
	//3 = 30.3"
	//....
	__uint8_t height_step = get_height();

	height = base_height + (step_size * (height_step - 1));

	return height;
}

float autonomous_controller::get_height_cm() {
	return get_height_inch() * 2.54f;
}
