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

#ifndef MIRRA_CPU_H_
#define MIRRA_CPU_H_

#include "../include/mirra_bus.h"
#include "../include/mirra_singleton.h"

namespace mirra {

	typedef enum {
		ADDRESS_ABSOLUTE = 0,
		ADDRESS_ABSOLUTE_X,
		ADDRESS_ABSOLUTE_Y,
		ADDRESS_ACCUMULATOR,
		ADDRESS_IMMEDIATE,
		ADDRESS_IMPLIED,
		ADDRESS_INDIRECT,
		ADDRESS_INDIRECT_X,
		ADDRESS_INDIRECT_Y,
		ADDRESS_RELATIVE,
		ADDRESS_ZERO_PAGE,
		ADDRESS_ZERO_PAGE_X,
		ADDRESS_ZERO_PAGE_Y,
	} address_t;

	#define ADDRESS_MAX ADDRESS_ZERO_PAGE_Y

	enum {
		FLAG_CARRY = 0,
		FLAG_ZERO,
		FLAG_INTERRUPT_DISABLE,
		FLAG_UNUSED_0,
		FLAG_INTERRUPT_ACTIVE,
		FLAG_UNUSED_1,
		FLAG_OVERFLOW,
		FLAG_SIGN,
	};

	#define FLAG_MAX FLAG_SIGN

	STRUCT_PACK(flag_t, {
		uint8_t carry : 1;
		uint8_t zero : 1;
		uint8_t interrupt_disable : 1;
		uint8_t unused_0 : 1;
		uint8_t interrupt_active : 1;
		uint8_t unused_1 : 1;
		uint8_t overflow : 1;
		uint8_t sign : 1;
		});

	class cpu :
			public mirra::singleton<mirra::cpu>,
			public mirra::bus {

		public:

			~cpu(void);

			void initialize(
				__in_opt const mirra::parameter_t &parameter = mirra::parameter_t()
				);

			bool is_initialized(void);

			bool is_started(void);

			void start(
				__in_opt const mirra::parameter_t &parameter = mirra::parameter_t()
				);

			void stop(void);

			std::string to_string(
				__in_opt bool verbose = false
				);

			void uninitialize(void);

			void update(void);

		protected:

			friend class mirra::singleton<mirra::cpu>;

			cpu(void);

			cpu(
				__in const cpu &other
				);

			cpu &operator=(
				__in const cpu &other
				);

			void clear(void);

			uint16_t indirect_address(
				__in mirra::address_t mode,
				__in uint16_t address,
				__out bool &boundary
				);

			void irq(void);

			void nmi(void);

			uint8_t pop_byte(void);

			uint16_t pop_word(void);

			void push_byte(
				__in uint8_t value
				);

			void push_word(
				__in uint16_t value
				);

			uint8_t read_byte(
				__in uint16_t address
				);

			uint16_t read_word(
				__in uint16_t address
				);

			void reset(void);

			void write_byte(
				__in uint16_t address,
				__in uint8_t value
				);

			void write_word(
				__in uint16_t address,
				__in uint16_t value
				);

			uint8_t m_accumulator;

			uint32_t m_cycles;

			mirra::flag_t m_flag;

			uint8_t m_index_x;

			uint8_t m_index_y;

			bool m_initialized;

			uint16_t m_program_counter;

			uint8_t m_stack_pointer;

			bool m_started;
	};
}

#endif // MIRRA_CPU_H_
