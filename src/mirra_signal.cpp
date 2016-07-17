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

#include <chrono>
#include "../include/mirra_signal.h"
#include "mirra_signal_type.h"

namespace mirra {

	static const std::string SIGNAL_STR[] = {
		"ABANDON", "CLEAR", "NOTIFY", "TIMEOUT",
		};

	#define SIGNAL_STRING(_TYPE_) \
		((_TYPE_) > SIGNAL_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(SIGNAL_STR[_TYPE_]))

	signal::signal(
		__in_opt bool notified,
		__in_opt bool resetting
		) :
			m_resetting(resetting),
			m_state(notified ? SIGNAL_NOTIFY : SIGNAL_CLEAR)
	{
		return;
	}

	signal::signal(
		__in const signal &other
		) :
			m_resetting(other.m_resetting),
			m_state(other.m_state)
	{
		return;
	}

	signal::~signal(void)
	{
		abandon();
	}

	signal &
	signal::operator=(
		__in const signal &other
		)
	{

		if(this != &other) {
			abandon();
			std::lock_guard<std::mutex> lock(m_mutex);
			m_resetting = other.m_resetting;
			m_state = other.m_state;
		}

		return *this;
	}

	void 
	signal::abandon(void)
	{
		set(SIGNAL_ABANDON);
	}

	std::string 
	signal::as_string(
		__in const signal &reference,
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << "[" << SIGNAL_STRING(reference.m_state);

		if(reference.m_state > SIGNAL_MAX) {
			result << " (" << SCALAR_AS_HEX(mirra::signal_t, reference.m_state) << ")";
		}

		result << ", " << (reference.m_resetting ? "AUTO" : "MANUAL") << "]";

		return result.str();
	}

	void 
	signal::clear(void)
	{
		set(SIGNAL_CLEAR);
	}

	bool 
	signal::is_notifiable(void)
	{
		return (state() == SIGNAL_CLEAR);
	}

	void 
	signal::notify(void)
	{
		set(SIGNAL_NOTIFY);
	}

	void 
	signal::set(
		__in mirra::signal_t state
		)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if(state > SIGNAL_MAX) {
			THROW_MIRRA_SIGNAL_EXCEPTION_FORMAT(MIRRA_SIGNAL_EXCEPTION_INVALID,
				"%x", m_state);
		} else if(m_state == SIGNAL_ABANDON) {
			THROW_MIRRA_SIGNAL_EXCEPTION(MIRRA_SIGNAL_EXCEPTION_ABANDONED);
		} else if((m_state != SIGNAL_CLEAR) && (state == SIGNAL_NOTIFY)) {
			THROW_MIRRA_SIGNAL_EXCEPTION_FORMAT(MIRRA_SIGNAL_EXCEPTION_BAD_STATE,
				"%s (%x)", SIGNAL_STRING(m_state), m_state);
		}

		m_state = state;
		m_condition.notify_all();
	}

	mirra::signal_t 
	signal::state(void)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_state;
	}

	std::string 
	signal::to_string(
		__in_opt bool verbose
		)
	{
		return mirra::signal::as_string(*this, verbose);
	}

	mirra::signal_t 
	signal::wait(
		__in_opt uint32_t timeout
		)
	{
		mirra::signal_t result;

		result = state();
		if(result == SIGNAL_ABANDON) {
			THROW_MIRRA_SIGNAL_EXCEPTION(MIRRA_SIGNAL_EXCEPTION_ABANDONED);
		}

		if(result == SIGNAL_CLEAR) {
			std::unique_lock<std::mutex> lock(m_mutex);

			if(timeout != SIGNAL_TIMEOUT_UNDEFINED) {

				if(!m_condition.wait_for(lock, std::chrono::milliseconds(timeout), 
						[this](void) { return (m_state != SIGNAL_CLEAR); })) {
					m_state = SIGNAL_TIMEOUT;
				}
			} else {
				m_condition.wait(lock, [this](void) { return (m_state != SIGNAL_CLEAR); });
			}

			result = m_state;
		}

		if((m_state == SIGNAL_NOTIFY) && m_resetting) {
			m_state = SIGNAL_CLEAR;
		}

		return result;
	}
}
