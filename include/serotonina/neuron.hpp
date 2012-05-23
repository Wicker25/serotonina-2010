/* 
    Title --- neuron.hpp

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

#ifndef _SEROTONINA_NEURON_HPP_
#define _SEROTONINA_NEURON_HPP_

namespace serotonina { // Namespace di Serotonina

/*!
 * @struct Neuron
 * \if english
 *   @brief Neuron's structure
 * \endif
 * \if italian
 *   @brief Struttura di un neurone
 * \endif
 */

// Struttura di un neurone della rete
struct Neuron {

	/**
	 * \if english
	 *   @brief Transfer value
	 * \endif
	 * \if italian
	 *   @brief Valore di trasferimento
	 * \endif
	 */

	T_Precision value;	// Valore di trasferimento

	/**
	 * \if english
	 *   @brief Error of output
	 * \endif
	 * \if italian
	 *   @brief Errore sull'uscita
	 * \endif
	 */

	T_Precision dEdy;	// Errore sull'uscita
};

} // Chiudo il namespace di Serotonina

#endif
