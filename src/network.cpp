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

T_Precision
get_rand() {

	T_Precision x = (T_Precision) rand();
	T_Precision y = ( sin(x) * sin(x) );

	return y;
}

size_t
values_on_string( const std::string &str, std::vector< T_Precision > &vect ) {

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
			vect.push_back( (T_Precision) atof( tmp.c_str() ) );

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
Network::training_report(	Network *network, size_t epochs, T_Precision max_error,
							const T_Precision *outputs, size_t outputs_size, void *data ) {

	// Stampo il rapporto dell'addestramento 
	printf( "  Epochs #%zu, current error: %.10f\n", epochs, (double) max_error );

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
	this->output_data = new T_Precision[this->layers[this->layers.size() - 1]->size];

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

			// Inizializzo il peso sinaptico
			synapse_t->weight = get_rand();
		}
	}
}

const T_Precision *
Network::Run( const T_Precision *input ) {

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

		this->layers[0]->neurons[i].value = __BETWEEN__( input[i], 0.0, 1.0 );
	}

	// Attivazione interna del neurone
	T_Precision activation;

	// Calcolo i valori di uscita di ogni strato della rete
	for ( t = 0; t < this->layers.size() - 1; t++ ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->connections[t]->first_synapse;

		// Preparo l'iteratore dei neuroni
		last_neuron_i	= this->layers[t + 1]->last_neuron;
		neuron_i		= this->layers[t + 1]->first_neuron;

		// Calcolo i valori di uscita dello strato corrente
		// Y_i = T( SUM( w_ji - BIAS_i ) ) oppure Y_i = T( SUM( w_ji ) )
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
	return (const T_Precision *) this->output_data;
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

	file << std::endl;

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
	std::vector< T_Precision > values;

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
				this->output_data = new T_Precision[this->layers[this->layers.size() - 1]->size];

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

} // Chiudo il namespace di Serotonina

#endif
