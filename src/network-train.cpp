/* 
    Title --- network-train.cpp

	Copyright (C) 2010 Giacomo Trudu - wicker25[at]gmail[dot]com

	This file is part of Serotonina.

    Serotonina is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Serotonina is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Serotonina.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SEROTONINA_NETWORK_TRAIN_CPP_
#define _SEROTONINA_NETWORK_TRAIN_CPP_

#include <serotonina/network.hpp>
#include <serotonina/network-inl.hpp>

namespace Serotonina { // Namespace di Serotonina

/*******************************************************************
 *           FUNZIONI DI ADDESTRAMENTO DELLA RETE NEURALE          *
 *******************************************************************/

void
Network::ComputeError( const T_Precision *target ) {

	// Iteratore
	size_t i = 0;

	// Iteratori dei neuroni
	Neuron *neuron_j, *last_neuron_j;
	Neuron *neuron_i, *last_neuron_i;

	// Iteratori delle sinapsi
	Synapse *synapse_t;

	// Preparo l'iteratore dei neuroni
	last_neuron_i	= this->layers[this->layers.size() - 1]->last_neuron;
	neuron_i		= this->layers[this->layers.size() - 1]->first_neuron;

	// Preparo l'iteratore delle sinapsi
	synapse_t = this->connections[this->layers.size() - 2]->first_synapse;

	// Calcolo l'errore dello strato di uscita
	for ( ; neuron_i <= last_neuron_i; i++, neuron_i++ ) {

		// Calcolo l'errore dell'uscita
		// dE/dy_i = -(D_i - Y_i)
		neuron_i->dEdy = - ( target[i] - neuron_i->value );

		// Preparo l'iteratore dei neuroni
		last_neuron_j	= this->layers[this->layers.size() - 2]->last_neuron;
		neuron_j		= this->layers[this->layers.size() - 2]->first_neuron;

		// Calcolo l'errore dei pesi sinaptici del neurone
		// dE/dw_ji += dE/dy_i * dy_i/dP_i * Y_j

		// Ciclo per tutti i neuroni del penultimo strato
		for ( ; neuron_j <= last_neuron_j; neuron_j++, synapse_t++ ) {

			synapse_t->train->dEdw += neuron_i->dEdy * __D_SIGMOID__( neuron_i->value ) * neuron_j->value;
		}

		// Aggiungo l'errore del BIAS ai pesi sinaptici del neurone
		// dE/dw_ji += dE/dy_i * dy_i/dP_i
		synapse_t->train->dEdw += neuron_i->dEdy * __D_SIGMOID__( neuron_i->value );

		// Sposto l'iteratore oltre la sinapsi del BIAS, nella riga contente le sinapsi del neurone successivo
		synapse_t++;

		// Aggiungo il quadrato dell'errore all'errore totale della rete (per il calcolo dell'errore quadratico medio)
		this->net_error += __POW2__( neuron_i->dEdy );
	}
}

void
Network::BackpropagateError() {

	// Errore da retropropagare
	T_Precision backpropagation;

	// Iteratori
	short int t = ( (short int) this->layers.size() - 1 ) - 2;

	// Iteratori delle sinapsi
	Synapse *synapse_row_t, *synapse_t;
	Synapse *synapse_row_t1, *synapse_t1;

	// Iteratori dei neuroni
	Neuron *neuron_j, *last_neuron_j;
	Neuron *neuron_i, *last_neuron_i;
	Neuron *neuron_k, *last_neuron_k;

	// Retropropago l'errore nella rete
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_row_t = this->connections[t]->first_synapse;

		// Preparo l'iteratore dei neuroni
		last_neuron_i	= this->layers[t]->last_neuron;
		neuron_i		= this->layers[t]->first_neuron;

		// Ciclo per tutti i neuroni dello strato 't'
		for ( ; neuron_i <= ( last_neuron_i + 1 ); neuron_i++, synapse_row_t++ ) {

			// Preparo l'iteratore delle sinapsi
			synapse_t = synapse_row_t;

			// Preparo l'iteratore delle sinapsi
			synapse_row_t1 = this->connections[t + 1]->first_synapse;

			// Preparo l'iteratore dei neuroni
			last_neuron_j	= this->layers[t + 1]->last_neuron;
			neuron_j		= this->layers[t + 1]->first_neuron;

			// Ciclo per tutti i neuroni dello strato 't + 1'
			for ( ; neuron_j <= last_neuron_j; neuron_j++, synapse_row_t1++, synapse_t += ( this->layers[t]->size + 1 ) ) {

				// Azzero l'errore da retropropagare
				backpropagation = 0;

				// Preparo l'iteratore delle sinapsi
				synapse_t1 = synapse_row_t1;

				// Preparo l'iteratore dei neuroni
				last_neuron_k	= this->layers[t + 2]->last_neuron;
				neuron_k		= this->layers[t + 2]->first_neuron;

				// Calcolo l'errore retropropagato dai neuroni dello strato successivo
				// dE/dz_k = SUM( dE/dy_j * dy_j/dP_j * dP_j/dz_k )

				// Ciclo per tutti i neuroni dello strato 't + 2'
				for ( ; neuron_k <= last_neuron_k; neuron_k++, synapse_t1 += ( this->layers[t + 1]->size + 1 ) ) {

					backpropagation += neuron_k->dEdy * __D_SIGMOID__( neuron_k->value ) * synapse_t1->weight;
				}

				// Memorizzo l'errore del neurone
				neuron_j->dEdy = backpropagation;

				// Calcolo l'errore dei pesi sinaptici del neurone tenendo conto del BIAS
				if ( neuron_i == last_neuron_i + 1 )

					// dE/dw_ij += dE/dy_j * dy_j/dP_j
					synapse_t->train->dEdw += neuron_j->dEdy * __D_SIGMOID__( neuron_j->value );
				else
					// dE/dw_ij += dE/dy_j * dy_j/dP_j * Z_i
					synapse_t->train->dEdw += neuron_j->dEdy * __D_SIGMOID__( neuron_j->value ) * neuron_i->value;
			}
		}
	}
}

void
Network::CreateTrainData() {

	// Iteratori
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Creo le strutture necessarie all'addestramento
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->connections[t]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Creo la struttura per l'addestramento
			synapse_t->train = new TrainData;
		}
	}
}

void
Network::DeleteTrainData() {

	// Iteratori
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Cancello le strutture necessarie all'addestramento
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->connections[t]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Cancello la struttura per l'addestramento
			delete synapse_t->train;
		}
	}
}

void
Network::UpdateWeightsBatch() {

	// Iteratori
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando la Regola Delta con il Momentum
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->connections[t]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Calcolo la modifica del peso
			synapse_t->train->delta_weight = - this->learning_rate * synapse_t->train->dEdw + this->momentum * synapse_t->train->delta_weight;

			// Aggiorno il peso sinaptico
			synapse_t->weight += synapse_t->train->delta_weight;

			// Azzero l'errore del peso sinaptico
			synapse_t->train->dEdw = 0.0;
		}
	}
}

void
Network::UpdateWeightsRprop() {

	// Iteratori
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della RPROP
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->connections[t]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Imposto un tasso minimo di apprendimento (se fosse zero l'addestramento finirebbe)
			synapse_t->train->learning_rate = __MAX__( synapse_t->train->learning_rate, 0.0001 );

			// Calcolo la variazione della derivata rispetto all'epoca precedente
			T_Precision delta_sign = synapse_t->train->prev_dEdw * synapse_t->train->dEdw;

			// Applico le regole della RPROP
			if ( delta_sign > 0.0 ) {

				// Incremento il tasso di apprendimento
				synapse_t->train->learning_rate = __MIN__( synapse_t->train->learning_rate * this->increase_factor, 50.0 );

				// Aggiorno il peso sinaptico
				synapse_t->weight += - __SIGN__( synapse_t->train->dEdw ) * synapse_t->train->learning_rate;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->train->prev_dEdw = synapse_t->train->dEdw;

			} else if ( delta_sign < 0.0 ) {

				// Decremento il tasso di apprendimento
				synapse_t->train->learning_rate = __MAX__( synapse_t->train->learning_rate * this->decrease_factor, 0.0 );

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->train->prev_dEdw = 0.0;

			} else { // if ( delta_sign == 0.0 )

				// Aggiorno il peso sinaptico
				synapse_t->weight += - __SIGN__( synapse_t->train->dEdw ) * synapse_t->train->learning_rate;
				synapse_t->train->prev_dEdw = synapse_t->train->dEdw;
			}

			// Azzero l'errore del peso sinaptico
			synapse_t->train->dEdw = 0.0;
		}
	}
}

void
Network::UpdateWeightsRpropPlus() {

	// Iteratori
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della RPROP+
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->connections[t]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Imposto un tasso minimo di apprendimento (se fosse zero l'addestramento finirebbe)
			synapse_t->train->learning_rate = __MAX__( synapse_t->train->learning_rate, 0.0001 );

			// Calcolo la variazione della derivata rispetto all'epoca precedente
			T_Precision delta_sign = synapse_t->train->prev_dEdw * synapse_t->train->dEdw;

			// Applico le regole della RPROP+
			if ( delta_sign > 0.0 ) {

				// Incremento il tasso di apprendimento
				synapse_t->train->learning_rate = __MIN__( synapse_t->train->learning_rate * this->increase_factor, 50.0 );

				// Aggiorno il peso sinaptico
				synapse_t->train->delta_weight = - __SIGN__( synapse_t->train->dEdw ) * synapse_t->train->learning_rate;
				synapse_t->weight += synapse_t->train->delta_weight;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->train->prev_dEdw = synapse_t->train->dEdw;

			} else if ( delta_sign < 0.0 ) {

				// Decremento il tasso di apprendimento
				synapse_t->train->learning_rate = __MAX__( synapse_t->train->learning_rate * this->decrease_factor, 0.0 );

				// Ripristino il vecchio peso sinaptico precedente
				synapse_t->weight -= synapse_t->train->delta_weight;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->train->prev_dEdw = 0.0;

			} else { // if ( delta_sign == 0.0 )

				// Aggiorno il peso sinaptico
				synapse_t->train->delta_weight = - __SIGN__( synapse_t->train->dEdw ) * synapse_t->train->learning_rate;
				synapse_t->weight += synapse_t->train->delta_weight;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->train->prev_dEdw = synapse_t->train->dEdw;
			}

			// Azzero l'errore del peso sinaptico
			synapse_t->train->dEdw = 0.0;
		}
	}
}

void
Network::UpdateWeightsRpropMinus() {

	// Iteratori
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della RPROP-
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->connections[t]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {
			// Imposto un tasso minimo di apprendimento (se fosse zero l'addestramento finirebbe)
			synapse_t->train->learning_rate = __MAX__( synapse_t->train->learning_rate, 0.0001 );

			// Calcolo la variazione della derivata rispetto all'epoca precedente
			T_Precision delta_sign = synapse_t->train->prev_dEdw * synapse_t->train->dEdw;

			// Applico le regole della RPROP-
			if ( delta_sign > 0.0 ) {

				// Incremento il tasso di apprendimento
				synapse_t->train->learning_rate = __MIN__( synapse_t->train->learning_rate * this->increase_factor, 50.0 );

			} else if ( delta_sign < 0.0 ) {

				// Decremento il tasso di apprendimento
				synapse_t->train->learning_rate = __MAX__( synapse_t->train->learning_rate * this->decrease_factor, 0.0 );
			}

			// Aggiorno il peso sinaptico
			synapse_t->weight += - __SIGN__( synapse_t->train->dEdw ) * synapse_t->train->learning_rate;

			// Memorizzo l'errore del peso sinaptico per il ciclo successivo
			synapse_t->train->prev_dEdw = synapse_t->train->dEdw;

			// Azzero l'errore del peso sinaptico
			synapse_t->train->dEdw = 0.0;
		}
	}
}

void
Network::UpdateWeightsIRpropPlus() {

	// Iteratori
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della IRPROP+
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->connections[t]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Imposto un tasso minimo di apprendimento (se fosse zero l'addestramento finirebbe)
			synapse_t->train->learning_rate = __MAX__( synapse_t->train->learning_rate, 0.0001 );

			// Calcolo la variazione della derivata rispetto all'epoca precedente
			T_Precision delta_sign = synapse_t->train->prev_dEdw * synapse_t->train->dEdw;

			// Applico le regole della IRPROP+
			if ( delta_sign > 0.0 ) {

				// Incremento il tasso di apprendimento
				synapse_t->train->learning_rate = __MIN__( synapse_t->train->learning_rate * this->increase_factor, 50.0 );

				// Aggiorno il peso sinaptico
				synapse_t->train->delta_weight = - __SIGN__( synapse_t->train->dEdw ) * synapse_t->train->learning_rate;
				synapse_t->weight += synapse_t->train->delta_weight;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->train->prev_dEdw = synapse_t->train->dEdw;

			} else if ( delta_sign < 0.0 ) {

				// Decremento il tasso di apprendimento
				synapse_t->train->learning_rate = __MAX__( synapse_t->train->learning_rate * this->decrease_factor, 0.0 );

				// Se l'errore è aumentato, ripristino il vecchio peso sinaptico precedente
				if ( this->net_error > this->prev_net_error )
					synapse_t->weight -= synapse_t->train->delta_weight;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->train->prev_dEdw = 0.0;

			} else { // if ( delta_sign == 0.0 )

				// Aggiorno il peso sinaptico
				synapse_t->train->delta_weight = - __SIGN__( synapse_t->train->dEdw ) * synapse_t->train->learning_rate;
				synapse_t->weight += synapse_t->train->delta_weight;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->train->prev_dEdw = synapse_t->train->dEdw;
			}

			// Azzero l'errore del peso sinaptico
			synapse_t->train->dEdw = 0.0;
		}
	}
}

void
Network::UpdateWeightsIRpropMinus() {

	// Iteratori
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della IRPROP-
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->connections[t]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Imposto un tasso minimo di apprendimento (se fosse zero l'addestramento finirebbe)
			synapse_t->train->learning_rate = __MAX__( synapse_t->train->learning_rate, 0.0001 );

			// Calcolo la variazione della derivata rispetto all'epoca precedente
			T_Precision delta_sign = synapse_t->train->prev_dEdw * synapse_t->train->dEdw;

			// Applico le regole della IRPROP-
			if ( delta_sign > 0.0 ) {

				// Incremento il tasso di apprendimento
				synapse_t->train->learning_rate = __MIN__( synapse_t->train->learning_rate * this->increase_factor, 50.0 );

			} else if ( delta_sign < 0.0 ) {

				// Decremento il tasso di apprendimento
				synapse_t->train->learning_rate = __MAX__( synapse_t->train->learning_rate * this->decrease_factor, 0.0 );

				// Azzero l'errore del peso sinaptico
				synapse_t->train->dEdw = 0.0;
			}

			// Aggiorno i pesi sinaptici
			synapse_t->weight += - __SIGN__( synapse_t->train->dEdw ) * synapse_t->train->learning_rate;

			// Memorizzo l'errore del peso sinaptico per il ciclo successivo
			synapse_t->train->prev_dEdw = synapse_t->train->dEdw;

			// Azzero l'errore del peso sinaptico
			synapse_t->train->dEdw = 0.0;
		}
	}
}

void
Network::Train(	const T_Precision *input_samples, const T_Precision *output_samples, size_t n_samples,
				T_Precision target_error, size_t max_epochs, size_t epochs_between_reports ) {

	#ifdef VERBOSE

		// Log di lavoro
		printf( "Start training with %zu samples.\n", n_samples );

	#endif

	// Creo le strutture per l'addestramento
	this->CreateTrainData();

	// Numero delle epoche dell'addestramento
	size_t epochs = 0;

	// Iteratori
	size_t i;

	// Addestro la rete neurale usando gli esempi
	do {

		// Memorizzo l'errore precedente
		this->prev_net_error = this->net_error;

		// Azzero l'errore corrente
		this->net_error = 0;

		// Ripeto per ogni esempio
		for ( i = 0; i < n_samples; i++ ) {

			// Eseguo la rete neurale
			this->Run( &input_samples[ i * this->layers[0]->size ] );

			// Calcolo l'errore delle uscite
			this->ComputeError( &output_samples[ i * this->layers[this->layers.size() - 1]->size ] );

			// Retropropago l'errore nella rete
			this->BackpropagateError();
		}

		// Calcolo l'errore quadratico medio della rete (MSE)
		// E(x) = SUM( e^2 ) / n_samples
		this->net_error /= ( this->layers[this->layers.size() - 1]->size * n_samples );

		// Controllo se non si è raggiunto l'errore desiderato
		if ( this->net_error > target_error ) {

			// Controllo l'algoritmo di apprendimento scelto
			switch ( this->train_algorithm ) {

				// Aggiorno i pesi della rete utilizzando la Regola Delta con il Momentum
				case TRAIN_BATCH: { this->UpdateWeightsBatch(); break; }

				// Aggiorno i pesi della rete utilizzando la RPROP
				case TRAIN_RPROP: { this->UpdateWeightsRprop(); break; }

				// Aggiorno i pesi della rete utilizzando la RPROP+
				case TRAIN_RPROP_PLUS: { this->UpdateWeightsRpropPlus(); break; }

				// Aggiorno i pesi della rete utilizzando la RPROP+
				case TRAIN_RPROP_MINUS: { this->UpdateWeightsRpropMinus(); break; }

				// Aggiorno i pesi della rete utilizzando la IRPROP+
				case TRAIN_IRPROP_PLUS: { this->UpdateWeightsIRpropPlus(); break; }

				// Aggiorno i pesi della rete utilizzando la IRPROP+
				case TRAIN_IRPROP_MINUS: { this->UpdateWeightsIRpropMinus(); break; }

				default: break;
			}
		} 

		// Log dell'addestramento ogni N epoche
		if ( epochs % epochs_between_reports == 0 ) {

			// Passo le informazioni alla funzione di report
			int flag = this->report_function(	this, epochs, this->net_error, (const T_Precision *) this->output_data,
												this->layers[this->layers.size() - 1]->size, this->report_function_data );

			// Controllo se è stato inviato un segnale di uscita
			if ( flag ) break;
		}

		// Incremento il contatore delle epoche
		epochs++;

	// Se non si è raggiunto l'errore massimo voluto e le epoche sono entro i limiti continuo l'addestramento
	} while ( this->net_error > target_error && epochs < max_epochs );


	// Report conclusivo
	if ( epochs % epochs_between_reports != 0 ) {

		// Passo le informazioni alla funzione di report
		this->report_function(	this, epochs, this->net_error, (const T_Precision *) this->output_data,
								this->layers[this->layers.size() - 1]->size, this->report_function_data );
	}

	// Cancello le strutture per l'addestramento
	this->DeleteTrainData();
}

void
Network::TrainOnFile( const std::string &path, T_Precision target_error, size_t max_epochs, size_t epochs_between_reports ) {

	// Apre uno stream al file dell'addestramento
	std::ifstream file( path.c_str() );

	// Controllo se il file è stato aperto correttamente
	if ( !file.is_open() ) {

		// Communico l'errore all'utente
		fprintf( stderr, "(W) Couldn't open file '%s'!\n", path.c_str() );

		// Termino l'esecuzione del programma
		exit(1);
	}

	// Valori d'ingresso
	std::vector< T_Precision > inputs;

	// Valori d'uscita
	std::vector< T_Precision > outputs;

	// Numero di campioni caricati
	size_t n_samples = 0;

	// Numero della riga letta
	size_t n_line = 0;

	// Flag di uscita
	bool end = false;

	// Buffer contenente la riga letta
	std::string line;

	// Leggo le informazioni sul tipo di addestramento
	while ( !end && file.good() ) {

		// Leggo la nuova riga
		std::getline( file, line );

		// Controllo che la linea non sia vuota o un commento
		if ( !line.empty() && line.at(0) != _COMMENT_ ) {

			// Vettore contenente i valori estratti
			std::vector< T_Precision > info;

			if ( values_on_string( line, info ) != 2 ) {

				// E se trovo un errore lo communico all'utente
				fprintf( stderr, " (W) Syntax error on training file '%s', at line %zu!\n", path.c_str(), n_line );
			}

			// Controllo se il tipo di addestramento non è adatto alla rete corrente
			if ( this->layers[0]->size != (size_t) info[0] || this->layers[this->layers.size() - 1]->size != (size_t) info[1] ) {

				// Communico l'errore all'utente
				fprintf( stderr, " (W) Inadequate training file '%s'!\n", path.c_str() );
			}

			// Imposto il flag di uscita
			end = true;
		}

		// Incremento il contatore della linea
		n_line++;
	}

	// Leggo i dati per l'addestramento una riga per volta
	while ( file.good() ) {

		// Leggo la nuova riga
		std::getline( file, line );

		// Controllo che la linea non sia vuota o un commento
		if ( !line.empty() && line.at(0) != _COMMENT_ ) {

			// Cerco il separatore nella riga
			int found = line.find_first_of( _SEPARATOR_ );

			// Estraggo i valori di ingresso
			if ( values_on_string( line.substr( 0, found ), inputs ) != this->layers[0]->size ) {

				// E se trovo un errore lo communico all'utente
				fprintf( stderr, " (W) Syntax error on training file '%s', at line %zu!\n", path.c_str(), n_line );
			}

			// Estraggo i valori di uscita
			if ( values_on_string( line.substr( found + 1 ), outputs ) != this->layers[this->layers.size() - 1]->size ) {

				// E se trovo un errore lo communico all'utente
				fprintf( stderr, " (W) Syntax error on training file '%s', at line %zu!\n", path.c_str(), n_line );
			}

			// Incremento il numero dei campioni caricati
			n_samples++;

			// Incremento il contatore della linea
			n_line++;
		}
	}

	// Chiudo lo stream al file
	file.close();

	// Richiama la funzione di addestramento principale usando i campioni del file
	this->Train( &inputs[0], &outputs[0], n_samples, target_error, max_epochs, epochs_between_reports );
}

} // Chiudo il namespace di Serotonina

#endif
