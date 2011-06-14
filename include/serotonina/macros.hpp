/* 
    Title --- macros.hpp

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

#ifndef _SEROTONINA_MACROS_HPP_
#define _SEROTONINA_MACROS_HPP_

#include <cstdlib>
#include <cmath>

namespace Serotonina { // Namespace di Serotonina

template < typename T >
inline T abs( const T &x ) {

	// Ritorna il valore assoluto di 'x'
	return ( (x > 0) ? x : -x );
}

template < typename T >
inline T min( const T &x, const T &y ) {

	// Ritorna il valore minimo tra 'x' ed 'y'
	return ( (x < y) ? x : y );
}

template < typename T >
inline T max( const T &x, const T &y ) {

	// Ritorna il valore massimo tra 'x' ed 'y'
	return ( (x > y) ? x : y );
}

template < typename T >
inline T between( const T &x, const T &y, const T &z ) {

	// Ritorna il valore compreso tra due soglie
	return ( (x < y) ? y : ((x > z) ? z : x) );
}

template < typename T >
inline T sign( const T &x ) {

	// Ritorna il segno di 'x'
	return ( (x == 0) ? 0 : (x > 0) ? +1 : -1 );
}

template < typename T >
inline T pow2( const T &x ) {

	// Ritorna la potenza di 'x'
	return (x * x);
}

template < typename T >
inline T pow3( const T &x ) {

	// Ritorna il cubo di 'x'
	return (x * x * x);
}

template < typename T >
inline T rand0to1() {

	// Ritorna un numero pseudocasuale compreso tra 0 e 1
	return (T) pow2( std::sin( rand() ) );
}

/** FUNZIONI DI TRASFERIMENTO E RISPETTIVE DERIVATE **/

template < typename T >
inline T sigmoid( const T &x ) {

	// Funzione sigmoide
	return ( 1.0 / ( 1.0 + std::exp(-x) ) );
}

template < typename T >
inline T d_sigmoid( const T &x ) {

	// Derivata della sigmoide come T'(x) = T(x)(1 - T(x))
	return ( x * ( 1 - x ) );
}

} // Chiudo il namespace di Serotonina

#endif
