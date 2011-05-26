/* 
    Title --- ocr-inl.hpp

    Copyright (C) 2010 Giacomo Trudu - wicker25[at]gmail[dot]com

    This file is part of Serotonina.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _SEROTONINA_OCR_INL_HPP_
#define _SEROTONINA_OCR_INL_HPP_

#include "extra/ocr.hpp"

namespace Serotonina { // Namespace di Serotonina

inline void
Ocr::add_char_button_callback( Fl_Widget *widget, void *data ) {

	// Ricavo un puntatore alla finestra principale
	Ocr *ocr = static_cast< Ocr * >( data );

	// Chiama il metodo corrispondente
	ocr->AddCharacterData();
}

inline void *
Ocr::static_start_training( void *data ) {

	// Ricavo un puntatore alla finestra principale
	Ocr *ocr = static_cast< Ocr * >( data );

	// Chiama il metodo corrispondente
	ocr->TrainAllNetwork();

	return NULL;
}

}; // Chiudo il namespace di Serotonina

#endif