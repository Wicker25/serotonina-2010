/* 
    Title --- network.cpp

    Copyright (C) 2010 Giacomo Trudu - wicker25[at]gmail[dot]com

    This file is part of Serotonina.

    Serotonina is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    Serotonina is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Serotonina.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SEROTONINA_NETWORK_CPP_
#define _SEROTONINA_NETWORK_CPP_

#include <serotonina/network.hpp>
#include <serotonina/network-inl.hpp>

namespace Serotonina { // Namespace di Serotonina

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

	// Numero dei neuroni dello strato
	size_t n_neurons;

	// Lista dei parametri della funzione
	va_list args;

	// Inizio la lettura della lista dei parametri
	va_start( args, n_layers );

	// Ricavo il numero dei neuroni dello strato di ingresso
	n_neurons = va_arg( args, size_t );

	// Creo lo strato di ingresso
	this->layers.push_back( new Layer( n_neurons, 0 ) );

	// Creo gli strati della rete
	while( n_layers-- > 1 ) {

		// Ricavo il numero dei neuroni dello strato
		n_neurons = va_arg( args, size_t );

		// Creo lo strato
		this->layers.push_back( new Layer( n_neurons, this->layers.back()->n_neurons ) );
	}   

	// Inizializzo i pesi sinaptici
	this->InizializeWeight();

	// Creo il vettore contenente i dati di uscita
	this->output_data = new T_Precision[this->layers.back()->n_neurons];

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
	size_t i = 0;

	// Elimino gli strati della rete
	for ( ; i < this->layers.size(); i++ )
		delete this->layers[i];

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
		synapse_t = this->layers[t + 1]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->layers[t + 1]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Inizializzo il peso sinaptico
			synapse_t->weight = rand0to1< T_Precision >();
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
	for ( i = 0; i < this->layers.front()->n_neurons; i++ ) {

		this->layers.front()->neurons[i].value = between( input[i], 0.0, 1.0 );
	}

	// Attivazione interna del neurone
	T_Precision activation;

	// Calcolo i valori di uscita di ogni strato della rete
	for ( t = 0; t < this->layers.size() - 1; t++ ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->layers[t + 1]->first_synapse;

		// Preparo l'iteratore dei neuroni
		last_neuron_i	= this->layers[t + 1]->last_neuron;
		neuron_i		= this->layers[t + 1]->first_neuron;

		// Calcolo i valori di uscita dello strato corrente
		// Y_i = T( SUM( w_ji - bias_i ) ) oppure Y_i = T( SUM( w_ji ) )
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

			// Aggiungo la soglia del bias
			activation += synapse_t->weight;

			// Sposto l'iteratore oltre la sinapsi del bias, nella riga contente le sinapsi del neurone successivo
			synapse_t++;

			// Calcolo il valore in uscita del neurone utilizzando una sigmoide
			neuron_i->value = sigmoid( activation );
		}
	}

	// Preparo l'iteratore dei neuroni
	last_neuron_i	= this->layers.back()->last_neuron;
	neuron_i		= this->layers.back()->first_neuron;

	// Copio le uscite nel vettore specifico
	for ( i = 0; neuron_i <= last_neuron_i; i++, neuron_i++ ) {

		// Copio l'uscita del neurone
		this->output_data[i] = neuron_i->value;
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

		file << this->layers[t]->n_neurons << " ";
	}

	file << std::endl;

	// Iteratori delle sinapsi
	Synapse *synapse_i, *last_synapse_i;

	// Copio tutte le sinapsi tra gli strati
	for ( t = 1; t < this->layers.size(); t++ ) {

		// Inserisco l'intestazione dei pesi sinaptici
		file << std::endl << "# Pesi sinaptici dello strato " << t << std::endl;

		// Preparo l'iteratore delle sinapsi
		last_synapse_i	= this->layers[t]->last_synapse;
		synapse_i		= this->layers[t]->first_synapse;

		// Inserisco i pesi sinaptici dello strato t
		for ( ; synapse_i <= last_synapse_i; synapse_i++ ) {

			file << synapse_i->weight << " ";
		}
	}


	// Insersco una linea vuota
	file << std::endl;

	// Chiudo lo stream al file
	file.close();
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

		// Elimino il vettore contenente i dati di uscita
		delete this->output_data;
	}

	// Apre uno stream al file sorgente
	std::ifstream file( path.c_str() );

	// Controllo se il file è stato aperto correttamente
	if ( !file.is_open() ) {

		// Communico l'errore all'utente
		fprintf( stderr, "(W) Couldn't read from file '%s'!\n", path.c_str() );

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
		if ( !line.empty() && line.at(0) != _SEROTONINA_COMMENT_ ) {

			// Estraggo i valori che descrivono la struttura della rete
			if ( values_from_string( line, values ) >= 2 ) {

				// Iteratore
				size_t t;

				// Creo lo strato di ingresso
				this->layers.push_back( new Layer( values[0], 0 ) );

				// Creo gli strati della rete
				for ( t = 1; t < values.size(); t++ ) {

					this->layers.push_back( new Layer( values[t], values[t - 1] ) );
				}

				// Inizializzo i pesi sinaptici
				this->InizializeWeight();

				// Creo il vettore contenente i dati di uscita
				this->output_data = new T_Precision[this->layers.back()->n_neurons];

				// Imposto la funzione di report dell'addestramento
				this->report_function = &Network::training_report;

				// Pulisco il vettore dei valori
				values.clear();

				// Imposto il flag di uscita
				end = true;

			} else {

				// E se trovo un errore lo communico all'utente
				fprintf(	stderr, " (W) Syntax error on training file '%s', at line %lu: Insufficient number of layers!\n",
							path.c_str(), (unsigned long) n_line );
			}

			// Incremento il contatore della linea
			n_line++;
		}
	}

	// Iteratori
	size_t i, t = 1;

	// Iteratori delle sinapsi
	Synapse *synapse_i, *last_synapse_i;

	// Leggo le sinapsi di tutti gli strati
	while ( t < this->layers.size() && file.good() ) {

		// Leggo la nuova riga
		std::getline( file, line );

		// Controllo che la linea non sia vuota o un commento
		if ( !line.empty() && line.at(0) != _SEROTONINA_COMMENT_ ) {

			// Estraggo i valori di uscita
			if ( values_from_string( line, values ) == this->layers[t]->n_synapses ) {

				// Preparo l'iteratore delle sinapsi
				synapse_i		= this->layers[t]->first_synapse;
				last_synapse_i	= this->layers[t]->last_synapse;

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
				fprintf(	stderr, " (W) Syntax error on training file '%s', at line %lu!\n",
							path.c_str(), (unsigned long)  n_line );
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
}

} // Chiudo il namespace di Serotonina

#endif
