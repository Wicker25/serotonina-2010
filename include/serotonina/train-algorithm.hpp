/* 
    Title --- train-algorithm.hpp

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

#ifndef _SEROTONINA_TRAIN_ALGORITHM_HPP_
#define _SEROTONINA_TRAIN_ALGORITHM_HPP_

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#include <vector>
#include <ctime>

#include <serotonina/version.hpp>
#include <serotonina/types.hpp>
#include <serotonina/macros.hpp>
#include <serotonina/string.hpp>

#include <serotonina/neuron.hpp>
#include <serotonina/synapse.hpp>
#include <serotonina/layer.hpp>

#include <serotonina/network.hpp>
#include <serotonina/network-inl.hpp>

namespace Serotonina { // Namespace di Serotonina

/*!
 * @namespace Serotonina::Algorithms
 * \if english
 *   Namespace of training algorithms.
 * \endif
 * \if italian
 *   Namespace degli algoritmi di addestramento
 * \endif
 */
 
// Prototipi
class Trainer;

/*!
 * @class TrainAlgorithm
 * \if english
 *   @brief Class of a training algorithm
 * \endif
 * \if italian
 *   @brief Classe di un algoritmo di addestramento
 * \endif
 */

class TrainAlgorithm {

	/* ALGORITMO DI ADDESTRAMENTO DELLA RETE NEURALE */

public:

	/**
	 * \if english
	 *   @brief Description of training algorithm
	 * \endif
	 * \if italian
	 *   @brief Descrizione dell'algoritmo di addestramento
	 * \endif
	 */

	// Descrizione dell'algoritmo
	static const char *description;

	/**
	 * \if english
	 *   @brief Check parameters of training
	 *   @param[in] train_params The parameters of training passed by Trainer
	 *   @return Result of checking
	 *   @retval true If the parameters are correct
	 *   @retval false If the parameters aren't correct
	 * \endif
	 * \if italian
	 *   @brief Controlla i parametri dell'addestramento
	 *   @param[in] train_params I parametri dell'addestramento passati Trainer
	 *   @return Risultato del controllo
	 *   @retval true Se i parametri sono corretti
	 *   @retval false Se i parametri non sono corretti
	 * \endif
	 */

	// Verifica la correttezza dei parametri
	static bool CheckParams( std::vector< T_Precision > &train_params );

	/**
	 * \if english
	 *   @brief Initializes structures of training
	 *   @param[in] network The neural network passed by Trainer
	 * \endif
	 * \if italian
	 *   @brief Inizializza le strutture dell'addestramento
	 *   @param[in] network La rete neurale passata dal Trainer
	 * \endif
	 */

	// Inizializza le strutture per l'addestramento
	static void InitTraining( Network &network );

	/**
	 * \if english
	 *   @brief Function to update weights
	 *   @param[in] network The neural network passed by Trainer
	 *   @param[in] train_params The parameters of training passed by Trainer
	 *   @param[in] net_error The network's error
	 *   @param[in] old_net_error The network's error at previous epoch
	 * \endif
	 * \if italian
	 *   @brief Funzione per la correzione dei pesi
	 *   @param[in] network La rete neurale passata dal Trainer
	 *   @param[in] train_params I parametri dell'addestramento passati dall'addestratore
	 *   @param[in] net_error L'errore della rete
	 *   @param[in] old_net_error L'errore della rete nell'epoca precedente
	 * \endif
	 */

	// Esegue la correzione dei pesi
	static void UpdateWeights(	Network &network, std::vector< T_Precision > &train_params,
								T_Precision net_error, T_Precision old_net_error );

	/**
	 * \if english
	 *   @brief Deinitializes structures of training
	 *   @param[in] network The neural network passed by Trainer
	 * \endif
	 * \if italian
	 *   @brief Deinizializza le strutture dell'addestramento
	 *   @param[in] network La rete neurale passata dal Trainer
	 * \endif
	 */

	// Deinizializza le strutture per l'addestramento
	static void EndTraining( Network &network );
};

} // Chiudo il namespace di Serotonina

#endif
