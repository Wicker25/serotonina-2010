/* 
    Title --- color.hpp

    Copyright (C) 2010 Giacomo Trudu - wicker25[at]gmail[dot]com

    This file is part of Serotonina.

    Serotonina is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation version 3 of the License.

    Serotonina is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Serotonina.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SEROTONINA_COLOR_HPP_
#define _SEROTONINA_COLOR_HPP_

#include <iostream>
#include <cstdio>
#include <stdint.h>

#include "version.hpp"

// Struttura di un colore RGB
struct ColorRGBub {

	uint8_t red;	// Rosso
	uint8_t green;	// Verde
	uint8_t blue;	// Blue

	// Metodi costruttore
	ColorRGBub() {

		red		= 0;
		green	= 0;
		blue	= 0;
	}

	ColorRGBub( uint8_t red_, uint8_t green_, uint8_t blue_ ) {

		red		= red_;
		green	= green_;
		blue	= blue_;
	}
};

#endif
