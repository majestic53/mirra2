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

#include "../include/mirra_input.h"
#include "mirra_input_type.h"

namespace mirra {

	static const std::string BUTTON_STR[] = {
		"A", "B", "SELECT", "START", "UP", "DOWN", "LEFT", "RIGHT",
		};

	#define BUTTON_STRING(_TYPE_) \
		((_TYPE_) > BUTTON_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(BUTTON_STR[_TYPE_]))

	static const std::string JOYPAD_STR[] = {
		"JOYPAD_0", "JOYPAD_1",
		};

	#define JOYPAD_STRING(_TYPE_) \
		((_TYPE_) > JOYPAD_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(JOYPAD_STR[_TYPE_]))

	static const std::string INPUT_PARAMETER_STR[] = {
		"JOYPAD_0_A", "JOYPAD_0_B", "JOYPAD_0_SELECT", "JOYPAD_0_START",
		"JOYPAD_0_UP", "JOYPAD_0_DOWN", "JOYPAD_0_LEFT", "JOYPAD_0_RIGHT",
		"JOYPAD_1_A", "JOYPAD_1_B", "JOYPAD_1_SELECT", "JOYPAD_1_START",
		"JOYPAD_1_UP", "JOYPAD_1_DOWN", "JOYPAD_1_LEFT", "JOYPAD_1_RIGHT",
		};

	#define INPUT_PARAMETER_STRING(_TYPE_) \
		((_TYPE_) > INPUT_PARAMETER_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(INPUT_PARAMETER_STR[_TYPE_]))

	static const SDL_Scancode INPUT_PARAMETER_DEF[] = {
		SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_RETURN, SDL_SCANCODE_SPACE,
		SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
		SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,
		SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,
		};

	#define INPUT_PARAMETER_DEFAULT(_TYPE_) \
		((_TYPE_) > INPUT_PARAMETER_MAX ? SDL_SCANCODE_UNKNOWN : \
		INPUT_PARAMETER_DEF[_TYPE_])

	input::input(void) :
		mirra::singleton<mirra::input>(OBJECT_INPUT),
		m_initialized(false),
		m_started(false)
	{
		return;
	}

	input::~input(void)
	{
		uninitialize();
	}

	void input::clear(void)
	{
		m_button.clear();
		m_button_position.clear();
		m_button_state.clear();
	}

	SDL_Scancode 
	input::find_button(
		__in mirra::joypad_t joypad,
		__in mirra::button_t button
		)
	{
		std::map<mirra::button_t, SDL_Scancode>::iterator button_iter;
		std::map<mirra::joypad_t, std::map<mirra::button_t, SDL_Scancode>>::iterator joypad_iter;

		joypad_iter = m_button.find(joypad);
		if(joypad_iter == m_button.end()) {
			THROW_MIRRA_INPUT_EXCEPTION_FORMAT(MIRRA_INPUT_EXCEPTION_NOT_FOUND,
				"%s (%x)", JOYPAD_STRING(joypad), joypad);
		}

		button_iter = joypad_iter->second.find(button);
		if(button_iter == joypad_iter->second.end()) {
			THROW_MIRRA_INPUT_EXCEPTION_FORMAT(MIRRA_INPUT_EXCEPTION_INVALID_BUTTON,
				"%s (%x)", BUTTON_STRING(button), button);
		}

		return button_iter->second;
	}

	mirra::button_t &
	input::find_button_position(
		__in mirra::joypad_t joypad
		)
	{
		std::map<mirra::joypad_t, mirra::button_t>::iterator iter;

		iter = m_button_position.find(joypad);
		if(iter == m_button_position.end()) {
			THROW_MIRRA_INPUT_EXCEPTION_FORMAT(MIRRA_INPUT_EXCEPTION_NOT_FOUND,
				"%s (%x)", JOYPAD_STRING(joypad), joypad);
		}

		return iter->second;
	}

	bool 
	input::find_button_state(
		__in mirra::joypad_t joypad,
		__in mirra::button_t button
		)
	{
		std::map<SDL_Scancode, bool>::iterator iter;

		iter = m_button_state.find(find_button(joypad, button));
		if(iter == m_button_state.end()) {
			THROW_MIRRA_INPUT_EXCEPTION_FORMAT(MIRRA_INPUT_EXCEPTION_NOT_FOUND,
				"%s (%x), %s (%x)", JOYPAD_STRING(joypad), joypad, 
				BUTTON_STRING(button), button);
		}

		return iter->second;
	}

	void 
	input::initialize(
		__in_opt const mirra::parameter_t &parameter
		)
	{

		if(m_initialized) {
			THROW_MIRRA_INPUT_EXCEPTION(MIRRA_INPUT_EXCEPTION_INITIALIZED);
		}

		m_initialized = true;
	}

	bool 
	input::is_initialized(void)
	{
		return m_initialized;
	}

	bool 
	input::is_pressed(
		__in mirra::joypad_t joypad,
		__in mirra::button_t button
		)
	{
		if(!m_initialized) {
			THROW_MIRRA_INPUT_EXCEPTION(MIRRA_INPUT_EXCEPTION_UNINITIALIZED);
		}

		if(!m_started) {
			THROW_MIRRA_INPUT_EXCEPTION(MIRRA_INPUT_EXCEPTION_STOPPED);
		}

		return find_button_state(joypad, button);
	}

	bool 
	input::is_started(void)
	{
		return m_started;
	}

	bool 
	input::poll(
		__in mirra::joypad_t joypad
		)
	{
		return find_button_state(joypad, find_button_position(joypad));
	}

	void 
	input::process(
		__in const SDL_KeyboardEvent &event
		)
	{
		std::map<SDL_Scancode, bool>::iterator iter;

		if(!m_initialized) {
			THROW_MIRRA_INPUT_EXCEPTION(MIRRA_INPUT_EXCEPTION_UNINITIALIZED);
		}

		if(!m_started) {
			THROW_MIRRA_INPUT_EXCEPTION(MIRRA_INPUT_EXCEPTION_STOPPED);
		}

		if(!event.repeat) {

			iter = m_button_state.find(event.keysym.scancode);
			if(iter != m_button_state.end()) {
				iter->second = (event.state == SDL_PRESSED);
			}
		}
	}

	void 
	input::reset(void)
	{
		std::map<SDL_Scancode, bool>::iterator state_iter;
		std::map<mirra::joypad_t, mirra::button_t>::iterator position_iter;

		if(!m_initialized) {
			THROW_MIRRA_INPUT_EXCEPTION(MIRRA_INPUT_EXCEPTION_UNINITIALIZED);
		}

		if(!m_started) {
			THROW_MIRRA_INPUT_EXCEPTION(MIRRA_INPUT_EXCEPTION_STOPPED);
		}

		for(position_iter = m_button_position.begin(); position_iter != m_button_position.end(); ++position_iter) {
			position_iter->second = BUTTON_A;
		}

		for(state_iter = m_button_state.begin(); state_iter != m_button_state.end(); ++state_iter) {
			state_iter->second = false;
		}
	}

	void 
	input::start(
		__in_opt const mirra::parameter_t &parameter
		)
	{
		
		mirra::parameter_t::const_iterator iter;
		size_t button_base, button_iter, joypad_iter;		
		mirra::object_parameter_t::const_iterator attribute_iter;
		std::map<SDL_Scancode, bool>::iterator button_state_iter;
		std::map<mirra::button_t, SDL_Scancode>::iterator button_scancode_iter;
		std::map<mirra::joypad_t, std::map<mirra::button_t, SDL_Scancode>>::iterator button_code_iter;

		if(!m_initialized) {
			THROW_MIRRA_INPUT_EXCEPTION(MIRRA_INPUT_EXCEPTION_UNINITIALIZED);
		}

		if(m_started) {
			THROW_MIRRA_INPUT_EXCEPTION(MIRRA_INPUT_EXCEPTION_STARTED);
		}

		for(joypad_iter = JOYPAD_0; joypad_iter <= JOYPAD_MAX; ++joypad_iter) {

			if(joypad_iter == JOYPAD_1) {
				button_base = INPUT_PARAMETER_JOYPAD_1_A;
			} else {
				button_base = INPUT_PARAMETER_JOYPAD_0_A;
			}

			m_button.insert(std::pair<mirra::joypad_t, std::map<mirra::button_t, SDL_Scancode>>(
				(mirra::joypad_t) joypad_iter, std::map<mirra::button_t, SDL_Scancode>()));

			button_code_iter = m_button.find((mirra::joypad_t) joypad_iter);
			if(button_code_iter == m_button.end()) {
				THROW_MIRRA_INPUT_EXCEPTION_FORMAT(MIRRA_INPUT_EXCEPTION_NOT_FOUND,
					"%s (%x)", JOYPAD_STRING(joypad_iter), joypad_iter);
			}

			for(button_iter = button_base; button_iter <= (button_base + BUTTON_MAX); ++button_iter) {
				button_code_iter->second.insert(std::pair<mirra::button_t, SDL_Scancode>(
					(mirra::button_t) (button_iter % (BUTTON_MAX + 1)), INPUT_PARAMETER_DEFAULT(button_iter)));
				m_button_state.insert(std::pair<SDL_Scancode, bool>(INPUT_PARAMETER_DEFAULT(button_iter), 
					false));
			}

			m_button_position.insert(std::pair<mirra::joypad_t, mirra::button_t>((mirra::joypad_t) joypad_iter,
				BUTTON_A));
		}

		iter = parameter.find(OBJECT_INPUT);
		if(iter != parameter.end()) {

			for(joypad_iter = JOYPAD_0; joypad_iter <= JOYPAD_MAX; ++joypad_iter) {

				if(joypad_iter == JOYPAD_1) {
					button_base = INPUT_PARAMETER_JOYPAD_1_A;
				} else {
					button_base = INPUT_PARAMETER_JOYPAD_0_A;
				}

				button_code_iter = m_button.find((mirra::joypad_t) joypad_iter);
				if(button_code_iter == m_button.end()) {
					THROW_MIRRA_INPUT_EXCEPTION_FORMAT(MIRRA_INPUT_EXCEPTION_NOT_FOUND,
						"%s (%x)", JOYPAD_STRING(joypad_iter), joypad_iter);
				}

				for(button_iter = button_base; button_iter <= (button_base + BUTTON_MAX); ++button_iter) {

					attribute_iter = iter->second.find(button_iter);
					if(attribute_iter != iter->second.end()) {

						if(attribute_iter->second.type != DATA_UNSIGNED) {
							THROW_MIRRA_INPUT_EXCEPTION_FORMAT(MIRRA_INPUT_EXCEPTION_INVALID_PARAMETER,
								"%s: %s (expecting %s)", INPUT_PARAMETER_STRING(button_iter),
								DATA_STRING(attribute_iter->second.type), DATA_STRING(DATA_UNSIGNED));
						}

						button_scancode_iter = button_code_iter->second.find(
							(mirra::button_t) (button_iter % (BUTTON_MAX + 1)));

						if(button_scancode_iter != button_code_iter->second.end()) {

							button_state_iter = m_button_state.find(button_scancode_iter->second);
							if(button_state_iter != m_button_state.end()) {
								m_button_state.erase(button_state_iter);
							}

							button_scancode_iter->second = (SDL_Scancode) attribute_iter->second.data.uvalue;
							m_button_state.insert(std::pair<SDL_Scancode, bool>(button_scancode_iter->second, 
								false));
						}
					}
				}
			}
		}

		m_started = true;
	}

	void 
	input::stop(void)
	{

		if(!m_initialized) {
			THROW_MIRRA_INPUT_EXCEPTION(MIRRA_INPUT_EXCEPTION_UNINITIALIZED);
		}

		if(m_started) {
			reset();
			m_started = false;
		}
	}

	void 
	input::strobe(
		__in mirra::joypad_t joypad
		)
	{
		find_button_position(joypad) = BUTTON_A;
	}

	std::string 
	input::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;
		std::map<SDL_Scancode, bool>::iterator state_iter;
		std::map<mirra::button_t, SDL_Scancode>::iterator button_iter;
		std::map<mirra::joypad_t, mirra::button_t>::iterator position_iter;
		std::map<mirra::joypad_t, std::map<mirra::button_t, SDL_Scancode>>::iterator joypad_iter;

		result << mirra::object::as_string(*this, verbose)
			<< " (" << (m_initialized ? "INIT" : "UNINIT")
			<< ", " << (m_started ? "START" : "STOP") << ")";

		if(m_initialized) {
			result << " INST=" << SCALAR_AS_HEX(uintptr_t, this);

			if(m_started) {

				for(joypad_iter = m_button.begin(); joypad_iter != m_button.end(); ++joypad_iter) {
					result << ", " << JOYPAD_STRING(joypad_iter->first) << "[";

					position_iter = m_button_position.find(joypad_iter->first);
					if(position_iter != m_button_position.end()) {
						result << BUTTON_STRING(position_iter->second);
					} else {
						result << STRING_UNKNOWN;
					}

					result << "]={";

					for(button_iter = joypad_iter->second.begin(); button_iter != joypad_iter->second.end(); 
							++button_iter) {

						if(button_iter != joypad_iter->second.begin()) {
							result << ", ";
						}

						result << BUTTON_STRING(button_iter->first) 
							<< "[" << SCALAR_AS_HEX(SDL_Scancode, button_iter->second) << "]:";

						state_iter = m_button_state.find(button_iter->second);
						if(state_iter != m_button_state.end()) {
							result << (state_iter->second ? "PRE" : "REL");
						} else {
							result << STRING_UNKNOWN;
						}
					}

					result << "}";
				}
			}
		}

		return result.str();
	}

	void 
	input::uninitialize(void)
	{

		if(m_initialized) {
			stop();
			clear();
			m_initialized = false;
		}
	}
}
