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

#ifndef MIRRA_DEFINE_H_
#define MIRRA_DEFINE_H_

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace mirra {

	#define __in
	#define __in_opt
	#define __inout
	#define __inout_opt
	#define __out
	#define __out_opt

	#define MIRRA "Mirra"
	#define MIRRA_MILESTONE "alpha"
	#define MIRRA_VERSION_MAJOR 0
	#define MIRRA_VERSION_MINOR 1
	#define MIRRA_VERSION_REVISION 1
	#define MIRRA_VERSION_WORKWEEK 1629

	#define STRING_EMPTY "EMPTY"
	#define STRING_UNKNOWN "UNKNOWN"

	#define SCALAR_AS_HEX(_TYPE_, _VAL_) \
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex \
			<< (uintmax_t) ((_TYPE_) (_VAL_)) << std::dec

	#define SCALAR_INVALID(_TYPE_) ((_TYPE_) -1)

	#define STRING_CHECK(_STR_) \
		(_STR_.empty() ? STRING_EMPTY : _STR_.c_str())
}

#endif // MIRRA_DEFINE_H_
