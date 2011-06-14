/* 
    Title --- string.cpp

    Copyright (C) 2010 Giacomo Trudu - wicker25[at]gmail[dot]com

    This file is part of Serotonina.

    Serotonina is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    Serotonina is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Serotonina.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SEROTONINA_STRING_CPP_
#define _SEROTONINA_STRING_CPP_

#include <serotonina/string.hpp>
#include <serotonina/string-inl.hpp>

namespace Serotonina { // Namespace di Serotonina

size_t
values_from_string( const std::string &str, std::vector< T_Precision > &vect ) {

	// Posizione del valore trovato
	std::string::size_type found = 0;

	// Sottostringa contenente i valori
	std::string tmp;

	// Numero di elementi inseriti
	size_t elements = 0;

	// Estraggo i valori numerici da una stringa di testo
	while ( found != std::string::npos ) {

		// Ricavo la sottostringa contenente il nuovo valore
		tmp = str.substr( found, str.find_first_of( ' ', found + 1 ) );

		// Controllo che la sottostringa non sia vuota
		if ( !tmp.empty() && tmp != " " ) {

			// Aggiungo il valore al vettore
			vect.push_back( (T_Precision) atof( tmp.c_str() ) );

			// Incremento l'indice dell'elemento
			elements++;
		}

		// Cerco il prossimo valore
		found = str.find_first_of( ' ', found + 1 );
	}

	// Ritorno il numero degli elementi inseriti
	return elements;
}

} // Chiudo il namespace di Serotonina

#endif
