/* 
    Title --- macros.hpp

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

#ifndef _SEROTONINA_MACROS_HPP_
#define _SEROTONINA_MACROS_HPP_

#include <cstdlib>
#include <cmath>

namespace Serotonina { // Namespace di Serotonina

/**
 * \if english
 *   @brief Returns the absolute value of 'x'
 *   @param[in] x The number 'x'
 *   @return The absolute value of 'x'
 * \endif
 * \if italian
 *   @brief Ritorna il valore assoluto di 'x'
 *   @param[in] x Il numero 'x'
 *   @return Il valore assoluto di 'x'
 * \endif
 */

template < typename T >
inline T abs( const T &x ) {

	// Ritorna il valore assoluto di 'x'
	return ( (x > 0) ? x : -x );
}

/**
 * \if english
 *   @brief Returns the lesser of 'x' and 'y'
 *   @param[in] x The number 'x'
 *   @param[in] y The number 'y'
 *   @return The lesser of 'x' and 'y'
 * \endif
 * \if italian
 *   @brief Ritorna il più piccolo tra 'x' e 'y'
 *   @param[in] x Il numero 'x'
 *   @param[in] y Il numero 'y'
 *   @return Il più piccolo tra 'x' e 'y'
 * \endif
 */

template < typename T >
inline T min( const T &x, const T &y ) {

	// Ritorna il valore minimo tra 'x' ed 'y'
	return ( (x < y) ? x : y );
}

/**
 * \if english
 *   @brief Returns the greater of 'x' and 'y'
 *   @param[in] x The number 'x'
 *   @param[in] y The number 'y'
 *   @return The greater of 'x' and 'y'
 * \endif
 * \if italian
 *   @brief Ritorna il più grande tra 'x' e 'y'
 *   @param[in] x Il numero 'x'
 *   @param[in] y Il numero 'y'
 *   @return Il più grande tra 'x' e 'y'
 * \endif
 */

template < typename T >
inline T max( const T &x, const T &y ) {

	// Ritorna il valore massimo tra 'x' ed 'y'
	return ( (x > y) ? x : y );
}

/**
 * \if english
 *   @brief Returns 'x' between 'y' and 'z'
 *   @param[in] x The number 'x'
 *   @param[in] y The number 'y'
 *   @param[in] z The number 'z'
 *   @return The value 'x' between 'y' and 'z'
 * \endif
 * \if italian
 *   @brief Ritorna il valore 'x' compresto tra 'y' e 'z'
 *   @param[in] x Il numero 'x'
 *   @param[in] y Il numero 'y'
 *   @param[in] z Il numero 'z'
 *   @return Il valore 'x' compresto tra 'y' e 'z''
 * \endif
 */

template < typename T >
inline T between( const T &x, const T &y, const T &z ) {

	// Ritorna il valore compreso tra due soglie
	return ( (x < y) ? y : ((x > z) ? z : x) );
}

/**
 * \if english
 *   @brief Returns the sign of 'x'
 *   @param[in] x The number 'x'
 *   @return The sign of 'x'
 *   @retval -1 if x is negative
 *   @retval 0 if x is null
 *   @retval 1 if x is positive
 * \endif
 * \if italian
 *   @brief Ritorna il segno di 'x'
 *   @param[in] x Il numero 'x'
 *   @return Il segno di 'x'
 *   @retval -1 se x è negativo
 *   @retval 0 se x è nullo
 *   @retval 1 se x è positivo
 * \endif
 */

template < typename T >
inline T sign( const T &x ) {

	// Ritorna il segno di 'x'
	return ( (x == 0) ? 0 : (x > 0) ? +1 : -1 );
}

/**
 * \if english
 *   @brief Returns the square of 'x'
 *   @param[in] x The number 'x'
 *   @return The square of 'x'
 * \endif
 * \if italian
 *   @brief Ritorna il quadrato di 'x'
 *   @param[in] x Il numero 'x'
 *   @return Il quadrato di 'x'
 * \endif
 */

template < typename T >
inline T pow2( const T &x ) {

	// Ritorna la potenza di 'x'
	return (x * x);
}

/**
 * \if english
 *   @brief Returns the cube of 'x'
 *   @param[in] x The number 'x'
 *   @return The cube of 'x'
 * \endif
 * \if italian
 *   @brief Ritorna il cubo di 'x'
 *   @param[in] x Il numero 'x'
 *   @return Il cubo di 'x'
 * \endif
 */

template < typename T >
inline T pow3( const T &x ) {

	// Ritorna il cubo di 'x'
	return (x * x * x);
}

/**
 * \if english
 *   @brief Returns a pseudo-random number between 0 and 1
 *   @return A pseudo-random number between 0 and 1
 * \endif
 * \if italian
 *   @brief Ritorna un numero pseudo-casuale compreso tra 0 e 1
 *   @return Un numero pseudo-casuale compreso tra 0 e 1
 * \endif
 */

template < typename T >
inline T rand0to1() {

	// Ritorna un numero pseudocasuale compreso tra 0 e 1
	return (T) pow2( std::sin( rand() ) );
}

/* FUNZIONI DI TRASFERIMENTO E RISPETTIVE DERIVATE */

/**
 * \if english
 *   @brief Returns sigmoid(x)
 *   @param[in] x The number 'x'
 *   @return sigmoid(x)
 * \endif
 * \if italian
 *   @brief Ritorna sigmoid(x)
 *   @param[in] x Il numero 'x'
 *   @return sigmoid(x)
 * \endif
 */

template < typename T >
inline T sigmoid( const T &x ) {

	// Funzione sigmoide
	return ( 1.0 / ( 1.0 + std::exp(-x) ) );
}

/**
 * \if english
 *   @brief Returns sigmoid'(x) = sigmoid(x)(1 - sigmoid(x))
 *   @param[in] x The number 'x'
 *   @return sigmoid'(x) = sigmoid(x)(1 - sigmoid(x))
 * \endif
 * \if italian
 *   @brief Ritorna sigmoid'(x) = sigmoid(x)(1 - sigmoid(x))
 *   @param[in] x Il numero 'x'
 *   @return sigmoid'(x) = sigmoid(x)(1 - sigmoid(x))
 * \endif
 */
 
template < typename T >
inline T d_sigmoid( const T &x ) {

	// Derivata della sigmoide come sigmoid'(x) = sigmoid(x)(1 - sigmoid(x))
	return ( x * ( 1 - x ) );
}

} // Chiudo il namespace di Serotonina

#endif
