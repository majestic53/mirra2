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

	#define _BIT_OPERATION(_OP_, _VAL_, _BIT_) ((_VAL_)_OP_(1 << (_BIT_)))
	#define BIT_CHECK(_VAL_, _BIT_) _BIT_OPERATION(&, _VAL_, _BIT_)
	#define BIT_CLEAR(_VAL_, _BIT_)	_BIT_OPERATION(&=~, _VAL_, _BIT_)
	#define BIT_SET(_VAL_, _BIT_) _BIT_OPERATION(|=, _VAL_, _BIT_)
	#define BIT_TOGGLE(_VAL_, _BIT_) _BIT_OPERATION(^=, _VAL_, _BIT_)

	#define WORD(_LOW_, _HIGH_) ((_LOW_) | ((_HIGH_) << CHAR_BIT))
	#define WORD_HIGH(_WORD_) WORD_LOW((_WORD_) >> CHAR_BIT)
	#define WORD_LOW(_WORD_) ((_WORD_) & UINT8_MAX)

	enum {
		OPCODE_ADC_ABSOLUTE = 0x6d,
		OPCODE_ADC_ABSOLUTE_X = 0x7d,
		OPCODE_ADC_ABSOLUTE_Y = 0x79,
		OPCODE_ADC_IMMEDIATE = 0x69,
		OPCODE_ADC_INDIRECT_X = 0x61,
		OPCODE_ADC_INDIRECT_Y = 0x71,
		OPCDOE_ADC_ZERO_PAGE = 0x65,
		OPCODE_ADC_ZERO_PAGE_X = 0x75,

		OPCODE_AND_ABSOLUTE = 0x2d,
		OPCODE_AND_ABSOLUTE_X = 0x3d,
		OPCODE_AND_ABSOLUTE_Y = 0x39,
		OPCODE_AND_IMMEDIATE = 0x29,
		OPCODE_AND_INDIRECT_X = 0x21,
		OPCODE_AND_INDIRECT_Y = 0x31,
		OPCODE_AND_ZERO_PAGE = 0x25,
		OPCODE_AND_ZERO_PAGE_X = 0x35,

		OPCODE_ASL_ABSOLUTE = 0xe,
		OPCODE_ASL_ABSOLUTE_X = 0x1e,
		OPCODE_ASL_ACCUMULATOR = 0xa,
		OPCODE_ASL_ZERO_PAGE = 0x6,
		OPCODE_ASL_ZERO_PAGE_X = 0x16,

		OPCODE_BIT_ABSOLUTE = 0x2c,
		OPCODE_BIT_ZERO_PAGE = 0x24,

		OPCODE_BCC_RELATIVE = 0x90,
		OPCODE_BEQ_RELATIVE = 0xf0,
		OPCODE_BMI_RELATIVE = 0x30,
		OPCODE_BNE_RELATIVE = 0xd0,
		OPCODE_BPL_RELATIVE = 0x10,
		OPCODE_BVC_RELATIVE = 0x50,
		OPCODE_BVS_RELATIVE = 0x70,

		OPCODE_BRK_IMPLIED = 0x0,

		OPCODE_CMP_ABSOLUTE = 0xcd,
		OPCODE_CMP_ABSOLUTE_X = 0xdd,
		OPCODE_CMP_ABSOLUTE_Y = 0xd9,
		OPCODE_CMP_IMMEDIATE = 0xc9,
		OPCODE_CMP_INDIRECT_X = 0xc1,
		OPCODE_CMP_INDIRECT_Y = 0xd1,
		OPCODE_CMP_ZERO_PAGE = 0xc5,
		OPCODE_CMP_ZERO_PAGE_X = 0xd5,

		OPCODE_CPX_ABSOLUTE = 0xec,
		OPCODE_CPX_IMMEDIATE = 0xe0,
		OPCODE_CPX_ZERO_PAGE = 0xe4,

		OPCODE_CPY_ABSOLUTE = 0xcc,
		OPCODE_CPY_IMMEDIATE = 0xc0,
		OPCODE_CPY_ZERO_PAGE = 0xc4,

		OPCODE_DEC_ABSOLUTE = 0xce,
		OPCODE_DEC_ABSOLUTE_X = 0xde,
		OPCODE_DEC_ZERO_PAGE = 0xc6,
		OPCODE_DEC_ZERO_PAGE_X = 0xd6,

		OPCODE_EOR_ABSOLUTE = 0x4d,
		OPCODE_EOR_ABSOLUTE_X = 0x5d,
		OPCODE_EOR_ABSOLUTE_Y = 0x59,
		OPCODE_EOR_IMMEDIATE = 0x49,
		OPCODE_EOR_INDIRECT_X = 0x41,
		OPCODE_EOR_INDIRECT_Y = 0x51,
		OPCODE_EOR_ZERO_PAGE = 0x45,
		OPCODE_EOR_ZERO_PAGE_X = 0x55,

		OPCODE_CLC_IMPLIED = 0x18,
		OPCODE_CLD_IMPLIED = 0xd8,
		OPCODE_CLI_IMPLIED = 0x58,
		OPCODE_CLV_IMPLIED = 0xb8,
		OPCODE_SEC_IMPLIED = 0x38,
		OPCODE_SED_IMPLIED = 0xf8,
		OPCODE_SEI_IMPLIED = 0x78,

		OPCODE_INC_ABSOLUTE = 0xee,
		OPCODE_INC_ABSOLUTE_X = 0xfe,
		OPCODE_INC_ZERO_PAGE = 0xe6,
		OPCODE_INC_ZERO_PAGE_X = 0xf6,

		OPCODE_JMP_ABSOLUTE = 0x4c,
		OPCODE_JMP_INDIRECT = 0x6c,

		OPCODE_JSR_ABSOLUTE = 0x20,

		OPCODE_LDA_ABSOLUTE = 0xad,
		OPCODE_LDA_ABSOLUTE_X = 0xbd,
		OPCODE_LDA_ABSOLUTE_Y = 0xb9,
		OPCODE_LDA_IMMEDIATE = 0xa9,
		OPCODE_LDA_INDIRECT_X = 0xa1,
		OPCODE_LDA_INDIRECT_Y = 0xb1,
		OPCODE_LDA_ZERO_PAGE = 0xa5,
		OPCODE_LDA_ZERO_PAGE_X = 0xb5,

		OPCODE_LDX_ABSOLUTE = 0xae,
		OPCODE_LDX_ABSOLUTE_Y = 0xbe,
		OPCODE_LDX_IMMEDIATE = 0xa2,
		OPCODE_LDX_ZERO_PAGE = 0xa6,
		OPCODE_LDX_ZERO_PAGE_Y = 0xb6,

		OPCODE_LDY_ABSOLUTE = 0xac,
		OPCODE_LDY_ABSOLUTE_X = 0xbc,
		OPCODE_LDY_IMMEDIATE = 0xa0,
		OPCODE_LDY_ZERO_PAGE = 0xa4,
		OPCODE_LDY_ZERO_PAGE_X = 0xb4,

		OPCODE_LSR_ABSOLUTE = 0x4e,
		OPCODE_LSR_ABSOLUTE_X = 0x5e,
		OPCODE_LSR_ACCUMULATOR = 0x4a,
		OPCODE_LSR_ZERO_PAGE = 0x46,
		OPCODE_LSR_ZERO_PAGE_X = 0x56,

		OPCODE_NOP_IMPLIED = 0xea,

		OPCODE_ORA_ABSOLUTE = 0xd,
		OPCODE_ORA_ABSOLUTE_X = 0x1d,
		OPCODE_ORA_ABSOLUTE_Y = 0x19,
		OPCODE_ORA_IMMEDIATE = 0x9,
		OPCODE_ORA_INDIRECT_X = 0x1,
		OPCODE_ORA_INDIRECT_Y = 0x11,
		OPCODE_ORA_ZERO_PAGE = 0x5,
		OPCODE_ORA_ZERO_PAGE_X = 0x15,

		OPCODE_DEX_IMPLIED = 0xca,
		OPCODE_DEY_IMPLIED = 0x88,
		OPCODE_INX_IMPLIED = 0xe8,
		OPCODE_INY_IMPLIED = 0xc8,
		OPCODE_TAX_IMPLIED = 0xaa,
		OPCODE_TAY_IMPLIED = 0xa8,
		OPCODE_TXA_IMPLIED = 0x8a,
		OPCODE_TYA_IMPLIED = 0x98,

		OPCODE_ROL_ABSOLUTE = 0x2e,
		OPCODE_ROL_ABSOLUTE_X = 0x3e,
		OPCODE_ROL_ACCUMULATOR = 0x2a,
		OPCODE_ROL_ZERO_PAGE = 0x26,
		OPCODE_ROL_ZERO_PAGE_X = 0x36,

		OPCODE_ROR_ABSOLUTE = 0x6e,
		OPCODE_ROR_ABSOLUTE_X = 0x7e,
		OPCODE_ROR_ACCUMULATOR = 0x6a,
		OPCODE_ROR_ZERO_PAGE = 0x66,
		OPCODE_ROR_ZERO_PAGE_X = 0x76,

		OPCODE_RTI_IMPLIED = 0x40,

		OPCODE_RTS_IMPLIED = 0x60,

		OPCODE_SBC_ABSOLUTE = 0xed,
		OPCODE_SBC_ABSOLUTE_X = 0xfd,
		OPCODE_SBC_ABSOLUTE_Y = 0xf9,
		OPCODE_SBC_IMMEDIATE = 0xe9,
		OPCODE_SBC_INDIRECT_X = 0xe1,
		OPCODE_SBC_INDIRECT_Y = 0xf1,
		OPCODE_SBC_ZERO_PAGE = 0xe5,
		OPCODE_SBC_ZERO_PAGE_X = 0xf5,

		OPCODE_STA_ABSOLUTE = 0x8d,
		OPCODE_STA_ABSOLUTE_X = 0x9d,
		OPCODE_STA_ABSOLUTE_Y = 0x99,
		OPCODE_STA_INDIRECT_X = 0x81,
		OPCODE_STA_INDIRECT_Y = 0x91,
		OPCODE_STA_ZERO_PAGE = 0x85,
		OPCODE_STA_ZERO_PAGE_X = 0x95,

		OPCODE_TSX_IMPLIED = 0xba,
		OPCODE_TXS_IMPLIED = 0x9a,
		OPCODE_PHA_IMPLIED = 0x48,
		OPCODE_PHP_IMPLIED = 0x8,
		OPCODE_PLA_IMPLIED = 0x68,
		OPCODE_PLP_IMPLIED = 0x28,

		OPCODE_STX_ABSOLUTE = 0x8e,
		OPCODE_STX_ZERO_PAGE = 0x86,
		OPCODE_STX_ZERO_PAGE_Y = 0x96,

		OPCODE_STY_ABSOLUTE = 0x8c,
		OPCODE_STY_ZERO_PAGE = 0x84,
		OPCODE_STY_ZERO_PAGE_X = 0x94,
	};

	static const std::string ADDRESS_STR[] = {
		"ABSOLUTE", "ABSOLUTE_X", "ABSOLUTE_Y", "ACCUMULATOR", "IMMEDIATE",
		"IMPLIED", "INDIRECT", "INDIRECT_X", "INDIRECT_Y", "RELATIVE",
		"ZERO_PAGE", "ZERO_PAGE_X", "ZERO_PAGE_Y",
		};

	#define ADDRESS_STRING(_TYPE_) \
		((_TYPE_) > ADDRESS_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(ADDRESS_STR[_TYPE_]))

	#define FLAG_UNDEFINED "*"

	static const std::string FLAG_STR[] = {
		"C", "Z", "I", FLAG_UNDEFINED, "B", FLAG_UNDEFINED, "V", "S",
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
		push_word(m_program_counter);
		push(*(uint8_t *) &m_flag);
		m_flag.interrupt_disable = true;
		m_program_counter = read_word(INTERRUPT_IRQ);
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
		push_word(m_program_counter);
		push(*(uint8_t *) &m_flag);
		m_flag.interrupt_disable = true;
		m_program_counter = read_word(INTERRUPT_NMI);
	}

	uint8_t 
	cpu::pop(void)
	{
		return read(++m_stack_pointer + STACK_BASE);
	}

	uint16_t 
	cpu::pop_word(void)
	{
		return WORD(pop(), pop());
	}

	void 
	cpu::push(
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
		push(WORD_HIGH(value));
		push(WORD_LOW(value));
	}

	uint8_t 
	cpu::read(
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
		return WORD(read(address), read(address + 1));
	}

	template <typename T> std::string 
	cpu::register_as_string(
		__in T value,
		__in_opt bool verbose,
		__in_opt bool flag
		)
	{
		int iter;
		std::stringstream result;

		result << SCALAR_AS_HEX(T, value);

		if(verbose) {
			result << " (";

			for(iter = ((CHAR_BIT * sizeof(T)) - 1); iter >= 0; --iter) {

				if(flag && (iter <= FLAG_MAX)) {
					result << (BIT_CHECK(value, iter) ? FLAG_STRING(iter) : FLAG_UNDEFINED);
				} else {
					result << (BIT_CHECK(value, iter) ? "1" : "0");
				}
			}

			result << ")";
		}

		return result.str();
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
				result << ", CYCLES=" << m_cycles
					<< ", A=" << mirra::cpu::register_as_string<uint8_t>(m_accumulator, verbose)
					<< ", X=" << mirra::cpu::register_as_string<uint8_t>(m_index_x, verbose)
					<< ", Y=" << mirra::cpu::register_as_string<uint8_t>(m_index_y, verbose)
					<< ", FLG=" << mirra::cpu::register_as_string<uint8_t>(*(uint8_t *) &m_flag, verbose, true)
					<< ", PC=" << mirra::cpu::register_as_string<uint16_t>(m_program_counter, verbose)
					<< ", SP=" << mirra::cpu::register_as_string<uint16_t>(m_stack_pointer + STACK_BASE,
						verbose);
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
		uint8_t code;

		if(!m_initialized) {
			THROW_MIRRA_CPU_EXCEPTION(MIRRA_CPU_EXCEPTION_UNINITIALIZED);
		}

		if(!m_started) {
			THROW_MIRRA_CPU_EXCEPTION(MIRRA_CPU_EXCEPTION_STOPPED);
		}

		code = read(m_program_counter++);
		switch(code) {

			// TODO: process instruction at MMU[PC]

			default:
				THROW_MIRRA_CPU_EXCEPTION_FORMAT(MIRRA_CPU_EXCEPTION_INVALID_INSTRUCTION,
					"%x", code);
		}
	}

	void 
	cpu::write(
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
		write(address, WORD_LOW(value));
		write(address + 1, WORD_HIGH(value));
	}
}
