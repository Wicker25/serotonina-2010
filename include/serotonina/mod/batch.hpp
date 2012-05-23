/* 
    Title --- batch.hpp

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

#ifndef _SEROTONINA_BATCH_HPP_
#define _SEROTONINA_BATCH_HPP_

#include <iostream>

#include <serotonina/train-algorithm.hpp>

namespace serotonina { // Namespace di Serotonina

namespace algorithms { // Namespace degli algoritmi

/*!
 * @class Batch
 * \if english
 *   @brief Class of batch algorithm
 * \endif
 * \if italian
 *   @brief Classe dell'algoritmo batch
 * \endif
 */

class Batch : public TrainAlgorithm {

	/* ALGORITMO BATCH */

public:

	/*!
	 * @struct TrainData
	 * \if english
	 *   @brief Structure containing data of training
	 * \endif
	 * \if italian
	 *   @brief Struttura contenente i dati per l'addestramento
	 * \endif
	 */

	// Struttura contenente i dati per l'addestramento
	struct TrainData {

		/**
		 * \if english
		 *   @brief Weight's correction at previous epoch
		 * \endif
		 * \if italian
		 *   @brief Correzione del peso nell'epoca precedente
		 * \endif
		 */

		T_Precision delta_weight;	// Correzione del peso della connessione
	};

	// Descrizione dell'algoritmo
	static const char *description;

	// Verifica la correttezza dei parametri
	static bool check( std::vector< T_Precision > &train_params );

	// Inizializza le strutture per l'addestramento
	static void init( Network &network );

	// Esegue la correzione dei pesi
	static void updateWeights(	Network &network, std::vector< T_Precision > &train_params,
								T_Precision net_error, T_Precision old_net_error );

	// Deinizializza le strutture per l'addestramento
	static void end( Network &network );
};

} // Chiudo il namespace degli algoritmi

} // Chiudo il namespace di Serotonina

#endif
