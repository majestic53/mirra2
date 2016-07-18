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

#include "../include/mirra_display.h"
#include "mirra_display_type.h"

namespace mirra {

	#define PIXEL_FILL RGB(0, 0, 0)

	#define PIXEL_INDEX(_X_, _Y_, _W_) (((_Y_) * (_W_)) + (_X_))

	#define WINDOW_INIT_FLAGS (0)
	#define WINDOW_RENDERER_INIT_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC \
		| SDL_RENDERER_TARGETTEXTURE)

	static const std::string DISPLAY_PARAMETER_STR[] = {
		"HEIGHT", "TITLE", "WIDTH",
		};

	#define DISPLAY_PARAMETER_STRING(_TYPE_) \
		((_TYPE_) > DISPLAY_PARAMETER_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(DISPLAY_PARAMETER_STR[_TYPE_]))

	display::display(void) :
		mirra::singleton<mirra::display>(OBJECT_DISPLAY),
		m_initialized(false),
		m_renderer_height(0),
		m_renderer_width(0),
		m_started(false),
		m_window(nullptr),
		m_window_renderer(nullptr),
		m_window_texture(nullptr)
	{
		return;
	}

	display::~display(void)
	{
		uninitialize();
	}

	uint32_t 
	display::at(
		__in uint32_t x,
		__in uint32_t y
		)
	{
		uint32_t index;

		if(!m_initialized) {
			THROW_MIRRA_DISPLAY_EXCEPTION(MIRRA_DISPLAY_EXCEPTION_UNINITIALIZED);
		}

		if(!m_started) {
			THROW_MIRRA_DISPLAY_EXCEPTION(MIRRA_DISPLAY_EXCEPTION_STOPPED);
		}

		index = PIXEL_INDEX(x, y, m_renderer_width);
		if(index >= m_frame.size()) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_INVALID_COORDINATE,
				"{%u, %u} [%u]", x, y, index);
		}

		return m_frame.at(index);
	}

	void 
	display::clear(void)
	{

		if(!m_initialized) {
			THROW_MIRRA_DISPLAY_EXCEPTION(MIRRA_DISPLAY_EXCEPTION_UNINITIALIZED);
		}

		if(!m_started) {
			THROW_MIRRA_DISPLAY_EXCEPTION(MIRRA_DISPLAY_EXCEPTION_STOPPED);
		}

		m_frame.clear();
		m_frame.resize(m_renderer_width * m_renderer_height, PIXEL_FILL);
		update();
	}

	void 
	display::initialize(
		__in_opt const mirra::parameter_t &parameter
		)
	{

		if(m_initialized) {
			THROW_MIRRA_DISPLAY_EXCEPTION(MIRRA_DISPLAY_EXCEPTION_INITIALIZED);
		}

		m_parameter_initialize = parameter;
		m_initialized = true;
	}

	bool 
	display::is_initialized(void)
	{
		return m_initialized;
	}

	bool 
	display::is_started(void)
	{
		return m_started;
	}

	void 
	display::set(
		__in uint32_t x,
		__in uint32_t y,
		__in uint32_t color
		)
	{
		uint32_t index;

		if(!m_initialized) {
			THROW_MIRRA_DISPLAY_EXCEPTION(MIRRA_DISPLAY_EXCEPTION_UNINITIALIZED);
		}

		if(!m_started) {
			THROW_MIRRA_DISPLAY_EXCEPTION(MIRRA_DISPLAY_EXCEPTION_STOPPED);
		}

		index = PIXEL_INDEX(x, y, m_renderer_width);
		if(index >= m_frame.size()) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_INVALID_COORDINATE,
				"{%u, %u} [%u]", x, y, index);
		}

		m_frame.at(index) = color;
	}

	void 
	display::start(
		__in_opt const mirra::parameter_t &parameter
		)
	{
		std::string title;
		uint32_t height, width;
		mirra::parameter_t::const_iterator iter;
		mirra::object_parameter_t::const_iterator attribute_iter;

		if(!m_initialized) {
			THROW_MIRRA_DISPLAY_EXCEPTION(MIRRA_DISPLAY_EXCEPTION_UNINITIALIZED);
		}

		if(m_started) {
			THROW_MIRRA_DISPLAY_EXCEPTION(MIRAR_DISPLAY_EXCEPTION_STARTED);
		}

		m_parameter_start = parameter;

		iter = parameter.find(OBJECT_DISPLAY);
		if(iter == parameter.end()) {
			THROW_MIRRA_DISPLAY_EXCEPTION(MIRRA_DISPLAY_EXCEPTION_MISSING);
		}

		attribute_iter = iter->second.find(DISPLAY_PARAMETER_TITLE);
		if(attribute_iter == iter->second.end()) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_MISSING,
				"%s", DISPLAY_PARAMETER_STRING(DISPLAY_PARAMETER_TITLE));
		}

		if(attribute_iter->second.type != DATA_STRING) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_MISSING,
				"%s: %s (expecting %s)", DISPLAY_PARAMETER_STRING(DISPLAY_PARAMETER_TITLE),
				DATA_STRING(attribute_iter->second.type), DATA_STRING(DATA_STRING));
		}

		title = (attribute_iter->second.data.strvalue ? attribute_iter->second.data.strvalue :
			STRING_EMPTY);

		attribute_iter = iter->second.find(DISPLAY_PARAMETER_WIDTH);
		if(attribute_iter == iter->second.end()) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_MISSING,
				"%s", DISPLAY_PARAMETER_STRING(DISPLAY_PARAMETER_WIDTH));
		}

		if(attribute_iter->second.type != DATA_UNSIGNED) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_MISSING,
				"%s: %s (expecting %s)", DISPLAY_PARAMETER_STRING(DISPLAY_PARAMETER_WIDTH),
				DATA_STRING(attribute_iter->second.type), DATA_STRING(DATA_UNSIGNED));
		}

		width = attribute_iter->second.data.uvalue;

		attribute_iter = iter->second.find(DISPLAY_PARAMETER_HEIGHT);
		if(attribute_iter == iter->second.end()) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_MISSING,
				"%s", DISPLAY_PARAMETER_STRING(DISPLAY_PARAMETER_HEIGHT));
		}

		if(attribute_iter->second.type != DATA_UNSIGNED) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_MISSING,
				"%s: %s (expecting %s)", DISPLAY_PARAMETER_STRING(DISPLAY_PARAMETER_HEIGHT),
				DATA_STRING(attribute_iter->second.type), DATA_STRING(DATA_UNSIGNED));
		}

		height = attribute_iter->second.data.uvalue;
		m_window = SDL_CreateWindow(STRING_CHECK(title), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			width, height, WINDOW_INIT_FLAGS);

		if(!m_window) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_EXTERNAL,
				"SDL_CreateWindow: %s", SDL_GetError());
		}

		m_window_renderer = SDL_CreateRenderer(m_window, SCALAR_INVALID(int), WINDOW_RENDERER_INIT_FLAGS);
		if(!m_window_renderer) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_EXTERNAL,
				"SDL_CreateRenderer: %s", SDL_GetError());
		}

		if(SDL_GetRendererOutputSize(m_window_renderer, &m_renderer_width, &m_renderer_height)) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_EXTERNAL,
				"SDL_GetRendererOutputSize: %s", SDL_GetError());
		}

		if(SDL_SetRenderDrawBlendMode(m_window_renderer, SDL_BLENDMODE_BLEND)) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_EXTERNAL,
				"SDL_SetRenderDrawBlendMode: %s", SDL_GetError());
		}

		m_window_texture = SDL_CreateTexture(m_window_renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING,
			width, height);

		if(!m_window_texture) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_EXTERNAL,
				"SDL_CreateTexture: %s", SDL_GetError());
		}

		if(SDL_SetTextureBlendMode(m_window_texture, SDL_BLENDMODE_BLEND)) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_EXTERNAL,
				"SDL_SetTextureBlendMode: %s", SDL_GetError());
		}

		m_started = true;
		clear();
	}

	void 
	display::stop(void)
	{

		if(m_initialized && m_started) {
			m_started = false;

			if(m_window_texture) {
				SDL_DestroyTexture(m_window_texture);
				m_window_texture = nullptr;
			}

			if(m_window_renderer) {
				SDL_DestroyRenderer(m_window_renderer);
				m_window_renderer = nullptr;
			}

			if(m_window) {
				SDL_DestroyWindow(m_window);
				m_window = nullptr;
			}

			m_renderer_height = 0;
			m_renderer_width = 0;
			m_frame.clear();
			m_parameter_start.clear();
		}
	}

	std::string 
	display::to_string(
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
				result << ", WIN=" << SCALAR_AS_HEX(uintptr_t, m_window)
					<< " (" << m_renderer_width << ", " << m_renderer_height << ")"
					<< ", REN=" << SCALAR_AS_HEX(uintptr_t, m_window_renderer)
					<< ", TXT=" << SCALAR_AS_HEX(uintptr_t, m_window_texture);
			}
		}

		return result.str();
	}

	void 
	display::uninitialize(void)
	{

		if(m_initialized) {
			stop();
			m_initialized = false;
			m_parameter_initialize.clear();
		}
	}

	void 
	display::update(void)
	{

		if(!m_initialized) {
			THROW_MIRRA_DISPLAY_EXCEPTION(MIRRA_DISPLAY_EXCEPTION_UNINITIALIZED);
		}

		if(!m_started) {
			THROW_MIRRA_DISPLAY_EXCEPTION(MIRRA_DISPLAY_EXCEPTION_STOPPED);
		}

		if(SDL_UpdateTexture(m_window_texture, nullptr, &m_frame[0], m_renderer_width * sizeof(uint32_t))) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_EXTERNAL,
				"SDL_UpdateTexture: %s", SDL_GetError());
		}

		if(SDL_RenderClear(m_window_renderer)) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_EXTERNAL,
				"SDL_RenderClear: %s", SDL_GetError());
		}

		if(SDL_RenderCopy(m_window_renderer, m_window_texture, nullptr, nullptr)) {
			THROW_MIRRA_DISPLAY_EXCEPTION_FORMAT(MIRRA_DISPLAY_EXCEPTION_EXTERNAL,
				"SDL_RenderCopy: %s", SDL_GetError());
		}

		SDL_RenderPresent(m_window_renderer);
	}
}
