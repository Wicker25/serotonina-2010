/* 
    Title --- rprop.cpp

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

#ifndef _SEROTONINA_RPROP_CPP_
#define _SEROTONINA_RPROP_CPP_

#include <serotonina/mod/rprop.hpp>

namespace Serotonina { // Namespace di Serotonina

namespace Algorithms { // Namespace degli algoritmi

// Imposto la descrizione dell'algoritmo
const char *Rprop::description = "Rprop";

bool
Rprop::CheckParams( std::vector< T_Precision > &train_params ) {

	// Flag di controllo
	bool valid = false;

	// Verifico la correttezza dei parametri dell'addestramento
	if ( train_params.size() >= 2 ) {

		// Fattore di decremento
		if ( train_params[0] > 0.0 && train_params[0] < 1.0 ) {

			// Fattore di incremento
			if ( train_params[1] >= 1.0 ) {

				// Imposto il flag di controllo
				valid = true;

			// Communico l'errore all'utente
			} else std::cout << "- The increase factor must be greater than 1 ." << std::endl;

		// Communico l'errore all'utente
		} else std::cout << "- The decrease factor must be a positive number less than 1 ." << std::endl;
	}

	// Controllo se sono stati inseriti i parametri opzionali
	if ( train_params.size() >= 4 ) {

		// Tasso di apprendimento minimo
		if ( train_params[2] >= 0.0 ) {

			// Tasso di apprendimento massimo
			if ( train_params[3] > 0.0 ) {

				// Imposto il flag di controllo
				valid = true;

			// Communico l'errore all'utente
			} else std::cout << "- The max learning rate must be greater than or equal to 0 ." << std::endl;

			// Correggo il tasso di apprendimento minimo
			if ( train_params[2] == 0 ) train_params[2] = 0.00001;

		// Communico l'errore all'utente
		} else std::cout << "- The min learning rate must be greater than or equal to 0 ." << std::endl;

	} else {

		// Aggiungo i parametri opzionali
		train_params.resize(4);

		// Imposto il tasso di apprendimento minimo
		train_params[2] = 0.00001;
		// Imposto il tasso di apprendimento massimo
		train_params[3] = 50.0;
	}

	return valid;
}

void
Rprop::InitTraining( Network &network ) {

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
			synapse_t->train = new Rprop::TrainingData;
		}
	}
}

void
Rprop::UpdateWeights(	Network &network, std::vector< T_Precision > &train_params,
						T_Precision net_error, T_Precision old_net_error ) {

	// Iteratori
	short int t = ( network.GetLayers().size() - 1 );

	// Concordanza della derivata dell'errore
	T_Precision delta_sign;

	// Puntatore alla struttura contenente i dati dell'addestramento
	Rprop::TrainingData *training_data;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della RPROP
	for ( ; t > 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = network.GetLayer(t).first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = network.GetLayer(t).last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Ricavo la struttura contenente i dati dell'addestramento
			training_data = ((Rprop::TrainingData *) synapse_t->train);

			// Calcolo la variazione della derivata rispetto all'epoca precedente
			delta_sign = training_data->prev_dEdw * synapse_t->dEdw;

			// Applico le regole della RPROP
			if ( delta_sign > 0.0 ) {

				// Incremento il tasso di apprendimento
				training_data->learning_rate = min( training_data->learning_rate * train_params[1], train_params[3] );

				// Aggiorno il peso sinaptico
				synapse_t->weight += - sign( synapse_t->dEdw ) * training_data->learning_rate;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				training_data->prev_dEdw = synapse_t->dEdw;

			} else if ( delta_sign < 0.0 ) {

				// Decremento il tasso di apprendimento
				training_data->learning_rate = max( training_data->learning_rate * train_params[0], train_params[2] );

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				training_data->prev_dEdw = 0.0;

			} else { // if ( delta_sign == 0.0 )

				// Aggiorno il peso sinaptico
				synapse_t->weight += - sign( synapse_t->dEdw ) * training_data->learning_rate;
				training_data->prev_dEdw = synapse_t->dEdw;
			}

			// Azzero l'errore del peso sinaptico
			synapse_t->dEdw = 0.0;
		}
	}
}

void
Rprop::EndTraining( Network &network ) {

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
			delete (Rprop::TrainingData *) synapse_t->train;
		}
	}
}

} // Chiudo il namespace degli algoritmi

} // Chiudo il namespace di Serotonina

#endif
