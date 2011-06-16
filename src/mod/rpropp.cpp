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

void
RpropPlus(	Network *network, const std::vector< T_Precision > &train_params,
			T_Precision net_error, T_Precision prev_net_error ) {

	// Iteratori
	short int t = ( network->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della RPROP+
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = network->layers[t + 1]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = network->layers[t + 1]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Imposto un tasso minimo di apprendimento (se fosse zero l'addestramento finirebbe)
			synapse_t->train->learning_rate = max( synapse_t->train->learning_rate, 0.0001 );

			// Calcolo la variazione della derivata rispetto all'epoca precedente
			T_Precision delta_sign = synapse_t->train->prev_dEdw * synapse_t->train->dEdw;

			// Applico le regole della RPROP+
			if ( delta_sign > 0.0 ) {

				// Incremento il tasso di apprendimento
				synapse_t->train->learning_rate = min( synapse_t->train->learning_rate * train_params[1], 50.0 );

				// Aggiorno il peso sinaptico
				synapse_t->train->delta_weight = - sign( synapse_t->train->dEdw ) * synapse_t->train->learning_rate;
				synapse_t->weight += synapse_t->train->delta_weight;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->train->prev_dEdw = synapse_t->train->dEdw;

			} else if ( delta_sign < 0.0 ) {

				// Decremento il tasso di apprendimento
				synapse_t->train->learning_rate = max( synapse_t->train->learning_rate * train_params[0], 0.0 );

				// Ripristino il vecchio peso sinaptico precedente
				synapse_t->weight -= synapse_t->train->delta_weight;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->train->prev_dEdw = 0.0;

			} else { // if ( delta_sign == 0.0 )

				// Aggiorno il peso sinaptico
				synapse_t->train->delta_weight = - sign( synapse_t->train->dEdw ) * synapse_t->train->learning_rate;
				synapse_t->weight += synapse_t->train->delta_weight;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->train->prev_dEdw = synapse_t->train->dEdw;
			}

			// Azzero l'errore del peso sinaptico
			synapse_t->train->dEdw = 0.0;
		}
	}
}

} // Chiudo il namespace degli algoritmi

} // Chiudo il namespace di Serotonina

#endif
