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

#ifndef MIRRA_SINGLETON_H_
#define MIRRA_SINGLETON_H_

#include "mirra_object.h"

namespace mirra {

	template <class T> class singleton :
			public mirra::object {

		public:

			virtual ~singleton(void)
			{
				return;
			}

			static T &acquire(void)
			{
				static T instance;
				return instance;
			}

			virtual void initialize(
				__in_opt const mirra::parameter_t &parameter = mirra::parameter_t()
				) = 0;

			virtual void start(
				__in_opt const mirra::parameter_t &parameter = mirra::parameter_t()
				) = 0;

			virtual void stop(void) = 0;

			virtual void uninitialize(void) = 0;

		protected:

			singleton(
				__in mirra::object_t type,
				__in_opt uint32_t subtype = OBJECT_SUBTYPE_UNDEFINED
				) :
					mirra::object(type, subtype)
			{
				return;
			}

			singleton(
				__in const singleton &other
				);

			singleton &operator=(
				__in const singleton &other
				);
	};
}

#endif // MIRRA_SINGLETON_H_
