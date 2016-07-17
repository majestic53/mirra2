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

#ifndef MIRRA_SIGNAL_H_
#define MIRRA_SIGNAL_H_

#include <condition_variable>
#include <mutex>
#include "mirra_define.h"

namespace mirra {

	typedef enum {
		SIGNAL_ABANDON = 0,
		SIGNAL_CLEAR,
		SIGNAL_NOTIFY,
		SIGNAL_TIMEOUT,
	} signal_t;

	#define SIGNAL_MAX SIGNAL_TIMEOUT

	#define SIGNAL_TIMEOUT_UNDEFINED SCALAR_INVALID(uint32_t)

	class signal {

		public:

			signal(
				__in_opt bool notified = false,
				__in_opt bool resetting = true
				);

			signal(
				__in const signal &other
				);

			virtual ~signal(void);

			signal &operator=(
				__in const signal &other
				);

			static std::string as_string(
				__in const signal &reference,
				__in_opt bool verbose = false
				);

			void clear(void);

			bool is_notifiable(void);

			void notify(void);

			mirra::signal_t state(void);

			std::string to_string(
				__in_opt bool verbose = false
				);

			mirra::signal_t wait(
				__in_opt uint32_t timeout = SIGNAL_TIMEOUT_UNDEFINED
				);

		protected:

			void abandon(void);

			void set(
				__in mirra::signal_t state
				);

			std::condition_variable m_condition;

			std::mutex m_mutex;

			bool m_resetting;

			mirra::signal_t m_state;

	};
}

#endif // MIRRA_SIGNAL_H_
