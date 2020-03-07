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

	enum e_preset_nr {
		preset_1 = 0x04,
		preset_2 = 0x08,
		preset_3 = 0x10,
		preset_4 = 0x20
	};

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
	 * @brief go up to the next height step
	 * @return success
	 */
	bool go_up_step();

	/**
	 * @brief go down to the next height step
	 * @return success
	 */
	bool go_down_step();

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
	bool go_to_preset(e_preset_nr preset);

	/**
	 * @brief save current height as a preset
	 * @param preset button nr. to save height to
	 * @return success
	 */
	bool save_height_as_preset(e_preset_nr preset);

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
private:
	uart_device * device = nullptr;

	int const controller_baudrate = 9600;
	uart_stop_bits_t const controller_stop_bits = UART_STOP_BITS_1;
	uart_word_length_t const controller_word_length = UART_DATA_8_BITS;

	__uint8_t const rec_header_first = 0x98;
	__uint8_t const rec_header_second = 0x00;

	__uint8_t const height_min = 0x4B;
	__uint8_t const height_max = 0x7B;

	__uint8_t get_height_from_buffer(__uint8_t * buf, __uint32_t  buf_size) const;
	bool is_valid_receive_header_first_stage(__uint8_t * first_byte) const;
	bool is_valid_receive_header_second_stage(__uint8_t * first_byte) const;
	bool is_height_value_in_boundaries(__uint8_t val) const;
};


#endif //ESP_AUTONOMOUS_AUTONOMOUS_CONTROLLER_H
