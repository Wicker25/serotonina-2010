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

// Prototipi
class Trainer;

class TrainAlgorithm {

	/* ALGORITMO DI ADDESTRAMENTO DELLA RETE NEURALE */

public:

	// Descrizione dell'algoritmo
	static const char *description;

	// Verifica la correttezza dei parametri
	static bool CheckParams( std::vector< T_Precision > &train_params );

	// Inizializza le strutture per l'addestramento
	static void InitTraining( Network &network );

	// Esegue la correzione dei pesi
	static void UpdateWeights(	Network &network, std::vector< T_Precision > &train_params,
								T_Precision net_error, T_Precision prev_net_error );

	// Deinizializza le strutture per l'addestramento
	static void EndTraining( Network &network );
};

} // Chiudo il namespace di Serotonina

#endif
