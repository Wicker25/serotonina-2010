/* 
    Title --- ocr-inl.hpp

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

#ifndef _SEROTONINA_OCR_INL_HPP_
#define _SEROTONINA_OCR_INL_HPP_

#include "extra/ocr.hpp"

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

#endif
