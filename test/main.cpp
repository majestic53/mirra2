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

/*void 
setup(
	__inout mirra::parameter_t &configuration
	)
{
	mirra::parameter_t::iterator iter;
	mirra::object_parameter_t::iterator attribute_iter;

	configuration.insert(std::pair<mirra::object_t, mirra::object_parameter_t>(
		mirra::OBJECT_INPUT, mirra::object_parameter_t()));

	iter = configuration.find(mirra::OBJECT_INPUT);
	if(iter != configuration.end()) {
		iter->second.insert(std::pair<uint32_t, mirra::parameter_data_t>(mirra::INPUT_PARAMETER_JOYPAD_1_B, 
			{mirra::DATA_UNSIGNED}));

		attribute_iter = iter->second.find(mirra::INPUT_PARAMETER_JOYPAD_1_B);
		if(attribute_iter != iter->second.end()) {
			attribute_iter->second.data.uvalue = SDL_SCANCODE_R;
		}
	}
}*/

int 
main(void)
{
	int result = 0;
	std::string title;
	//mirra::parameter_t configuration;

	title = mirra::version(true);
	std::cout << title << std::endl;

	try {
		mirra::runtime &instance = mirra::runtime::acquire();
		instance.initialize();
		//setup(configuration);
		instance.start(/*configuration*/);
		instance.wait();
		instance.stop();
		instance.uninitialize();
	} catch(mirra::exception &exc) {
		std::cerr << exc.to_string(true) << std::endl;
		result = SCALAR_INVALID(int);
	}

	return result;
}
