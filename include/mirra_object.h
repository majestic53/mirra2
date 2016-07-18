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

#ifndef MIRRA_OBJECT_H_
#define MIRRA_OBJECT_H_

#include <map>
#include "mirra_define.h"

namespace mirra {

	typedef enum {
		OBJECT_APU = 0,
		OBJECT_BLOCK,
		OBJECT_CPU,
		OBJECT_DISPLAY,
		OBJECT_INPUT,
		OBJECT_MAPPER,
		OBJECT_MMU,
		OBJECT_PPU,
		OBJECT_ROM,
		OBJECT_RUNTIME,
	} object_t;

	#define OBJECT_MAX OBJECT_RUNTIME

	#define OBJECT_SUBTYPE_UNDEFINED SCALAR_INVALID(uint32_t)

	typedef enum {
		DATA_BOOLEAN = 0,
		DATA_FLOAT,
		DATA_SIGNED,
		DATA_STRING,
		DATA_UNSIGNED,
	} data_t;

	#define DATA_MAX DATA_UNSIGNED

	static const std::string DATA_STR[] = {
		"BOOLEAN", "FLOAT", "SIGNED", "STRING", "UNSIGNED",
		};

	#define DATA_STRING(_TYPE_) \
		((_TYPE_) > DATA_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(DATA_STR[_TYPE_]))

	typedef struct {
		mirra::data_t type;
		union {
			bool bvalue;
			float fvalue;
			int32_t ivalue;
			const char *strvalue;
			uint32_t uvalue;
		} data;
	} parameter_data_t;

	typedef std::map<uint32_t, mirra::parameter_data_t> object_parameter_t;

	typedef std::map<mirra::object_t, mirra::object_parameter_t> parameter_t;

	class object {

		public:

			object(
				__in mirra::object_t type,
				__in_opt uint32_t subtype = OBJECT_SUBTYPE_UNDEFINED
				);

			object(
				__in const object &other
				);

			virtual ~object(void);

			object &operator=(
				__in const object &other
				);

			static std::string as_string(
				__in const object &reference,
				__in_opt bool verbose = false
				);

			virtual void start(
				__in_opt const mirra::parameter_t &parameter = mirra::parameter_t()
				) = 0;

			virtual void stop(void) = 0;

			uint32_t subtype(void);

			virtual std::string to_string(
				__in_opt bool verbose = false
				);

			mirra::object_t type(void);

		protected:

			uint32_t m_subtype;

			mirra::object_t m_type;

	};
}

#endif // MIRRA_OBJECT_H_
