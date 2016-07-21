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

#include "../include/mirra_cpu.h"
#include "mirra_cpu_type.h"

namespace mirra {

	#define INTERRUPT_IRQ 0xfffe
	#define INTERRUPT_NMI 0xfffa
	#define INTERRUPT_RESET 0xfffc

	#define REGISTER_FLAG_DEFAULT {0, 0, 1, 0, 1, 1, 0, 0}
	#define REGISTER_ACCUMULATOR_DEFAULT 0
	#define REGISTER_INDEX_X_DEFAULT 0
	#define REGISTER_INDEX_Y_DEFAULT 0
	#define REGISTER_PROGRAM_COUNTER_DEFAULT 0
	#define REGISTER_STACK_POINTER_DEFAULT UINT8_MAX

	#define STACK_BASE (UINT8_MAX + 1)

	static const std::string ADDRESS_STR[] = {
		"ABSOLUTE", "ABSOLUTE_X", "ABSOLUTE_Y", "ACCUMULATOR", "IMMEDIATE",
		"IMPLIED", "INDIRECT", "INDIRECT_X", "INDIRECT_Y", "RELATIVE",
		"ZERO_PAGE", "ZERO_PAGE_X", "ZERO_PAGE_Y",
		};

	#define ADDRESS_STRING(_TYPE_) \
		((_TYPE_) > ADDRESS_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(ADDRESS_STR[_TYPE_]))

	static const std::string FLAG_STR[] = {
		"C", "Z", "I", "-", "-", "B", "V", "S",
		};

	#define FLAG_STRING(_TYPE_) \
		((_TYPE_) > FLAG_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(FLAG_STR[_TYPE_]))

	cpu::cpu(void) :
		mirra::singleton<mirra::cpu>(OBJECT_CPU),
		m_accumulator(REGISTER_ACCUMULATOR_DEFAULT),
		m_cycles(0),
		m_flag(REGISTER_FLAG_DEFAULT),
		m_index_x(REGISTER_INDEX_X_DEFAULT),
		m_index_y(REGISTER_INDEX_Y_DEFAULT),
		m_initialized(false),
		m_program_counter(REGISTER_PROGRAM_COUNTER_DEFAULT),
		m_stack_pointer(REGISTER_STACK_POINTER_DEFAULT),
		m_started(false)
	{
		return;
	}

	cpu::~cpu(void)
	{
		return;
	}

	void 
	cpu::clear(void)
	{
		m_accumulator = REGISTER_ACCUMULATOR_DEFAULT;
		m_cycles = 0;
		m_flag = REGISTER_FLAG_DEFAULT;
		m_index_x = REGISTER_INDEX_X_DEFAULT;
		m_index_y = REGISTER_INDEX_Y_DEFAULT;
		m_program_counter = REGISTER_PROGRAM_COUNTER_DEFAULT;
		m_stack_pointer = REGISTER_STACK_POINTER_DEFAULT;
	}

	uint16_t 
	cpu::indirect_address(
		__in mirra::address_t mode,
		__in uint16_t address,
		__out bool &boundary
		)
	{
		uint16_t result = 0;

		switch(mode) {
			case ADDRESS_ABSOLUTE_X:
				result = (address + m_index_x);
				boundary = (((address & UINT8_MAX) + m_index_x) > UINT8_MAX);
				break;
			case ADDRESS_ABSOLUTE_Y:
				result = (address + m_index_y);
				boundary = (((address & UINT8_MAX) + m_index_y) > UINT8_MAX);
				break;
			case ADDRESS_INDIRECT:
				result = read_word(address);
				boundary = false;
				break;
			case ADDRESS_INDIRECT_X:
				result = read_word(address + m_index_x);
				boundary = false;
				break;
			case ADDRESS_INDIRECT_Y:
				result = read_word(address);
				boundary = (((result & UINT8_MAX) + m_index_y) > UINT8_MAX);
				result += m_index_y;
				break;
			case ADDRESS_ZERO_PAGE_X:
				result = ((address + m_index_x) % UINT8_MAX);
				boundary = false;
				break;
			case ADDRESS_ZERO_PAGE_Y:
				result = ((address + m_index_y) % UINT8_MAX);
				boundary = false;
				break;
			default:
				THROW_MIRRA_CPU_EXCEPTION_FORMAT(MIRRA_CPU_EXCEPTION_INVALID_INDIRECT,
					"%s (%x)", ADDRESS_STRING(mode), mode);
		}

		return result;
	}

	void 
	cpu::initialize(
		__in_opt const mirra::parameter_t &parameter
		)
	{

		if(m_initialized) {
			THROW_MIRRA_CPU_EXCEPTION(MIRRA_CPU_EXCEPTION_INITIALIZED);
		}

		m_initialized = true;
	}

	void 
	cpu::irq(void)
	{
		// TODO: perform irq
	}

	bool 
	cpu::is_initialized(void)
	{
		return m_initialized;
	}

	bool 
	cpu::is_started(void)
	{
		return m_started;
	}

	void 
	cpu::nmi(void)
	{
		// TODO: perform nmi
	}

	uint8_t 
	cpu::pop_byte(void)
	{
		return read(++m_stack_pointer + STACK_BASE);
	}

	uint16_t 
	cpu::pop_word(void)
	{
		return (pop_byte() | (pop_byte() << CHAR_BIT));
	}

	void 
	cpu::push_byte(
		__in uint8_t value
		)
	{
		write(m_stack_pointer-- + STACK_BASE, value);
	}

	void 
	cpu::push_word(
		__in uint16_t value
		)
	{
		push_byte(value >> CHAR_BIT);
		push_byte(value);
	}

	uint8_t 
	cpu::read_byte(
		__in uint16_t address
		)
	{

		// TODO: read byte from MMU
		return 0;
		// ---
	}

	uint16_t 
	cpu::read_word(
		__in uint16_t address
		)
	{
		return ((read_byte(address + 1) << CHAR_BIT) | read_byte(address));
	}

	void 
	cpu::reset(void)
	{
		clear();
		m_program_counter = read_word(INTERRUPT_RESET);
	}

	void 
	cpu::start(
		__in_opt const mirra::parameter_t &parameter
		)
	{

		if(!m_initialized) {
			THROW_MIRRA_CPU_EXCEPTION(MIRRA_CPU_EXCEPTION_UNINITIALIZED);
		}

		if(m_started) {
			THROW_MIRRA_CPU_EXCEPTION(MIRRA_CPU_EXCEPTION_STARTED);
		}

		reset();
		m_started = true;
	}

	void 
	cpu::stop(void)
	{

		if(!m_initialized) {
			THROW_MIRRA_CPU_EXCEPTION(MIRRA_CPU_EXCEPTION_UNINITIALIZED);
		}

		if(m_started) {
			clear();
			m_started = false;
		}
	}

	std::string 
	cpu::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << mirra::object::as_string(*this, verbose)
			<< " (" << (m_initialized ? "INIT" : "UNINIT")
			<< ", " << (m_started ? "START" : "STOP") << ")";

		if(m_initialized) {
			result << " INST=" << SCALAR_AS_HEX(uintptr_t, this);

			if(m_started) {

				// TODO: print cpu register state
			}
		}

		return result.str();
	}

	void 
	cpu::uninitialize(void)
	{

		if(m_initialized) {
			stop();
			m_initialized = true;
		}
	}

	void 
	cpu::update(void)
	{

		if(!m_initialized) {
			THROW_MIRRA_CPU_EXCEPTION(MIRRA_CPU_EXCEPTION_UNINITIALIZED);
		}

		if(!m_started) {
			THROW_MIRRA_CPU_EXCEPTION(MIRRA_CPU_EXCEPTION_STOPPED);
		}

		// TODO: process instruction at MMU[PC]
	}

	void 
	cpu::write_byte(
		__in uint16_t address,
		__in uint8_t value
		)
	{
		// TODO: write byte to MMU
	}

	void 
	cpu::write_word(
		__in uint16_t address,
		__in uint16_t value
		)
	{
		write_byte(address, value);
		write_byte(address + 1, value >> CHAR_BIT);
	}
}
