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

#include <functional>
#include "../include/mirra_runtime.h"
#include "../include/mirra_cpu.h"
#include "mirra_runtime_type.h"

namespace mirra {

	#define RUNTIME_INIT_FLAGS (SDL_INIT_AUDIO | SDL_INIT_VIDEO)
	#define RUNTIME_START_TIMEOUT 5000

	std::string 
	version(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		if(verbose) {
			result << MIRRA << " ";
		}

		result << MIRRA_VERSION_MAJOR << "." << MIRRA_VERSION_MINOR 
			<< "." << MIRRA_VERSION_WORKWEEK << "." << MIRRA_VERSION_REVISION 
			<< "-" << MIRRA_MILESTONE;

		return result.str();
	}

	runtime::runtime(void) :
		mirra::singleton<mirra::runtime>(OBJECT_RUNTIME),
		m_initialized(false),
		m_started(false)
	{
		return;
	}

	runtime::~runtime(void)
	{
		uninitialize();
	}

	void 
	runtime::initialize(
		__in_opt const mirra::parameter_t &parameter
		)
	{

		if(m_initialized) {
			THROW_MIRRA_RUNTIME_EXCEPTION(MIRRA_RUNTIME_EXCEPTION_INITIALIZED);
		}

		m_parameter_initialize = parameter;
		mirra::display::acquire().initialize(m_parameter_initialize);
		mirra::input::acquire().initialize(m_parameter_initialize);

		// TODO: initialize sigletons

		m_initialized = true;
	}

	bool 
	runtime::is_initialized(void)
	{
		return m_initialized;
	}

	bool 
	runtime::is_started(void)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_started;
	}

	void 
	runtime::run(
		__in mirra::runtime &context
		)
	{
		SDL_Event event;
		mirra::input &input = mirra::input::acquire();
		mirra::display &display = mirra::display::acquire();

		if(SDL_Init(RUNTIME_INIT_FLAGS)) {
			THROW_MIRRA_RUNTIME_EXCEPTION_FORMAT(MIRRA_RUNTIME_EXCEPTION_EXTERNAL,
				"SDL_Init failed: %s", SDL_GetError());
		}

		if(context.m_signal_start.is_notifiable()) {
			context.m_signal_start.notify();
		}

		display.start(context.m_parameter_start);
		input.start(context.m_parameter_start);

		// TODO: start singletons

		while(context.is_started()) {

			while(SDL_PollEvent(&event)) {

				switch(event.type) {
					case SDL_KEYDOWN:
					case SDL_KEYUP:
						input.process(event.key);
						break;
					case SDL_QUIT:
						context.set(false);
						break;
					default:
						break;
				}
			}

			// TODO: update singletons
		}

		// TODO: stop singletons

		input.stop();
		display.stop();
		SDL_Quit();

		if(context.m_signal_wait.is_notifiable()) {
			context.m_signal_wait.notify();
		}
	}

	void 
	runtime::set(
		__in bool started
		)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_started = started;
	}

	void 
	runtime::start(
		__in_opt const mirra::parameter_t &parameter
		)
	{

		if(!m_initialized) {
			THROW_MIRRA_RUNTIME_EXCEPTION(MIRRA_RUNTIME_EXCEPTION_UNINITIALIZED);
		}

		if(is_started()) {
			THROW_MIRRA_RUNTIME_EXCEPTION(MIRRA_RUNTIME_EXCEPTION_STARTED);
		}

		m_parameter_start = parameter;
		set(true);
		m_thread = std::thread(mirra::runtime::run, std::ref(*this));

		if(SIGNAL_TIMEOUT == m_signal_start.wait(RUNTIME_START_TIMEOUT)) {
			THROW_MIRRA_RUNTIME_EXCEPTION(MIRRA_RUNTIME_EXCEPTION_TIMEOUT);
		}

		m_thread.detach();
	}

	void 
	runtime::stop(void)
	{

		if(m_initialized && is_started()) {
			set(false);

			if(m_thread.joinable()) {
				m_thread.join();
			}
			
			m_parameter_start.clear();
		}
	}

	std::string 
	runtime::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << mirra::object::as_string(*this, verbose)
			<< " (" << (m_initialized ? "INIT" : "UNINIT")
			<< ", " << (is_started() ? "START" : "STOP") << ")";

		if(m_initialized) {
			result << " INST=" << SCALAR_AS_HEX(uintptr_t, this);

			if(is_started()) {
				result << ", TID=" << m_thread.get_id();
			}
		}

		return result.str();
	}

	void 
	runtime::uninitialize(void)
	{

		if(m_initialized) {
			stop();
			m_initialized = false;

			// TODO: uninitialize singletons

			mirra::input::acquire().uninitialize();
			mirra::display::acquire().uninitialize();
			m_parameter_initialize.clear();
		}
	}

	void 
	runtime::wait(
		__in uint32_t timeout
		)
	{

		if(!m_initialized) {
			THROW_MIRRA_RUNTIME_EXCEPTION(MIRRA_RUNTIME_EXCEPTION_UNINITIALIZED);
		}

		if(!is_started()) {
			THROW_MIRRA_RUNTIME_EXCEPTION(MIRRA_RUNTIME_EXCEPTION_STOPPED);
		}

		if(SIGNAL_TIMEOUT == m_signal_wait.wait(timeout)) {
			stop();
		}
	}
}
