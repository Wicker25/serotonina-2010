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
    along with Serotonina.  If not, see <http://www.gnu.org/licenses/>.

    To compile: g++ `pkg-config --libs --cflags serotonina` create_train_algorithm.cpp -o create_train_algorithm
*/

#include <iostream>

// Includo l'intestazione di Serotonina
#include <serotonina.hpp>

// Uso il namespace di Serotonina
using namespace Serotonina;

class MyBatch : public TrainAlgorithm {

	/* ALGORITMO PERSONALIZZATO */

public:

	// Descrizione dell'algoritmo
	static const char *description;

	// Verifica la correttezza dei parametri
	static bool CheckParams( const std::vector< T_Precision > &train_params );

	// Esegue la correzione dei pesi
	static void UpdateWeights(	Network &network, const std::vector< T_Precision > &train_params,
								T_Precision net_error, T_Precision prev_net_error );
};


// Imposto la descrizione dell'algoritmo
const char *MyBatch::description = "MyBatch :P";

bool
MyBatch::CheckParams( const std::vector< T_Precision > &train_params ) {

	// Flag di controllo
	bool valid = false;

	// Verifico la correttezza dei parametri dell'addestramento
	if ( train_params.size() >= 2 ) {

		if ( train_params[0] > 0.0 && train_params[0] < 1.0 ) {

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
MyBatch::UpdateWeights(	Network &network, const std::vector< T_Precision > &train_params,
						T_Precision net_error, T_Precision prev_net_error ) {

	// Iteratori
	short int t = ( network.GetLayers().size() - 1 );

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

			// Calcolo la modifica del peso
			synapse_t->train->delta_weight = - train_params[0] * synapse_t->train->dEdw + train_params[1] * synapse_t->train->delta_weight;

			// Aggiorno il peso sinaptico
			synapse_t->weight += synapse_t->train->delta_weight;

			// Azzero l'errore del peso sinaptico
			synapse_t->train->dEdw = 0.0;
		}
	}
}

int main( void ) {

	// Creo la rete neurale
	Network network( 3, 2, 10, 1 );

	// Creo l'addestratore della rete neurale
	Trainer trainer( network );

	// Addestro la rete neurale con il metodo Batch
	trainer.SetParameters( 0.5, 0.8 );
	trainer.TrainOnFile< MyBatch >( "train/xor.train", 0.000001, 100000, 5000 );

	// Ingressi della rete neurale
	Serotonina::T_Precision in[2];

	// Iteratori
	size_t i, j = 0;

	// Eseguo alcuni test con la rete neurale
	for ( ; j <= 1; j++ ) {

		for ( i = 0; i <= 1; i++ ) {

			// Imposto gli ingressi
			in[0] = (T_Precision) j;
			in[1] = (T_Precision) i;

			// Calcolo l'uscita della rete neurale
			const std::vector< Serotonina::T_Precision > &out = network.Run( in );

			// Stampo i risultati
			std::cout.setf( std::ios::fixed, std::ios::floatfield );
			std::cout << "Run ( " << in[0] << " AND " << in[1] << " ) = " << out[0] << std::endl;
		}
	}

	return 0;
}

