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

#ifndef MIRRA_DISPLAY_TYPE_H_
#define MIRRA_DISPLAY_TYPE_H_

#include "../include/mirra_exception.h"

namespace mirra {

	#define MIRRA_DISPLAY_HEADER "[MIRRA::DISPLAY]"

#ifndef NDEBUG
	#define MIRRA_DISPLAY_EXCEPTION_HEADER MIRRA_DISPLAY_HEADER " "
#else
	#define MIRRA_DISPLAY_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		MIRRA_DISPLAY_EXCEPTION_EXTERNAL = 0,
		MIRRA_DISPLAY_EXCEPTION_INITIALIZED,
		MIRRA_DISPLAY_EXCEPTION_INVALID_COORDINATE,
		MIRRA_DISPLAY_EXCEPTION_INVALID_PARAMETER,
		MIRAR_DISPLAY_EXCEPTION_STARTED,
		MIRRA_DISPLAY_EXCEPTION_STOPPED,
		MIRRA_DISPLAY_EXCEPTION_UNINITIALIZED,
	};

	#define MIRRA_DISPLAY_EXCEPTION_MAX MIRRA_DISPLAY_EXCEPTION_UNINITIALIZED

	static const std::string MIRRA_DISPLAY_EXCEPTION_STR[] = {
		MIRRA_DISPLAY_EXCEPTION_HEADER "External exception",
		MIRRA_DISPLAY_EXCEPTION_HEADER "Display is initialized",
		MIRRA_DISPLAY_EXCEPTION_HEADER "Invalid pixel coordinate",
		MIRRA_DISPLAY_EXCEPTION_HEADER "Invalid parameter type",
		MIRRA_DISPLAY_EXCEPTION_HEADER "Display is started",
		MIRRA_DISPLAY_EXCEPTION_HEADER "Display is stopped",
		MIRRA_DISPLAY_EXCEPTION_HEADER "Display is uninitialized",
		};

	#define MIRRA_DISPLAY_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > MIRRA_DISPLAY_EXCEPTION_MAX ? MIRRA_DISPLAY_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
		STRING_CHECK(MIRRA_DISPLAY_EXCEPTION_STR[_TYPE_]))

	#define THROW_MIRRA_DISPLAY_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(MIRRA_DISPLAY_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
}

#endif // MIRRA_DISPLAY_TYPE_H_
