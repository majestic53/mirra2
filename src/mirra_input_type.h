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

#ifndef MIRRA_INPUT_TYPE_H_
#define MIRRA_INPUT_TYPE_H_

#include "../include/mirra_exception.h"

namespace mirra {

	#define MIRRA_INPUT_HEADER "[MIRRA:INPUT]"

#ifndef NDEBUG
	#define MIRRA_INPUT_EXCEPTION_HEADER MIRRA_INPUT_HEADER " "
#else
	#define MIRRA_INPUT_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		
		MIRRA_INPUT_EXCEPTION_INITIALIZED = 0,
		MIRRA_INPUT_EXCEPTION_INVALID_BUTTON,
		MIRRA_INPUT_EXCEPTION_INVALID_PARAMETER,
		MIRRA_INPUT_EXCEPTION_NOT_FOUND,
		MIRRA_INPUT_EXCEPTION_STARTED,
		MIRRA_INPUT_EXCEPTION_STOPPED,
		MIRRA_INPUT_EXCEPTION_UNINITIALIZED,
	};

	#define MIRRA_INPUT_EXCEPTION_MAX MIRRA_INPUT_EXCEPTION_UNINITIALIZED

	static const std::string MIRRA_INPUT_EXCEPTION_STR[] = {
		MIRRA_INPUT_EXCEPTION_HEADER "Input is initialized",
		MIRRA_INPUT_EXCEPTION_HEADER "Invalid button",
		MIRRA_INPUT_EXCEPTION_HEADER "Invalid parameter type",
		MIRRA_INPUT_EXCEPTION_HEADER "Input does not exist",
		MIRRA_INPUT_EXCEPTION_HEADER "Input is started",
		MIRRA_INPUT_EXCEPTION_HEADER "Input is stopped",
		MIRRA_INPUT_EXCEPTION_HEADER "Input is uninitialized",
		};

	#define MIRAR_INPUT_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > MIRRA_INPUT_EXCEPTION_MAX ? MIRRA_INPUT_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
		STRING_CHECK(MIRRA_INPUT_EXCEPTION_STR[_TYPE_]))

	#define THROW_MIRRA_INPUT_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(MIRAR_INPUT_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_MIRRA_INPUT_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(MIRAR_INPUT_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
}

#endif // MIRRA_INPUT_TYPE_H_
