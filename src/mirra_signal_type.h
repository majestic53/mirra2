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

#ifndef MIRRA_SIGNAL_TYPE_H_
#define MIRRA_SIGNAL_TYPE_H_

#include "../include/mirra_exception.h"

namespace mirra {

	#define MIRRA_SIGNAL_HEADER "[MIRRA::SIGNAL]"

#ifndef NDEBUG
	#define MIRRA_SIGNAL_EXCEPTION_HEADER MIRRA_SIGNAL_HEADER " "
#else
	#define MIRRA_SIGNAL_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		MIRRA_SIGNAL_EXCEPTION_ABANDONED = 0,
		MIRRA_SIGNAL_EXCEPTION_BAD_STATE,
		MIRRA_SIGNAL_EXCEPTION_INVALID,
	};

	#define MIRRA_SIGNAL_EXCEPTION_MAX MIRRA_SIGNAL_EXCEPTION_INVALID

	static const std::string MIRRA_SIGNAL_EXCEPTION_STR[] = {
		MIRRA_SIGNAL_EXCEPTION_HEADER "Signal is abandoned",
		MIRRA_SIGNAL_EXCEPTION_HEADER "Signal is in a bad state",
		MIRRA_SIGNAL_EXCEPTION_HEADER "Invalid signal state",
		};

	#define MIRRA_SIGNAL_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > MIRRA_SIGNAL_EXCEPTION_MAX ? MIRRA_SIGNAL_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
		STRING_CHECK(MIRRA_SIGNAL_EXCEPTION_STR[_TYPE_]))

	#define THROW_MIRRA_SIGNAL_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(MIRRA_SIGNAL_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_MIRRA_SIGNAL_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(MIRRA_SIGNAL_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
}

#endif // MIRRA_SIGNAL_TYPE_H_
