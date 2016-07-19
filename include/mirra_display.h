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

#ifndef MIRRA_DISPLAY_H_
#define MIRRA_DISPLAY_H_

#include <vector>
#include <SDL2/SDL.h>
#include "mirra_singleton.h"

namespace mirra {

	enum {
		DISPLAY_PARAMETER_HEIGHT = 0,
		DISPLAY_PARAMETER_TITLE,
		DISPLAY_PARAMETER_WIDTH,
	};

	#define DISPLAY_PARAMETER_MAX DISPLAY_PARAMETER_WIDTH

	#define RGB(_R_, _G_, _B_) RGBA(_R_, _G_, _B_, UINT8_MAX)
	#define RGBA(_R_, _G_, _B_, _A_) \
		((((_B_) & UINT8_MAX) << 24) | (((_G_) & UINT8_MAX) << 16) \
		| (((_R_) & UINT8_MAX) << 8) | (_A_ & UINT8_MAX))

	class display :
			public mirra::singleton<mirra::display> {

		public:

			~display(void);

			uint32_t at(
				__in uint32_t x,
				__in uint32_t y
				);

			void clear(void);

			void initialize(
				__in_opt const mirra::parameter_t &parameter = mirra::parameter_t()
				);

			bool is_initialized(void);

			bool is_started(void);

			void set(
				__in uint32_t x,
				__in uint32_t y,
				__in uint32_t color
				);

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

			friend class mirra::singleton<mirra::display>;

			display(void);

			display(
				__in const display &other
				);

			display &operator=(
				__in const display &other
				);

			std::vector<uint32_t> m_frame;

			bool m_initialized;

			int32_t m_renderer_height;

			int32_t m_renderer_width;

			bool m_started;

			SDL_Window *m_window;

			SDL_Renderer *m_window_renderer;

			SDL_Texture *m_window_texture;
	};
}

#endif // MIRRA_DISPLAY_H_
