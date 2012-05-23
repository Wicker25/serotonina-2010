/* 
	Title --- create_train_algorithm.cpp

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

    To compile: g++ `pkg-config --libs --cflags serotonina` create_train_algorithm.cpp -o create_train_algorithm
*/

#include <iostream>

// Includo l'intestazione di Serotonina
#include <serotonina.hpp>

// Uso il namespace di Serotonina
using namespace serotonina;

class MyBatch : public TrainAlgorithm {

	/* ALGORITMO PERSONALIZZATO */

public:

	// Struttura contenente i dati per l'addestramento
	struct TrainData {

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
								T_Precision net_error, T_Precision prev_net_error );

	// Deinizializza le strutture per l'addestramento
	static void end( Network &network );
};


// Imposto la descrizione dell'algoritmo
const char *MyBatch::description = "MyBatch :P";

bool
MyBatch::check( std::vector< T_Precision > &train_params ) {

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
MyBatch::init( Network &network ) {

	// Iteratori
	short int t = ( network.getLayers().size() - 1 );

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Creo le strutture necessarie all'addestramento
	for ( ; t > 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = network.getLayer(t).first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = network.getLayer(t).last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Creo la struttura per l'addestramento
			synapse_t->train = new MyBatch::TrainData;
		}
	}
}

void
MyBatch::updateWeights(	Network &network, std::vector< T_Precision > &train_params,
						T_Precision net_error, T_Precision prev_net_error ) {

	// Iteratori
	short int t = ( network.getLayers().size() - 1 );

	// Puntatore alla struttura contenente i dati dell'addestramento
	MyBatch::TrainData *training_data;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando la Regola Delta con il Momentum
	for ( ; t > 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = network.getLayer(t).first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = network.getLayer(t).last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Ricavo la struttura contenente i dati dell'addestramento
			training_data = ((MyBatch::TrainData *) synapse_t->train);

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
MyBatch::end( Network &network ) {

	// Iteratori
	short int t = ( network.getLayers().size() - 1 );

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Cancello le strutture necessarie all'addestramento
	for ( ; t > 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = network.getLayer(t).first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = network.getLayer(t).last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Cancello la struttura per l'addestramento
			delete (MyBatch::TrainData *) synapse_t->train;
		}
	}
}


int main( void ) {

	// Creo la rete neurale
	Network network( 3, 2, 10, 1 );

	// Creo l'addestratore della rete neurale
	Trainer trainer( network );

	// Addestro la rete neurale con il metodo Batch
	trainer.setParams( 0.5, 0.8 );
	trainer.trainOnFile< MyBatch >( "train/xor.train", 0.000001, 100000, 5000 );

	// Ingressi della rete neurale
	T_Precision in[2];

	// Iteratori
	size_t i, j = 0;

	// Eseguo alcuni test con la rete neurale
	for ( ; j <= 1; j++ ) {

		for ( i = 0; i <= 1; i++ ) {

			// Imposto gli ingressi
			in[0] = (T_Precision) j;
			in[1] = (T_Precision) i;

			// Calcolo l'uscita della rete neurale
			const std::vector< T_Precision > &out = network.run( in );

			// Stampo i risultati
			std::cout.setf( std::ios::fixed, std::ios::floatfield );
			std::cout << "Run ( " << in[0] << " AND " << in[1] << " ) = " << out[0] << std::endl;
		}
	}

	return 0;
}

