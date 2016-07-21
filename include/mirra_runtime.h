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

#ifndef MIRRA_RUNTIME_H_
#define MIRRA_RUNTIME_H_

#include <thread>
#include "mirra_exception.h"
#include "mirra_signal.h"
#include "../include/mirra_display.h"
#include "../include/mirra_input.h"

namespace mirra {

	std::string version(
		__in_opt bool verbose = false
		);

	class runtime :
			public mirra::singleton<mirra::runtime> {

		public:

			~runtime(void);

			void initialize(
				__in_opt const mirra::parameter_t &parameter = mirra::parameter_t()
				);

			bool is_initialized(void);

			bool is_started(void);

			void start(
				__in_opt const mirra::parameter_t &parameter = mirra::parameter_t()
				);

			void stop(void);

			std::string to_string(
				__in_opt bool verbose = false
				);

			void uninitialize(void);

			void wait(
				__in uint32_t timeout = SIGNAL_TIMEOUT_UNDEFINED
				);

		protected:

			friend class mirra::singleton<mirra::runtime>;

			runtime(void);

			static void run(
				__in mirra::runtime &context
				);

			void set(
				__in bool started
				);

			bool m_initialized;

			std::mutex m_mutex;

			mirra::parameter_t m_parameter_initialize;

			mirra::parameter_t m_parameter_start;

			mirra::signal m_signal_start;

			mirra::signal m_signal_wait;

			bool m_started;

			std::thread m_thread;
	};
}

#endif // MIRRA_RUNTIME_H_
