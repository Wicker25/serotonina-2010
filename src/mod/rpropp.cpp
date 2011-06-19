/* 
    Title --- rpropp.cpp

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

#ifndef _SEROTONINA_RPROPP_CPP_
#define _SEROTONINA_RPROPP_CPP_

#include <serotonina/mod/rpropp.hpp>

namespace Serotonina { // Namespace di Serotonina

namespace Algorithms { // Namespace degli algoritmi

// Imposto la descrizione dell'algoritmo
const char *RpropPlus::description = "Rprop+";

void
RpropPlus::UpdateWeights(	Network &network, std::vector< T_Precision > &train_params,
							T_Precision net_error, T_Precision prev_net_error ) {

	// Iteratori
	short int t = ( network.GetLayers().size() - 1 );

	// Concordanza della derivata dell'errore
	T_Precision delta_sign;

	// Puntatore alla struttura contenente i dati dell'addestramento
	RpropPlus::TrainingData *training_data;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della RPROP+
	for ( ; t > 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = network.GetLayer(t).first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = network.GetLayer(t).last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Ricavo la struttura contenente i dati dell'addestramento
			training_data = ((RpropPlus::TrainingData *) synapse_t->train);

			// Calcolo la variazione della derivata rispetto all'epoca precedente
			delta_sign = training_data->prev_dEdw * synapse_t->dEdw;

			// Applico le regole della RPROP+
			if ( delta_sign > 0.0 ) {

				// Incremento il tasso di apprendimento
				training_data->learning_rate = min( training_data->learning_rate * train_params[1], train_params[3] );

				// Aggiorno il peso sinaptico
				training_data->delta_weight = - sign( synapse_t->dEdw ) * training_data->learning_rate;
				synapse_t->weight += training_data->delta_weight;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				training_data->prev_dEdw = synapse_t->dEdw;

			} else if ( delta_sign < 0.0 ) {

				// Decremento il tasso di apprendimento
				training_data->learning_rate = max( training_data->learning_rate * train_params[0], train_params[2] );

				// Ripristino il vecchio peso sinaptico precedente
				synapse_t->weight -= training_data->delta_weight;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				training_data->prev_dEdw = 0.0;

			} else { // if ( delta_sign == 0.0 )

				// Aggiorno il peso sinaptico
				training_data->delta_weight = - sign( synapse_t->dEdw ) * training_data->learning_rate;
				synapse_t->weight += training_data->delta_weight;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				training_data->prev_dEdw = synapse_t->dEdw;
			}

			// Azzero l'errore del peso sinaptico
			synapse_t->dEdw = 0.0;
		}
	}
}

} // Chiudo il namespace degli algoritmi

} // Chiudo il namespace di Serotonina

#endif
