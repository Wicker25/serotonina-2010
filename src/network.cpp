/* 
    Title --- network.cpp

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

#ifndef _SEROTONINA_NETWORK_CPP_
#define _SEROTONINA_NETWORK_CPP_

#include <serotonina/network.hpp>
#include <serotonina/network-inl.hpp>

namespace Serotonina { // Namespace di Serotonina

Network::Network( size_t n_layers, ... ) {

	// Inizializzo il generatore di numeri pseudocasuali
	srand( (size_t) time( NULL ) );

	// Struttura degli strati
	std::vector< size_t > layers_struct;

	// Lista dei parametri della funzione
	va_list args;

	// Inizio la lettura della lista dei parametri
	va_start( args, n_layers );

	// Memorizzo la struttura degli strati in un vettore
	while( n_layers-- > 0 ) {

		// Aggiungo lo strato al vettore
		layers_struct.push_back( va_arg( args, size_t ) );
	}   

	// Termino la lettura della lista dei parametri
	va_end( args );

	// Costruisco le strutture della nuova rete neurale
	this->MakeStructures( layers_struct.size(), layers_struct.data() );
}

Network::Network( size_t n_layers, const size_t *layers_struct ) {

	// Inizializzo il generatore di numeri pseudocasuali
	srand( (size_t) time( NULL ) );

	// Costruisco le strutture della nuova rete neurale
	this->MakeStructures( n_layers, layers_struct );
}

Network::Network( const std::vector< size_t > &layers_struct ) {

	// Inizializzo il generatore di numeri pseudocasuali
	srand( (size_t) time( NULL ) );

	// Costruisco le strutture della nuova rete neurale
	this->MakeStructures( layers_struct.size(), layers_struct.data() );
}

Network::Network( const std::string &path ) {

	// Inizializzo il generatore di numeri pseudocasuali
	srand( (size_t) time( NULL ) );

	// Carico la rete neurale dal file
	this->Load( path );
}

Network::~Network() {

	// Iteratore
	size_t i = 0;

	// Elimino gli strati della rete
	for ( ; i < this->layers.size(); i++ )
		delete this->layers[i];

	// Pulisco il vettore degli strati
	this->layers.clear();
}

void
Network::MakeStructures( size_t n_layers, const size_t *layers_struct ) {

	// Controllo che siano stati scelti almeno tre strati
	if ( n_layers < 2 ) {

		// Sollevo un'eccezione
		throw Exception( "Insufficient number of layers" );
	}

	// Creo lo strato di ingresso
	this->layers.push_back( new Layer( layers_struct[0], 0 ) );

	// Iteratore
	size_t i = 1;

	// Creo gli strati della rete
	for ( ; i < n_layers; i++ ) {

		// Creo lo strato
		this->layers.push_back( new Layer( layers_struct[i], layers_struct[i - 1] ) );
	}   

	// Creo il vettore contenente i dati di uscita
	this->output_data.resize( this->layers.back()->n_neurons );
}

void
Network::SetInputs( const std::vector< T_Precision > &inputs ) {

	// Controllo che gli input passati siano tanti quanti quelli della rete
	if ( inputs.size() != this->layers.front()->n_neurons ) {

		// Sollevo un'eccezione
		throw Exception( "Insufficient number of inputs" );
	}

	// Richiamo la funzione principale
	this->SetInputs( &inputs[0] );
}

void
Network::SetInputs( const T_Precision *inputs ) {

	// Iteratore
	size_t i = 0;

	// Imposto gli ingressi della rete neurale
	for ( ; i < this->layers.front()->n_neurons; i++ ) {

		this->layers.front()->neurons[i].value = between( inputs[i], 0.0, 1.0 );
	}
}

const std::vector< T_Precision > &
Network::Run() {

	// Iteratore
	short unsigned int t;

	// Iteratori dei neuroni
	Neuron *neuron_j, *last_neuron_j;
	Neuron *neuron_i, *last_neuron_i;

	// Iteratori delle sinapsi
	Synapse *synapse_t;

	// Calcolo i valori di uscita di ogni strato della rete
	for ( t = 1; t < this->layers.size(); t++ ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->layers[t]->first_synapse;

		// Preparo l'iteratore dei neuroni
		last_neuron_i	= this->layers[t]->last_neuron;
		neuron_i		= this->layers[t]->first_neuron;

		// Calcolo i valori di uscita dello strato corrente
		// Y_i = T( SUM( w_ji - bias_i ) ) oppure Y_i = T( SUM( w_ji ) )
		for ( ; neuron_i <= last_neuron_i; neuron_i++ ) {

			// Azzero il valore di attivazione del neurone
			neuron_i->value = 0;

			// Preparo l'iteratore dei neuroni
			last_neuron_j	= this->layers[t - 1]->last_neuron;
			neuron_j		= this->layers[t - 1]->first_neuron;

			// Imposto il valore di attivazione come la somma pesata di tutti i neuroni in ingresso
			for ( ; neuron_j <= last_neuron_j; neuron_j++, synapse_t++ ) {

				neuron_i->value += synapse_t->weight * neuron_j->value;
			}

			// Aggiungo la soglia del bias
			neuron_i->value += synapse_t->weight;

			// Sposto l'iteratore oltre la sinapsi del bias, nella riga contente le sinapsi del neurone successivo
			synapse_t++;

			// Calcolo il valore in uscita del neurone utilizzando una sigmoide
			neuron_i->value = sigmoid( neuron_i->value );
		}
	}

	// Preparo l'iteratore dei neuroni
	last_neuron_i	= this->layers.back()->last_neuron;
	neuron_i		= this->layers.back()->first_neuron;

	// Iteratore
	size_t i = 0;

	// Copio le uscite nel vettore specifico
	for ( ; neuron_i <= last_neuron_i; i++, neuron_i++ ) {

		// Copio l'uscita del neurone
		this->output_data[i] = neuron_i->value;
	}

	// Ritorna un puntatore ai valori di uscita
	return this->GetOutputs();
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

		// Sollevo un'eccezione
		throw Exception( "Couldn't open file '", path, "'" );
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
Network::Load( const std::string &path ) {

	// Controllo se esiste già una rete neurale
	if ( !this->layers.empty() ) {

		// Iteratore
		size_t i = 0;

		// Elimino gli strati della rete
		for ( ; i < this->layers.size(); i++ )
			delete this->layers[i];

		// Pulisco il vettore degli strati
		this->layers.clear();
	}

	// Apre uno stream al file sorgente
	std::ifstream file( path.c_str() );

	// Controllo se il file è stato aperto correttamente
	if ( !file.is_open() ) {

		// Sollevo un'eccezione
		throw Exception( "Couldn't read from file '", path, "'" );
	}

	// Vettore con i valori estratti dalla riga
	std::vector< size_t > layers_struct;

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
			if ( values_from_string< size_t >( line, layers_struct ) >= 2 ) {

				// Costruisco le strutture della nuova rete neurale
				this->MakeStructures( layers_struct.size(), layers_struct.data() );

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

	// Vettore con i valori estratti dalla riga
	std::vector< T_Precision > values;

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
}

std::ostream &
operator<<( std::ostream &out, Network &network ) {

	// Scrivo la struttura della rete neurale nello stream
	out << "<Neural network [" << network.GetLayer(0).n_neurons;

	size_t i = 1;

	for ( ; i < network.GetLayers().size(); i++ ) {

		out << "," << network.GetLayer(i).n_neurons;
	}

	out << "]>";

	return out;
}

} // Chiudo il namespace di Serotonina

#endif
