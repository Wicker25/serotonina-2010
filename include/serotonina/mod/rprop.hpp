/* 
    Title --- rprop.hpp

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

#ifndef _SEROTONINA_RPROP_HPP_
#define _SEROTONINA_RPROP_HPP_

#include <iostream>

#include <serotonina/train-algorithm.hpp>

namespace serotonina { // Namespace di Serotonina

namespace algorithms { // Namespace degli algoritmi

/*!
 * @class Rprop
 * \if english
 *   @brief Class of rprop algorithm
 * \endif
 * \if italian
 *   @brief Classe dell'algoritmo rprop
 * \endif
 */

class Rprop : public TrainAlgorithm {

	/* ALGORITMO RPROP */

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
		 *   @brief Learning rate of the connection
		 * \endif
		 * \if italian
		 *   @brief Tasso di apprendimento della connessione
		 * \endif
		 */

		T_Precision learning_rate;	// Tasso di apprendimento della connessione

		/**
		 * \if english
		 *   @brief Weight's correction at previous epoch
		 * \endif
		 * \if italian
		 *   @brief Correzione del peso nell'epoca precedente
		 * \endif
		 */

		T_Precision delta_weight;	// Correzione del peso della connessione

		/**
		 * \if english
		 *   @brief The error of the connection
		 * \endif
		 * \if italian
		 *   @brief L'errore della connessione
		 * \endif
		 */

		T_Precision dEdw;			// Errore della connessione

		/**
		 * \if english
		 *   @brief The error of the connection at previous epoch
		 * \endif
		 * \if italian
		 *   @brief L'errore della connessione nell'epoca precedente
		 * \endif
		 */

		T_Precision prev_dEdw;		// Errore precedente della connessione

		// Metodo costruttore
		TrainData() {

			// Inizializzo i parametri dell'addestramento
			delta_weight	= 0.0;
			dEdw			= 0.0;
			prev_dEdw		= 0.0;
			learning_rate	= 0.1;
		}
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
