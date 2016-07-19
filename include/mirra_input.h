/**
 * mirra
 * Copyright (C) 2016 David Jolly
 * ----------------------
 *
 * mirra is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mirra is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MIRRA_INPUT_H_
#define MIRRA_INPUT_H_

#include <SDL2/SDL.h>
#include "mirra_singleton.h"

namespace mirra {

	typedef enum {
		BUTTON_A = 0,
		BUTTON_B,
		BUTTON_SELECT,
		BUTTON_START,
		BUTTON_UP,
		BUTTON_DOWN,
		BUTTON_LEFT,
		BUTTON_RIGHT,
	} button_t;

	#define BUTTON_MAX BUTTON_RIGHT

	typedef enum {
		JOYPAD_0 = 0,
		JOYPAD_1,
	} joypad_t;

	#define JOYPAD_MAX JOYPAD_1

	enum {
		INPUT_PARAMETER_JOYPAD_0_A = 0,
		INPUT_PARAMETER_JOYPAD_0_B,
		INPUT_PARAMETER_JOYPAD_0_SELECT,
		INPUT_PARAMETER_JOYPAD_0_START,
		INPUT_PARAMETER_JOYPAD_0_UP,
		INPUT_PARAMETER_JOYPAD_0_DOWN,
		INPUT_PARAMETER_JOYPAD_0_LEFT,
		INPUT_PARAMETER_JOYPAD_0_RIGHT,
		INPUT_PARAMETER_JOYPAD_1_A,
		INPUT_PARAMETER_JOYPAD_1_B,
		INPUT_PARAMETER_JOYPAD_1_SELECT,
		INPUT_PARAMETER_JOYPAD_1_START,
		INPUT_PARAMETER_JOYPAD_1_UP,
		INPUT_PARAMETER_JOYPAD_1_DOWN,
		INPUT_PARAMETER_JOYPAD_1_LEFT,
		INPUT_PARAMETER_JOYPAD_1_RIGHT,
	};

	#define INPUT_PARAMETER_MAX INPUT_PARAMETER_JOYPAD_1_RIGHT

	class input :
			public mirra::singleton<mirra::input> {

		public:

			~input(void);

			void initialize(
				__in_opt const mirra::parameter_t &parameter = mirra::parameter_t()
				);

			bool is_initialized(void);

			bool is_pressed(
				__in mirra::joypad_t joypad,
				__in mirra::button_t button
				);

			bool is_started(void);

			bool poll(
				__in mirra::joypad_t joypad
				);

			void process(
				__in const SDL_KeyboardEvent &event
				);

			void reset(void);

			void start(
				__in_opt const mirra::parameter_t &parameter = mirra::parameter_t()
				);

			void stop(void);

			void strobe(
				__in mirra::joypad_t joypad
				);

			std::string to_string(
				__in_opt bool verbose = false
				);

			void uninitialize(void);

		protected:

			friend class mirra::singleton<mirra::input>;

			input(void);

			input(
				__in const input &other
				);

			input &operator=(
				__in const input &other
				);

			void clear(void);

			SDL_Scancode find_button(
				__in mirra::joypad_t joypad,
				__in mirra::button_t button
				);

			mirra::button_t &find_button_position(
				__in mirra::joypad_t joypad
				);

			bool find_button_state(
				__in mirra::joypad_t joypad,
				__in mirra::button_t button
				);

			std::map<mirra::joypad_t, std::map<mirra::button_t, SDL_Scancode>> m_button;

			std::map<mirra::joypad_t, mirra::button_t> m_button_position;

			std::map<SDL_Scancode, bool> m_button_state;

			bool m_initialized;

			bool m_started;

	};
}

#endif // MIRRA_INPUT_H_
