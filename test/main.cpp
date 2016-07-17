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

#include "../include/mirra_runtime.h"

int 
main(void)
{
	int result = 0;

	std::cout << mirra::version(true) << std::endl;

	try {
		mirra::runtime &instance = mirra::runtime::acquire();
		instance.initialize();
		instance.start();
		instance.wait();
		instance.stop();
		instance.uninitialize();
	} catch(mirra::exception &exc) {
		std::cerr << exc.to_string(true) << std::endl;
		result = SCALAR_INVALID(int);
	}

	return result;
}
