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

#ifndef MIRRA_RUNTIME_TYPE_H_
#define MIRRA_RUNTIME_TYPE_H_

#include "../include/mirra_exception.h"

namespace mirra {

	#define MIRRA_RUNTIME_HEADER "[MIRRA::RUNTIME]"

#ifndef NDEBUG
	#define MIRRA_RUNTIME_EXCEPTION_HEADER MIRRA_RUNTIME_HEADER " "
#else
	#define MIRRA_RUNTIME_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		MIRRA_RUNTIME_EXCEPTION_EXTERNAL = 0,
		MIRRA_RUNTIME_EXCEPTION_INITIALIZED,
		MIRRA_RUNTIME_EXCEPTION_STARTED,
		MIRRA_RUNTIME_EXCEPTION_STOPPED,
		MIRRA_RUNTIME_EXCEPTION_TIMEOUT,
		MIRRA_RUNTIME_EXCEPTION_UNINITIALIZED,
	};

	#define MIRRA_RUNTIME_EXCEPTION_MAX MIRRA_RUNTIME_EXCEPTION_UNINITIALIZED

	static const std::string MIRRA_RUNTIME_EXCEPTION_STR[] = {
		MIRRA_RUNTIME_EXCEPTION_HEADER "External exception",
		MIRRA_RUNTIME_EXCEPTION_HEADER "Runtime is initialized",
		MIRRA_RUNTIME_EXCEPTION_HEADER "Runtime is started",
		MIRRA_RUNTIME_EXCEPTION_HEADER "Runtime is stopped",
		MIRRA_RUNTIME_EXCEPTION_HEADER "Failed to start runtime",
		MIRRA_RUNTIME_EXCEPTION_HEADER "Runtime is uninitialized",
		};

	#define MIRRA_RUNTIME_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > MIRRA_RUNTIME_EXCEPTION_MAX ? MIRRA_RUNTIME_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
		STRING_CHECK(MIRRA_RUNTIME_EXCEPTION_STR[_TYPE_]))

	#define THROW_MIRRA_RUNTIME_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(MIRRA_RUNTIME_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_MIRRA_RUNTIME_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(MIRRA_RUNTIME_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
}

#endif // MIRRA_RUNTIME_TYPE_H_
