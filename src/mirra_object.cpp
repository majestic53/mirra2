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

#include "../include/mirra_object.h"
#include "mirra_object_type.h"

namespace mirra {

	static const std::string OBJECT_STR[] = {
		"APU", "BLOCK", "CPU", "DISPLAY", "INPUT",
		"MAPPER", "MMU", "PPU", "ROM", "RUNTIME",
		};

	#define OBJECT_STRING(_TYPE_) \
		((_TYPE_) > OBJECT_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(OBJECT_STR[_TYPE_]))

	object::object(
		__in mirra::object_t type,
		__in_opt uint32_t subtype
		) :
			m_subtype(subtype),
			m_type(type)
	{

		if(m_type > OBJECT_MAX) {
			THROW_MIRRA_OBJECT_EXCEPTION_FORMAT(MIRRA_OBJECT_EXCEPTION_INVALID,
				"%x", m_type);
		}
	}

	object::object(
		__in const object &other
		) :
			m_subtype(other.m_subtype),
			m_type(other.m_type)
	{
		return;
	}

	object::~object(void)
	{
		return;
	}

	object &
	object::operator=(
		__in const object &other
		)
	{

		if(this != &other) {
			m_subtype = other.m_subtype;
			m_type = other.m_type;
		}

		return *this;
	}

	std::string 
	object::as_string(
		__in const object &reference,
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << "[" << OBJECT_STRING(reference.m_type);

		if(reference.m_type > OBJECT_MAX) {
			result << " (" << SCALAR_AS_HEX(mirra::object_t, reference.m_type) << ")";
		}

		if(reference.m_subtype != OBJECT_SUBTYPE_UNDEFINED) {
			result << ", " << SCALAR_AS_HEX(uint32_t, reference.m_subtype);
		}

		result << "]";

		return result.str();
	}

	uint32_t 
	object::subtype(void)
	{
		return m_subtype;
	}

	std::string 
	object::to_string(
		__in_opt bool verbose
		)
	{
		return mirra::object::as_string(*this, verbose);
	}

	mirra::object_t 
	object::type(void)
	{
		return m_type;
	}
}
