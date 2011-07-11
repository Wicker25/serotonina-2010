/* 
    Title --- synapse.hpp

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

#ifndef _SEROTONINA_SYNAPSE_HPP_
#define _SEROTONINA_SYNAPSE_HPP_

namespace Serotonina { // Namespace di Serotonina

/*!
 * @struct Synapse
 * \if english
 *   @brief Neuron's structure
 * \endif
 * \if italian
 *   @brief Struttura di un neurone
 * \endif
 */

// Struttura di una connessione tra due neuroni (sinapsi)
struct Synapse {

	/**
	 * \if english
	 *   @brief Weight of connection
	 * \endif
	 * \if italian
	 *   @brief Peso della connessione
	 * \endif
	 */

	T_Precision weight;	// Peso della connessione

	/**
	 * \if english
	 *   @brief Error of connection
	 * \endif
	 * \if italian
	 *   @brief Errore della connessione
	 * \endif
	 */

	T_Precision dEdw;	// Errore della connessione

	/**
	 * \if english
	 *   @brief Pointer to structure for training
	 * \endif
	 * \if italian
	 *   @brief Puntatore alla struttura per l'addestramento
	 * \endif
	 */

	void *train;	// Puntatore alla struttura per l'addestramento
};

} // Chiudo il namespace di Serotonina

#endif
