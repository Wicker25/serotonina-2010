/* 
    Title --- irpropm.cpp

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

#ifndef _SEROTONINA_IRPROPM_CPP_
#define _SEROTONINA_IRPROPM_CPP_

#include <serotonina/mod/irpropm.hpp>

namespace serotonina { // Namespace di Serotonina

namespace algorithms { // Namespace degli algoritmi

// Imposto la descrizione dell'algoritmo
const char *IRpropMinus::description = "IRprop-";

void
IRpropMinus::updateWeights(	Network &network, std::vector< T_Precision > &train_params,
							T_Precision net_error, T_Precision old_net_error ) {

	// Iteratori
	short int t = ( network.getLayers().size() - 1 );

	// Concordanza della derivata dell'errore
	T_Precision delta_sign;

	// Puntatore alla struttura contenente i dati dell'addestramento
	IRpropMinus::TrainData *training_data;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della IRPROP-
	for ( ; t > 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = network.getLayer(t).first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = network.getLayer(t).last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Ricavo la struttura contenente i dati dell'addestramento
			training_data = ((IRpropMinus::TrainData *) synapse_t->train);

			// Calcolo la variazione della derivata rispetto all'epoca precedente
			delta_sign = training_data->prev_dEdw * synapse_t->dEdw;

			// Applico le regole della IRPROP-
			if ( delta_sign > 0.0 ) {

				// Incremento il tasso di apprendimento
				training_data->learning_rate = min( training_data->learning_rate * train_params[1], train_params[3] );

			} else if ( delta_sign < 0.0 ) {

				// Decremento il tasso di apprendimento
				training_data->learning_rate = max( training_data->learning_rate * train_params[0], train_params[2] );

				// Azzero l'errore del peso sinaptico
				synapse_t->dEdw = 0.0;
			}

			// Aggiorno i pesi sinaptici
			synapse_t->weight += - sign( synapse_t->dEdw ) * training_data->learning_rate;

			// Memorizzo l'errore del peso sinaptico per il ciclo successivo
			training_data->prev_dEdw = synapse_t->dEdw;

			// Azzero l'errore del peso sinaptico
			synapse_t->dEdw = 0.0;
		}
	}
}

} // Chiudo il namespace degli algoritmi

} // Chiudo il namespace di Serotonina

#endif
