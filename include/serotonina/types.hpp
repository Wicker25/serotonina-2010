/* 
    Title --- types.hpp

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

#ifndef _SEROTONINA_TYPES_HPP_
#define _SEROTONINA_TYPES_HPP_

#include <vector>

namespace serotonina { // Namespace di Serotonina

/**
 * \typedef T_Precision
 * \if english
 *   @brief The type used for structures of the neural network
 * \endif
 * \if italian
 *   @brief Il tipo usato per le strutture della rete neurale
 * \endif
 */

// Precisione dei valori della rete
//typedef float Precision;
typedef double T_Precision;

} // Chiudo il namespace di Serotonina

#endif
