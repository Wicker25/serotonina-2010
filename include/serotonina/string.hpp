/* 
    Title --- string.hpp

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
    along with Serotonina. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SEROTONINA_STRING_HPP_
#define _SEROTONINA_STRING_HPP_

#include <iostream>
#include <cstdlib>

#include <serotonina/types.hpp>

namespace Serotonina { // Namespace di Serotonina

/**
 * \if english
 *   @brief Returns a vector containing the number in a string
 *   @param[in] str The string
 *   @param[out] vect The output vector
 *   @return Vector containing the number in a string
 * \endif
 * \if italian
 *   @brief Ritorna un vettore contenente i numeri in una stringa
 *   @param[in] str La stringa
 *   @param[out] vect Il vettore di uscita
 *   @return Un vettore contenente i numeri nella stringa
 * \endif
 */

template < typename T > size_t
values_from_string( const std::string &str, std::vector< T > &vect ) {

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
			vect.push_back( (T) atof( tmp.c_str() ) );

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
