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

#ifndef MIRRA_CPU_TYPE_H_
#define MIRRA_CPU_TYPE_H_

#include "../include/mirra_exception.h"

namespace mirra {

	#define MIRRA_CPU_HEADER "[MIRRA::CPU]"

#ifndef NDEBUG
	#define MIRRA_CPU_EXCEPTION_HEADER MIRRA_CPU_HEADER " "
#else
	#define MIRRA_CPU_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		MIRRA_CPU_EXCEPTION_INITIALIZED = 0,
		MIRRA_CPU_EXCEPTION_INVALID_INDIRECT,
		MIRRA_CPU_EXCEPTION_INVALID_INSTRUCTION,
		MIRRA_CPU_EXCEPTION_STARTED,
		MIRRA_CPU_EXCEPTION_STOPPED,
		MIRRA_CPU_EXCEPTION_UNINITIALIZED,
	};

	#define MIRRA_CPU_EXCEPTION_MAX MIRRA_CPU_EXCEPTION_UNINITIALIZED

	static std::string MIRRA_CPU_EXCEPTION_STR[] = {
		MIRRA_CPU_EXCEPTION_HEADER "CPU is initialized",
		MIRRA_CPU_EXCEPTION_HEADER "Invalid indirect addressing mode",
		MIRRA_CPU_EXCEPTION_HEADER "Invalid instruction",
		MIRRA_CPU_EXCEPTION_HEADER "CPU is started",
		MIRRA_CPU_EXCEPTION_HEADER "CPU is stopped",
		MIRRA_CPU_EXCEPTION_HEADER "CPU is uninitialized",
		};

	#define MIRRA_CPU_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > MIRRA_CPU_EXCEPTION_MAX ? MIRRA_CPU_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
		STRING_CHECK(MIRRA_CPU_EXCEPTION_STR[_TYPE_]))

	#define THROW_MIRRA_CPU_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(MIRRA_CPU_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_MIRRA_CPU_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(MIRRA_CPU_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
}

#endif // MIRRA_CPU_TYPE_H_
