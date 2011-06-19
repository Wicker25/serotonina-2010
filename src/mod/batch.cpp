/* 
    Title --- batch.cpp

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

#ifndef _SEROTONINA_BATCH_CPP_
#define _SEROTONINA_BATCH_CPP_

#include <serotonina/mod/batch.hpp>

namespace Serotonina { // Namespace di Serotonina

namespace Algorithms { // Namespace degli algoritmi

// Imposto la descrizione dell'algoritmo
const char *Batch::description = "Batch";

bool
Batch::CheckParams( std::vector< T_Precision > &train_params ) {

	// Flag di controllo
	bool valid = false;

	// Verifico la correttezza dei parametri dell'addestramento
	if ( train_params.size() >= 2 ) {

		// Tasso di apprendimento
		if ( train_params[0] > 0.0 && train_params[0] < 1.0 ) {

			// Momentum
			if ( train_params[1] >= 0.0 && train_params[1] < 1.0 ) {

				// Imposto il flag di controllo
				valid = true;

			// Communico l'errore all'utente
			} else std::cout << "- The momentum must be between 0.0 and 1.0 ." << std::endl;

		// Communico l'errore all'utente
		} else std::cout << "- The learning rate must be a positive number less than 1 ." << std::endl;
	}

	return valid;
}

void
Batch::AllocData( Network &network ) {

	// Iteratori
	short int t = ( network.GetLayers().size() - 1 );

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Creo le strutture necessarie all'addestramento
	for ( ; t > 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = network.GetLayer(t).first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = network.GetLayer(t).last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Creo la struttura per l'addestramento
			synapse_t->train = new Batch::TrainingData;
		}
	}
}

void
Batch::UpdateWeights(	Network &network, std::vector< T_Precision > &train_params,
						T_Precision net_error, T_Precision prev_net_error ) {

	// Iteratori
	short int t = ( network.GetLayers().size() - 1 );

	// Puntatore alla struttura contenente i dati dell'addestramento
	Batch::TrainingData *training_data;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando la Regola Delta con il Momentum
	for ( ; t > 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = network.GetLayer(t).first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = network.GetLayer(t).last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Ricavo la struttura contenente i dati dell'addestramento
			training_data = ((Batch::TrainingData *) synapse_t->train);

			// Calcolo la modifica del peso
			training_data->delta_weight = - train_params[0] * synapse_t->dEdw + \
											 train_params[1] * training_data->delta_weight;

			// Aggiorno il peso sinaptico
			synapse_t->weight += training_data->delta_weight;

			// Azzero l'errore del peso sinaptico
			synapse_t->dEdw = 0.0;
		}
	}
}

void
Batch::DeallocData( Network &network ) {

	// Iteratori
	short int t = ( network.GetLayers().size() - 1 );

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Cancello le strutture necessarie all'addestramento
	for ( ; t > 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = network.GetLayer(t).first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = network.GetLayer(t).last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Cancello la struttura per l'addestramento
			delete (Batch::TrainingData *) synapse_t->train;
		}
	}
}

} // Chiudo il namespace degli algoritmi

} // Chiudo il namespace di Serotonina

#endif
