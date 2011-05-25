/* 
    Title --- network.cpp

	Copyright (C) 2010 Giacomo Trudu - wicker25[at]gmail[dot]com

	This file is part of Serotonina.

	This library is free software; you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _SEROTONINA_NETWORK_CPP_
#define _SEROTONINA_NETWORK_CPP_

#include <serotonina/network.hpp>
#include <serotonina/network-inl.hpp>

namespace Serotonina { // Namespace di Serotonina

Precision
get_rand() {

	Precision x = (Precision) rand();
	Precision y = ( sin(x) * sin(x) );

	return y;
}

size_t
values_on_string( const std::string &str, std::vector< Precision > &vect ) {

	// Posizione del valore trovato
	std::string::size_type found = 0;

	// Sottostringa contenente i valori
	std::string tmp;

	// Numero di elementi inseriti
	size_t elements = 0;

	// Estraggo i valori numerici da una stringa di testo
	while ( found != std::string::npos ) {

		// Ricavo la sottostringa contenente il nuovo valore
		tmp = str.substr( found, str.find_first_of( ' ', found + 1 ) );

		// Controllo che la sottostringa non sia vuota
		if ( !tmp.empty() && tmp != " " ) {

			// Aggiungo il valore al vettore
			vect.push_back( (Precision) atof( tmp.c_str() ) );

			// Incremento l'indice dell'elemento
			elements++;
		}

		// Cerco il prossimo valore
		found = str.find_first_of( ' ', found + 1 );
	}

	// Ritorno il numero degli elementi inseriti
	return elements;
}

/** INIZIO METODI STATICI **/

int
Network::training_report(	Network *network, size_t epochs, Precision max_error,
							const Precision *outputs, size_t outputs_size, void *data ) {

	// Stampo il rapporto dell'addestramento 
	printf( "  Epochs #%zu, current error: %.10F\n", epochs, (double) max_error );

	return 0;
}

/** FINE METODI STATICI **/


Network::Network( size_t n_layers, ... ) {

	// Inizializzo il generatore di numeri pseudocasuali
	srand( (size_t) time( NULL ) );

	// Controllo siano stati scelti un minimo di due strati
	if ( n_layers < 2 ) {

		// Communico l'errore all'utente
		fprintf( stderr, "(W) Insufficient number of layers!\n" );
		
		// Termino
		exit(0);
	}

	// Lista dei parametri della funzione
	va_list args;

	// Inizio la lettura della lista dei parametri
	va_start( args, n_layers );

	// Creo gli strati della rete
	while( n_layers-- > 0 ) {

		this->layers.push_back( new Layer( va_arg( args, size_t ) ) );
	}   

	// Iteratore
	size_t t = 0;

	// Creo le connessioni tra gli strati
	for ( ; t < this->layers.size() - 1; t++ ) {

		this->connections.push_back( new LayerConnections( this->layers[t]->size + 1, this->layers[t + 1]->size ) );
	}

	// Creo il vettore contenente i dati di uscita
	this->output_data = new Precision[this->layers[this->layers.size() - 1]->size];

	#ifdef VERBOSE

		// Log di lavoro
		printf( "New neural network [%zu", this->layers[0]->size );

		for ( t = 1; t < this->layers.size(); t++ ) {

			printf( ",%zu", this->layers[t]->size );
		}

		printf( "].\n" );

	#endif

	// Termino la lettura della lista dei parametri
	va_end( args );

	// Imposto la funzione di report dell'addestramento
	this->report_function = &Network::training_report;

	// Imposto l'algoritmo di addestramento predefinito
	this->SetTrainingAlgorithm( TRAIN_IRPROP_MINUS );

	// Imposto il tasso di apprendimento e il momentum
	this->SetTrainingParameters( 0.5, 0.8 );

	// Imposto i fattori di incremento e decremento del RPROP
	this->SetRpropFactor( 1.2, 0.5 );
}

Network::Network( const std::string &path ) {

	// Carico la rete neurale dal file
	this->Load( path, true );
}

Network::~Network() {

	// Iteratore
	size_t i;

	// Elimino gli strati della rete
	for ( i = 0; i < this->layers.size(); i++ )
		delete this->layers[i];

	// Elimino le connessioni tra gli strati
	for ( i = 0; i < this->connections.size(); i++ )
		delete this->connections[i];

	// Elimino il vettore contenente i dati di uscita
	delete this->output_data;
}

void
Network::InizializeWeight() {

	// Iteratori
	size_t j, i;
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;

	// Reinizializzo i pesi sinaptici con valori casuali
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		for ( i = 0; i <= this->layers[t]->size; i++ ) {

			for ( j = 0; j < this->layers[t + 1]->size; j++, synapse_t++ ) {

				// Inizializzo il peso sinaptico
				synapse_t->weight = get_rand();
			}
		}
	}
}

const Precision *
Network::Run( const Precision *input ) {

	// Iteratore
	size_t i;
	short unsigned int t;

	// Iteratori dei neuroni
	Neuron *neuron_j, *last_neuron_j;
	Neuron *neuron_i, *last_neuron_i;

	// Iteratori delle sinapsi
	Synapse *synapse_t;

	// Carico i valori in ingresso
	for ( i = 0; i < this->layers[0]->size; i++ ) {

		this->layers[0]->neurons[i].value = input[i];
	}

	// Attivazione interna del neurone
	Precision activation;

	// Calcolo i valori di uscita di ogni strato della rete
	for ( t = 0; t < this->layers.size() - 1; t++ ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		// Preparo l'iteratore dei neuroni
		last_neuron_i	= this->layers[t + 1]->last_neuron;
		neuron_i		= this->layers[t + 1]->first_neuron;

		// Calcolo i valori di uscita dello strato corrente
		// Y_i = f( SUM( w_ji - BIAS_i ) ) oppure Y_i = f( SUM( w_ji ) )
		for ( i = 0; neuron_i <= last_neuron_i; i++, neuron_i++ ) {

			// Azzero il valore di attivazione del neurone
			activation = 0;

			// Preparo l'iteratore dei neuroni
			last_neuron_j	= this->layers[t]->last_neuron;
			neuron_j		= this->layers[t]->first_neuron;

			// Imposto il valore di attivazione come la somma pesata di tutti i neuroni in ingresso
			for ( ; neuron_j <= last_neuron_j; neuron_j++, synapse_t++ ) {

				activation += synapse_t->weight * neuron_j->value;
			}

			// Aggiungo la soglia del BIAS
			activation += synapse_t->weight;

			// Sposto l'iteratore oltre la sinapsi del BIAS, nella riga contente le sinapsi del neurone successivo
			synapse_t++;

			// Calcolo il valore in uscita del neurone utilizzando una sigmoide
			neuron_i->value = __SIGMOID__( activation );

			// Se sto processando l'ultimo strato copio i valori delle uscite nel vettore specifico
			if ( (size_t) ( t + 1 ) == this->layers.size() - 1 ) {

				this->output_data[i] = neuron_i->value;
			}
		}
	}

	// Ritorna un puntatore ai valori di uscita
	return (const Precision *) this->output_data;
}

/*******************************************************************
 *           FUNZIONI DI ADDESTRAMENTO DELLA RETE NEURALE          *
 *******************************************************************/

void
Network::ComputeError( const Precision *target ) {

	// Iteratori
	size_t j, i = 0;

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
	for ( i = 0; neuron_i <= last_neuron_i; i++, neuron_i++ ) {

		// Calcolo l'errore dell'uscita
		// dE/dy_i = -(T_i - Y_i)
		neuron_i->dEdy = - ( target[i] - neuron_i->value );

		// Preparo l'iteratore dei neuroni
		last_neuron_j	= this->layers[this->layers.size() - 2]->last_neuron;
		neuron_j		= this->layers[this->layers.size() - 2]->first_neuron;

		// Calcolo l'errore dei pesi sinaptici del neurone
		// dE/dw_ji += dE/dy_i * f'(net_i) * Y_j

		// Ciclo per tutti i neuroni del penultimo strato
		for ( j = 0; neuron_j <= last_neuron_j; j++, neuron_j++, synapse_t++ ) {

			synapse_t->dEdw += neuron_i->dEdy * __D_SIGMOID__( neuron_i->value ) * neuron_j->value;
		}

		// Aggiungo l'errore del BIAS ai pesi sinaptici del neurone
		// dE/dw_ji += dE/dy_i * f'(net_i)
		synapse_t->dEdw += neuron_i->dEdy * __D_SIGMOID__( neuron_i->value );

		// Sposto l'iteratore oltre la sinapsi del BIAS, nella riga contente le sinapsi del neurone successivo
		synapse_t++;

		// Aggiungo il quadrato dell'errore all'errore totale della rete (per il calcolo dell'errore quadratico medio)
		this->net_error += __POW2__( neuron_i->dEdy );
	}
}

void
Network::BackpropagateError() {

	// Errore da retropropagare
	Precision backpropagation;

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
				// dE/dy_j = SUM( dE/dy_k * f'(net_k) * w_jk )

				// Ciclo per tutti i neuroni dello strato 't + 2'
				for ( ; neuron_k <= last_neuron_k; neuron_k++, synapse_t1 += ( this->layers[t + 1]->size + 1 ) ) {

					backpropagation += neuron_k->dEdy * __D_SIGMOID__( neuron_k->value ) * synapse_t1->weight;
				}

				// Memorizzo l'errore del neurone
				neuron_j->dEdy = backpropagation;

				// Calcolo l'errore dei pesi sinaptici del neurone tenendo conto del BIAS
				if ( neuron_i == last_neuron_i + 1 )

					// dE/dw_ij += dE/dy_j * f'(net_j)
					synapse_t->dEdw += neuron_j->dEdy * __D_SIGMOID__( neuron_j->value );
				else
					// dE/dw_ij += dE/dy_j * f'(net_j) * Y_i
					synapse_t->dEdw += neuron_j->dEdy * __D_SIGMOID__( neuron_j->value ) * neuron_i->value;
			}
		}
	}
}

void
Network::UpdateWeightsBatch() {

	// Iteratori
	size_t j, i;
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando la Regola Delta con il Momentum
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		for ( i = 0; i <= this->layers[t]->size; i++ ) {

			for ( j = 0; j < this->layers[t + 1]->size; j++, synapse_t++ ) {

				// Calcolo la modifica del peso
				synapse_t->delta_weight = - this->learning_rate * synapse_t->dEdw + this->momentum * synapse_t->delta_weight;
	
				// Aggiorno il peso sinaptico
				synapse_t->weight += synapse_t->delta_weight;

				// Azzero l'errore del peso sinaptico
				synapse_t->dEdw = 0.0;
			}
		}
	}
}

void
Network::UpdateWeightsRprop() {

	// Iteratori
	size_t j, i;
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della RPROP
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		for ( i = 0; i <= this->layers[t]->size; i++ ) {

			for ( j = 0; j < this->layers[t + 1]->size; j++, synapse_t++ ) {

				// Imposto un tasso minimo di apprendimento (se fosse zero l'addestramento finirebbe)
				synapse_t->learning_rate = __MAX__( synapse_t->learning_rate, 0.0001 );

				// Calcolo la variazione della derivata rispetto all'epoca precedente
				Precision delta_sign = synapse_t->prev_dEdw * synapse_t->dEdw;

				// Applico le regole della RPROP+
				if ( delta_sign > 0.0 ) {

					// Incremento il tasso di apprendimento
					synapse_t->learning_rate = __MIN__( synapse_t->learning_rate * this->increase_factor, 50.0 );

					// Aggiorno il peso sinaptico
					synapse_t->weight += - __SIGN__( synapse_t->dEdw ) * synapse_t->learning_rate;

					// Memorizzo l'errore del peso sinaptico per il ciclo successivo
					synapse_t->prev_dEdw = synapse_t->dEdw;

				} else if ( delta_sign < 0.0 ) {

					// Decremento il tasso di apprendimento
					synapse_t->learning_rate = __MAX__( synapse_t->learning_rate * this->decrease_factor, 0.0 );

					// Memorizzo l'errore del peso sinaptico per il ciclo successivo
					synapse_t->prev_dEdw = 0.0;

				} else { // if ( delta_sign == 0.0 )

					// Aggiorno il peso sinaptico
					synapse_t->weight += - __SIGN__( synapse_t->dEdw ) * synapse_t->learning_rate;
					synapse_t->prev_dEdw = synapse_t->dEdw;
				}

				// Azzero l'errore del peso sinaptico
				synapse_t->dEdw = 0.0;
			}
		}
	}
}

void
Network::UpdateWeightsRpropPlus() {

	// Iteratori
	size_t j, i;
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della RPROP+
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		for ( i = 0; i <= this->layers[t]->size; i++ ) {

			for ( j = 0; j < this->layers[t + 1]->size; j++, synapse_t++ ) {

				// Imposto un tasso minimo di apprendimento (se fosse zero l'addestramento finirebbe)
				synapse_t->learning_rate = __MAX__( synapse_t->learning_rate, 0.0001 );

				// Calcolo la variazione della derivata rispetto all'epoca precedente
				Precision delta_sign = synapse_t->prev_dEdw * synapse_t->dEdw;

				// Applico le regole della RPROP+
				if ( delta_sign > 0.0 ) {

					// Incremento il tasso di apprendimento
					synapse_t->learning_rate = __MIN__( synapse_t->learning_rate * this->increase_factor, 50.0 );

					// Aggiorno il peso sinaptico
					synapse_t->delta_weight = - __SIGN__( synapse_t->dEdw ) * synapse_t->learning_rate;
					synapse_t->weight += synapse_t->delta_weight;

					// Memorizzo l'errore del peso sinaptico per il ciclo successivo
					synapse_t->prev_dEdw = synapse_t->dEdw;

				} else if ( delta_sign < 0.0 ) {

					// Decremento il tasso di apprendimento
					synapse_t->learning_rate = __MAX__( synapse_t->learning_rate * this->decrease_factor, 0.0 );

					// Ripristino il vecchio peso sinaptico precedente
					synapse_t->weight -= synapse_t->delta_weight;

					// Memorizzo l'errore del peso sinaptico per il ciclo successivo
					synapse_t->prev_dEdw = 0.0;

				} else { // if ( delta_sign == 0.0 )

					// Aggiorno il peso sinaptico
					synapse_t->delta_weight = - __SIGN__( synapse_t->dEdw ) * synapse_t->learning_rate;
					synapse_t->weight += synapse_t->delta_weight;

					// Memorizzo l'errore del peso sinaptico per il ciclo successivo
					synapse_t->prev_dEdw = synapse_t->dEdw;
				}

				// Azzero l'errore del peso sinaptico
				synapse_t->dEdw = 0.0;
			}
		}
	}
}

void
Network::UpdateWeightsRpropMinus() {

	// Iteratori
	size_t j, i;
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della RPROP-
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		for ( i = 0; i <= this->layers[t]->size; i++ ) {

			for ( j = 0; j < this->layers[t + 1]->size; j++, synapse_t++ ) {

				// Imposto un tasso minimo di apprendimento (se fosse zero l'addestramento finirebbe)
				synapse_t->learning_rate = __MAX__( synapse_t->learning_rate, 0.0001 );

				// Calcolo la variazione della derivata rispetto all'epoca precedente
				Precision delta_sign = synapse_t->prev_dEdw * synapse_t->dEdw;

				// Applico le regole della RPROP-
				if ( delta_sign > 0.0 ) {

					// Incremento il tasso di apprendimento
					synapse_t->learning_rate = __MIN__( synapse_t->learning_rate * this->increase_factor, 50.0 );

				} else if ( delta_sign < 0.0 ) {

					// Decremento il tasso di apprendimento
					synapse_t->learning_rate = __MAX__( synapse_t->learning_rate * this->decrease_factor, 0.0 );
				}

				// Aggiorno il peso sinaptico
				synapse_t->weight += - __SIGN__( synapse_t->dEdw ) * synapse_t->learning_rate;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->prev_dEdw = synapse_t->dEdw;

				// Azzero l'errore del peso sinaptico
				synapse_t->dEdw = 0.0;
			}
		}
	}
}

void
Network::UpdateWeightsIRpropPlus() {

	// Iteratori
	size_t j, i;
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della RPROP+
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		for ( i = 0; i <= this->layers[t]->size; i++ ) {

			for ( j = 0; j < this->layers[t + 1]->size; j++, synapse_t++ ) {

				// Imposto un tasso minimo di apprendimento (se fosse zero l'addestramento finirebbe)
				synapse_t->learning_rate = __MAX__( synapse_t->learning_rate, 0.0001 );

				// Calcolo la variazione della derivata rispetto all'epoca precedente
				Precision delta_sign = synapse_t->prev_dEdw * synapse_t->dEdw;

				// Applico le regole della RPROP+
				if ( delta_sign > 0.0 ) {

					// Incremento il tasso di apprendimento
					synapse_t->learning_rate = __MIN__( synapse_t->learning_rate * this->increase_factor, 50.0 );

					// Aggiorno il peso sinaptico
					synapse_t->delta_weight = - __SIGN__( synapse_t->dEdw ) * synapse_t->learning_rate;
					synapse_t->weight += synapse_t->delta_weight;

					// Memorizzo l'errore del peso sinaptico per il ciclo successivo
					synapse_t->prev_dEdw = synapse_t->dEdw;

				} else if ( delta_sign < 0.0 ) {

					// Decremento il tasso di apprendimento
					synapse_t->learning_rate = __MAX__( synapse_t->learning_rate * this->decrease_factor, 0.0 );

					// Se l'errore è aumentato, ripristino il vecchio peso sinaptico precedente
					if ( this->net_error > this->prev_net_error )
						synapse_t->weight -= synapse_t->delta_weight;

					// Memorizzo l'errore del peso sinaptico per il ciclo successivo
					synapse_t->prev_dEdw = 0.0;

				} else { // if ( delta_sign == 0.0 )

					// Aggiorno il peso sinaptico
					synapse_t->delta_weight = - __SIGN__( synapse_t->dEdw ) * synapse_t->learning_rate;
					synapse_t->weight += synapse_t->delta_weight;

					// Memorizzo l'errore del peso sinaptico per il ciclo successivo
					synapse_t->prev_dEdw = synapse_t->dEdw;
				}

				// Azzero l'errore del peso sinaptico
				synapse_t->dEdw = 0.0;
			}
		}
	}
}

void
Network::UpdateWeightsIRpropMinus() {

	// Iteratori
	size_t j, i;
	short int t = ( this->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando le regole della RPROP+
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		for ( i = 0; i <= this->layers[t]->size; i++ ) {

			for ( j = 0; j < this->layers[t + 1]->size; j++, synapse_t++ ) {

				// Imposto un tasso minimo di apprendimento (se fosse zero l'addestramento finirebbe)
				synapse_t->learning_rate = __MAX__( synapse_t->learning_rate, 0.0001 );

				// Calcolo la variazione della derivata rispetto all'epoca precedente
				Precision delta_sign = synapse_t->prev_dEdw * synapse_t->dEdw;

				// Applico le regole della RPROP-
				if ( delta_sign > 0.0 ) {

					// Incremento il tasso di apprendimento
					synapse_t->learning_rate = __MIN__( synapse_t->learning_rate * this->increase_factor, 50.0 );

				} else if ( delta_sign < 0.0 ) {

					// Decremento il tasso di apprendimento
					synapse_t->learning_rate = __MAX__( synapse_t->learning_rate * this->decrease_factor, 0.0 );

					// Azzero l'errore del peso sinaptico
					synapse_t->dEdw = 0.0;
				}

				// Aggiorno i pesi sinaptici
				synapse_t->weight += - __SIGN__( synapse_t->dEdw ) * synapse_t->learning_rate;

				// Memorizzo l'errore del peso sinaptico per il ciclo successivo
				synapse_t->prev_dEdw = synapse_t->dEdw;

				// Azzero l'errore del peso sinaptico
				synapse_t->dEdw = 0.0;
			}
		}
	}
}

void
Network::Train(	const Precision *input_samples, const Precision *output_samples, size_t n_samples,
				Precision target_error, size_t max_epochs, size_t epochs_between_reports ) {

	#ifdef VERBOSE

		// Log di lavoro
		printf( "Start training with %zu samples.\n", n_samples );

	#endif

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
			int flag = this->report_function(	this, epochs, this->net_error, (const Precision *) this->output_data,
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
		this->report_function(	this, epochs, this->net_error, (const Precision *) this->output_data,
								this->layers[this->layers.size() - 1]->size, this->report_function_data );
	}
}

void
Network::TrainOnFile( const std::string &path, Precision target_error, size_t max_epochs, size_t epochs_between_reports ) {

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
	std::vector< Precision > inputs;

	// Valori d'uscita
	std::vector< Precision > outputs;

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
			std::vector< Precision > info;

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

/*******************************************************************
 *         FUNZIONI DI SALVATAGGIO E CARICAMENTO DELLE RETI        *
 *******************************************************************/

void
Network::Save( const std::string &path ) {

	// Apre uno stream al file di destinazione
	std::ofstream file( path.c_str() );

	// Controllo se il file è stato aperto correttamente
	if ( !file.is_open() ) {

		// Communico l'errore all'utente
		fprintf( stderr, "(W) Couldn't open file '%s'!\n", path.c_str() );

		// Termino l'esecuzione del programma
		exit(1);
	}

	// Imposto la precisione dei pesi
	file.setf( std::ios::fixed, std::ios::floatfield );
	file.precision( 10 );

	// Inserisco l'intestazione del file
	file << "# Rete neurale" << std::endl;

	// Iteratore
	size_t t = 0;

	// Inserisco le informazioni generali della rete
	for ( ; t < this->layers.size(); t++ ) {

		file << this->layers[t]->size << " ";
	}

	file <<  std::endl;

	// Iteratori delle sinapsi
	Synapse *synapse_i, *last_synapse_i;

	// Copio tutte le sinapsi tra gli strati
	for ( t = 0; t < this->connections.size(); t++ ) {

		// Inserisco l'intestazione dei pesi sinaptici
		file << std::endl << "# Pesi sinaptici dello strato " << t << std::endl;

		// Preparo l'iteratore delle sinapsi
		last_synapse_i	= this->connections[t]->last_synapse;
		synapse_i		= this->connections[t]->first_synapse;

		// Inserisco i pesi sinaptici dello strato t
		for ( ; synapse_i <= last_synapse_i; synapse_i++ ) {

			file << synapse_i->weight << " ";
		}
	}


	// Insersco una linea vuota
	file << std::endl;

	// Chiudo lo stream al file
	file.close();

	#ifdef VERBOSE

		// Log di lavoro
		printf( "Neural network saved.\n" );

	#endif
}

void
Network::Load( const std::string &path, bool new_ ) {

	// Controllo se la rete era già stata creata precedentemente
	if ( !new_ ) {

		// Iteratore
		size_t i;

		// Elimino gli strati della rete
		for ( i = 0; i < this->layers.size(); i++ )
			delete this->layers[i];

		// Elimino le connessioni tra gli strati
		for ( i = 0; i < this->connections.size(); i++ )
			delete this->connections[i];

		// Elimino il vettore contenente i dati di uscita
		delete this->output_data;

		#ifdef VERBOSE

			// Log di lavoro
			printf( "Old neural network deleted.\n" );

		#endif
	}

	// Apre uno stream al file sorgente
	std::ifstream file( path.c_str() );

	// Controllo se il file è stato aperto correttamente
	if ( !file.is_open() ) {

		// Communico l'errore all'utente
		fprintf( stderr, "(W) Couldn't open file '%s'!\n", path.c_str() );

		// Termino l'esecuzione del programma
		exit(1);
	}

	// Vettore con i valori estratti dalla riga
	std::vector< Precision > values;

	// Flag di uscita
	bool end = false;

	// Numero della riga letta
	size_t n_line = 0;

	// Buffer contenente la riga letta
	std::string line;

	// Leggo le informazioni generali della rete
	while ( !end && file.good() ) {

		// Leggo la nuova riga
		std::getline( file, line );

		// Controllo che la linea non sia vuota o un commento
		if ( !line.empty() && line.at(0) != _COMMENT_ ) {

			// Estraggo i valori di uscita
			if ( values_on_string( line, values ) >= 2 ) {

				// Iteratore
				size_t t = 0;

				// Creo gli strati della rete
				for ( ; t < values.size(); t++ ) {

					this->layers.push_back( new Layer( values[t] ) );
				}

				// Creo le connessioni tra gli strati
				for ( t = 0; t < this->layers.size() - 1; t++ ) {

					this->connections.push_back( new LayerConnections( this->layers[t]->size + 1, this->layers[t + 1]->size ) );
				}

				// Creo il vettore contenente i dati di uscita
				this->output_data = new Precision[this->layers[this->layers.size() - 1]->size];

				// Imposto la funzione di report dell'addestramento
				this->report_function = &Network::training_report;

				#ifdef VERBOSE

					// Log di lavoro
					printf( "New neural network [%zu", this->layers[0]->size );

					for ( t = 1; t < this->layers.size(); t++ ) {

						printf( ",%zu", this->layers[t]->size );
					}

					printf( "].\n" );

				#endif

				// Pulisco il vettore dei valori
				values.clear();

				// Imposto il flag di uscita
				end = true;

			} else {

				// E se trovo un errore lo communico all'utente
				fprintf( stderr, " (W) Syntax error on training file '%s', at line %zu: Insufficient number of layers!\n", path.c_str(), n_line );
			}

			// Incremento il contatore della linea
			n_line++;
		}
	}

	// Iteratori
	size_t i, t = 0;

	// Iteratori delle sinapsi
	Synapse *synapse_i, *last_synapse_i;

	// Leggo le sinapsi di tutti gli strati
	while ( t < this->connections.size() && file.good() ) {

		// Leggo la nuova riga
		std::getline( file, line );

		// Controllo che la linea non sia vuota o un commento
		if ( !line.empty() && line.at(0) != _COMMENT_ ) {

			// Estraggo i valori di uscita
			if ( values_on_string( line, values ) == this->connections[t]->size ) {

				// Preparo l'iteratore delle sinapsi
				last_synapse_i	= this->connections[t]->last_synapse;
				synapse_i		= this->connections[t]->first_synapse;

				// Copio i pesi sinaptici dello strato t
				for ( i = 0; synapse_i <= last_synapse_i; i++, synapse_i++ ) {

					synapse_i->weight = values[i];
				}

				// Pulisco il vettore dei valori
				values.clear();

				// Incremento il contatore delle connessioni
				t++;

			} else {

				// E se trovo un errore lo communico all'utente
				fprintf( stderr, " (W) Syntax error on training file '%s', at line %zu!\n", path.c_str(), n_line );
			}

			// Incremento il contatore della linea
			n_line++;
		}
	}

	// Chiudo lo stream al file
	file.close();

	// Imposto la funzione di report dell'addestramento
	this->report_function = &Network::training_report;

	// Imposto l'algoritmo di addestramento predefinito
	this->SetTrainingAlgorithm( TRAIN_IRPROP_MINUS );

	// Imposto il tasso di apprendimento e il momentum
	this->SetTrainingParameters( 0.5, 0.8 );

	// Imposto i fattori di incremento e decremento del RPROP
	this->SetRpropFactor( 1.2, 0.5 );

	#ifdef VERBOSE

		// Log di lavoro
		printf( "Neural network loaded.\n" );

	#endif
}

}; // Chiudo il namespace di Serotonina

#endif
