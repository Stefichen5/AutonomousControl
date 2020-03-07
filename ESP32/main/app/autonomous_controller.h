//
// Created by stefan on 06.03.20.
//

#ifndef ESP_AUTONOMOUS_AUTONOMOUS_CONTROLLER_H
#define ESP_AUTONOMOUS_AUTONOMOUS_CONTROLLER_H


#include "../hw_interface/uart_device.h"

class autonomous_controller {
public:
	autonomous_controller(uart_device * uart);
	~autonomous_controller() = default;

	typedef __uint8_t t_button;

	/**
	 * @brief send "go up" command
	 * @return success
	 */
	bool go_up();

	/**
	* @brief send "go down" command
	* @return success
	*/
	bool go_down();

	/**
	 * @brief go to a specific height step
	 * @param height
	 * @return success
	 */
	bool go_to_height(__uint8_t height);

	/**
	 * @brief go to a specific preset height
	 * @param preset height (button 1-4 on original controller)
	 * @return success
	 */
	bool go_to_preset(t_button preset);

	/**
	 * @brief save current height as a preset
	 * @param preset button nr. to save height to (1-4)
	 * @return success
	 */
	bool save_height_as_preset(t_button preset);

	/**
	 * @brief send one ore more button presses
	 * @param button_combination logical or combination of buttons pressed
	 * @return success
	 */
	bool send_button(t_button button_combination);

	/**
	 * @brief get current height
	 * @return height (raw)
	 */
	__uint8_t get_height_raw();

	/**
	 * @brief get current height
	 * @return height (in steps, 1 is lowest)
	 */
	__uint8_t get_height();

	t_button const button_down = 0x01;
	t_button const	button_up = 0x02;
	t_button const	button_1 = 0x04;
	t_button const	button_2 = 0x08;
	t_button const	button_3 = 0x10;
	t_button const	button_4 = 0x20;
	t_button const	button_M = 0x40;
private:
	uart_device * device = nullptr;

	int const controller_baudrate = 9600;
	uart_stop_bits_t const controller_stop_bits = UART_STOP_BITS_1;
	uart_word_length_t const controller_word_length = UART_DATA_8_BITS;

	__uint8_t const rec_header_first = 0x98;
	__uint8_t const rec_header_second = 0x00;

	__uint8_t const snd_header_first = 0xD8;
	__uint8_t const snd_header_second = 0x66;

	#define REC_MSG_LEN 6
	#define SND_MSG_LEN 5

	__uint8_t const snd_msg_empty[SND_MSG_LEN] = {snd_header_first, snd_header_first, snd_header_second, 0x00, 0x00};

	__uint8_t const height_min = 0x4B;
	__uint8_t const height_max = 0x7B;

	__uint8_t get_height_from_buffer(__uint8_t * buf, __uint32_t  buf_size) const;
	bool is_valid_receive_header_first_stage(__uint8_t * first_byte) const;
	bool is_valid_receive_header_second_stage(__uint8_t * first_byte) const;
	bool is_height_value_in_boundaries(__uint8_t val) const;
	void fill_send_message_template(__uint8_t * buf, t_button buttons);

	bool send_message(__uint8_t * msg);
};


#endif //ESP_AUTONOMOUS_AUTONOMOUS_CONTROLLER_H
