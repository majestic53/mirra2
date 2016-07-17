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

#ifndef MIRRA_BUS_H_
#define MIRRA_BUS_H_

#include <climits>
#include "mirra_define.h"

namespace mirra {

	#define BUS_WORD_MAX UINT16_MAX
	#define BUS_WORD_WIDTH (sizeof(uint16_t) * CHAR_BIT)

	class bus {

		public:

			virtual uint8_t read(
				__in uint16_t address
				) = 0;

			virtual void update(void) = 0;

			virtual void write(
				__in uint16_t address,
				__in uint8_t value
				) = 0;

	};
}

#endif // MIRRA_BUS_H_
