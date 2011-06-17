/* 
    Title --- trainer.cpp

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

#ifndef _SEROTONINA_TRAINER_CPP_
#define _SEROTONINA_TRAINER_CPP_

#include <serotonina/trainer.hpp>
#include <serotonina/trainer-inl.hpp>

namespace Serotonina { // Namespace di Serotonina

/** INIZIO METODI STATICI **/

int
Trainer::training_report(	Network *network, size_t epochs, time_t elapsed_time, T_Precision max_error,
							const T_Precision *outputs, size_t outputs_size, void *data ) {

	// Calcolo le ore, i minuti e i secondi del tempo trascorso
	size_t hours, mins, secs;

	secs	= elapsed_time % 60;
	mins	= ( elapsed_time / 60 ) % 60;
	hours	= ( elapsed_time / 3600 );

	// Stampo il rapporto dell'addestramento
	printf(	"  Epochs #%lu, time %luh%lum%lus, error %.10f\n",
			(unsigned long) epochs, (unsigned long) hours, (unsigned long) mins, (unsigned long) secs, (double) max_error );

	return 0;
}

/** FINE METODI STATICI **/


Trainer::Trainer( Network *network_ ) {

	// Memorizzo l'indirizzo della rete neurale
	this->SetNetwork( network_ );

	// Inizializzo i pesi sinaptici della rete neurale
	this->InitWeights();

	// Imposto la funzione di report dell'addestramento
	this->report_fun = &Trainer::training_report;
}

Trainer::Trainer( Network &network_ ) {

	// Memorizzo l'indirizzo della rete neurale
	this->SetNetwork( network_ );

	// Inizializzo i pesi sinaptici della rete neurale
	this->InitWeights();

	// Imposto la funzione di report dell'addestramento
	this->report_fun = &Trainer::training_report;
}

Trainer::Trainer( Network *network_, bool init_weights ) {

	// Memorizzo l'indirizzo della rete neurale
	this->SetNetwork( network_ );

	// Controllo se si è scelto di inizializzare i pesi sinaptici della rete
	if ( init_weights ) {

		// Inizializzo i pesi sinaptici della rete neurale
		this->InitWeights();
	}

	// Imposto la funzione di report dell'addestramento
	this->report_fun = &Trainer::training_report;
}

Trainer::Trainer( Network &network_, bool init_weights ) {

	// Memorizzo l'indirizzo della rete neurale
	this->SetNetwork( network_ );

	// Controllo se si è scelto di inizializzare i pesi sinaptici della rete
	if ( init_weights ) {

		// Inizializzo i pesi sinaptici della rete neurale
		this->InitWeights();
	}

	// Imposto la funzione di report dell'addestramento
	this->report_fun = &Trainer::training_report;
}

Trainer::~Trainer() {

}

void
Trainer::InitWeights() {

	// Iteratori
	short int t = ( this->network->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando la Regola Delta con il Momentum
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->network->layers[t + 1]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->network->layers[t + 1]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Inizializzo il peso sinaptico
			synapse_t->weight = rand0to1< T_Precision >();
		}
	}
}

void
Trainer::CreateTrainData() {

	// Iteratori
	short int t = ( this->network->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Creo le strutture necessarie all'addestramento
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->network->layers[t + 1]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->network->layers[t + 1]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Creo la struttura per l'addestramento
			synapse_t->train = new TrainData;
		}
	}
}

void
Trainer::DeleteTrainData() {

	// Iteratori
	short int t = ( this->network->layers.size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Cancello le strutture necessarie all'addestramento
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->network->layers[t + 1]->first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->network->layers[t + 1]->last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Cancello la struttura per l'addestramento
			delete synapse_t->train;
		}
	}
}

void
Trainer::ComputeError( const T_Precision *target ) {

	// Iteratore
	size_t i = 0;

	// Iteratori dei neuroni
	Neuron *neuron_j, *last_neuron_j;
	Neuron *neuron_i, *last_neuron_i;

	// Iteratori delle sinapsi
	Synapse *synapse_t;

	// Preparo l'iteratore dei neuroni
	last_neuron_i	= this->network->layers.back()->last_neuron;
	neuron_i		= this->network->layers.back()->first_neuron;

	// Preparo l'iteratore delle sinapsi
	synapse_t = this->network->layers.back()->first_synapse;

	// Calcolo l'errore dello strato di uscita
	for ( ; neuron_i <= last_neuron_i; i++, neuron_i++ ) {

		// Calcolo l'errore dell'uscita
		// dE/dy_i = -(D_i - Y_i)
		neuron_i->dEdy = - ( target[i] - neuron_i->value );

		// Preparo l'iteratore dei neuroni
		last_neuron_j	= this->network->layers[this->network->layers.size() - 2]->last_neuron;
		neuron_j		= this->network->layers[this->network->layers.size() - 2]->first_neuron;

		// Calcolo l'errore dei pesi sinaptici del neurone
		// dE/dw_ji += dE/dy_i * dy_i/dP_i * dP_i/dw_ji

		// Ciclo per tutti i neuroni del penultimo strato
		for ( ; neuron_j <= last_neuron_j; neuron_j++, synapse_t++ ) {

			synapse_t->train->dEdw += neuron_i->dEdy * d_sigmoid( neuron_i->value ) * neuron_j->value;
		}

		// Aggiungo l'errore del bias al peso sinaptico del neurone
		// dE/dw_ji += dE/dy_i * dy_i/dP_i
		synapse_t->train->dEdw += neuron_i->dEdy * d_sigmoid( neuron_i->value );

		// Sposto l'iteratore oltre la sinapsi del bias, nella riga contente le sinapsi del neurone successivo
		synapse_t++;

		// Aggiungo il quadrato dell'errore all'errore totale della rete (per il calcolo dell'errore quadratico medio)
		this->net_error += pow2( neuron_i->dEdy );
	}
}

void
Trainer::BackpropagateError() {

	// Iteratori
	short int t = ( (short int) this->network->layers.size() - 1 ) - 2;

	// Delta per la correzione del peso sinaptico
	T_Precision delta;

	// Iteratori dei neuroni
	Neuron *neuron_j, *last_neuron_j;
	Neuron *neuron_i, *last_neuron_i;
	Neuron *neuron_k, *last_neuron_k;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *synapse_row_t1, *synapse_t1;

	// Retropropago l'errore nella rete
	for ( ; t >= 0; t-- ) {

		// Preparo gli iteratori delle sinapsi
		synapse_t = this->network->layers[t + 1]->first_synapse;
		synapse_row_t1 = this->network->layers[t + 2]->first_synapse;

		// Preparo l'iteratore dei neuroni
		last_neuron_j	= this->network->layers[t + 1]->last_neuron;
		neuron_j		= this->network->layers[t + 1]->first_neuron;

		// Ciclo per tutti i neuroni dello strato 't + 1'
		for ( ; neuron_j <= last_neuron_j; neuron_j++, synapse_row_t1++ ) {

			// Azzero l'errore sull'uscita 
			neuron_j->dEdy = 0;

			// Preparo l'iteratore delle sinapsi
			synapse_t1 = synapse_row_t1;

			// Preparo l'iteratore dei neuroni
			last_neuron_k	= this->network->layers[t + 2]->last_neuron;
			neuron_k		= this->network->layers[t + 2]->first_neuron;

			// Calcolo l'errore retropropagato dai neuroni dello strato successivo
			// dE/dz_k = SUM( dE/dy_j * dy_j/dP_j * dP_j/dz_k )

			// Ciclo per tutti i neuroni dello strato 't + 2'
			for ( ; neuron_k <= last_neuron_k; neuron_k++, synapse_t1 += this->network->layers[t + 2]->synapses_per_row ) {

				neuron_j->dEdy += neuron_k->dEdy * d_sigmoid( neuron_k->value ) * synapse_t1->weight;
			}

			// Calcolo il delta per la correzione del peso sinaptico
			delta = neuron_j->dEdy * d_sigmoid( neuron_j->value );

			// Preparo l'iteratore dei neuroni
			last_neuron_i	= this->network->layers[t]->last_neuron;
			neuron_i		= this->network->layers[t]->first_neuron;

			// Ciclo per tutti i neuroni dello strato 't'
			for ( ; neuron_i <= last_neuron_i; neuron_i++, synapse_t++ ) {

				// dE/dw_ij += dE/dy_j * dy_j/dP_j * Z_i
				synapse_t->train->dEdw += delta * neuron_i->value;
			}

			// Aggiungo l'errore del bias al peso sinaptico del neurone
			// dE/dw_ij += dE/dy_j * dy_j/dP_j
			synapse_t->train->dEdw += delta;

			// Sposto l'iteratore oltre la sinapsi del bias, nella riga contente le sinapsi del neurone successivo
			synapse_t++;
		}
	}
}

void
Trainer::Train(	T_TrainFun &train_fun, const T_Precision *input_samples, const T_Precision *output_samples, size_t n_samples,
				T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports ) {

	// Creo le strutture per l'addestramento
	this->CreateTrainData();

	// Numero delle epoche dell'addestramento
	size_t epochs = 0;

	// Cronometro dell'addestramento
	time_t t_elapsed, t_start = time( NULL );

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
			this->network->Run( &input_samples[ i * this->network->layers.front()->n_neurons ] );

			// Calcolo l'errore delle uscite
			this->ComputeError( &output_samples[ i * this->network->layers.back()->n_neurons ] );

			// Retropropago l'errore nella rete
			this->BackpropagateError();
		}

		// Calcolo l'errore quadratico medio della rete (MSE)
		// E(x) = SUM( e^2 ) / n_samples
		this->net_error /= ( this->network->layers.back()->n_neurons * n_samples );

		// Controllo se non si è raggiunto l'errore desiderato
		if ( this->net_error > desired_error ) {

			// Aggiorno i pesi sinaptici della rete
			train_fun( *this->network, this->train_params, this->net_error, this->prev_net_error );

			// Log dell'addestramento ogni N epoche
			if ( epochs % epochs_between_reports == 0 ) {

				// Calcolo quanto tempo è passato dall'inizio dell'addestramento
				t_elapsed = ( time( NULL ) - t_start );

				// Passo le informazioni alla funzione di report
				int flag = this->report_fun(	this->network, epochs, t_elapsed, this->net_error, (const T_Precision *) this->network->output_data,
												this->network->layers.back()->n_neurons, this->report_fun_data );

				// Controllo se è stato inviato un segnale di uscita
				if ( flag ) break;
			}

			// Incremento il contatore delle epoche
			epochs++;
		}

	// Se non si è raggiunto l'errore massimo voluto e le epoche sono entro i limiti continuo l'addestramento
	} while ( this->net_error > desired_error && epochs < max_epochs );


	// Report conclusivo
	if ( epochs % epochs_between_reports != 0 ) {

		// Calcolo quanto tempo è passato dall'inizio dell'addestramento
		t_elapsed = ( time( NULL ) - t_start );

		// Passo le informazioni alla funzione di report
		this->report_fun(	this->network, epochs, t_elapsed, this->net_error, (const T_Precision *) this->network->output_data,
								this->network->layers.back()->n_neurons, this->report_fun_data );
	}

	// Cancello le strutture per l'addestramento
	this->DeleteTrainData();
}

void
Trainer::TrainOnFile( T_TrainFun &train_fun, const std::string &path, T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports ) {

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
	std::vector< T_Precision > inputs_samples;

	// Valori d'uscita
	std::vector< T_Precision > outputs_samples;

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
		if ( !line.empty() && line.at(0) != _SEROTONINA_COMMENT_ ) {

			// Vettore contenente i valori estratti
			std::vector< T_Precision > info;

			if ( values_from_string( line, info ) != 2 ) {

				// E se trovo un errore lo communico all'utente
				fprintf( stderr, " (W) Syntax error on training file '%s', at line %lu!\n", path.c_str(), (unsigned long) n_line );
			}

			// Controllo se il tipo di addestramento non è adatto alla rete corrente
			if ( this->network->layers.front()->n_neurons != (size_t) info[0] || this->network->layers.back()->n_neurons != (size_t) info[1] ) {

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
		if ( !line.empty() && line.at(0) != _SEROTONINA_COMMENT_ ) {

			// Cerco il separatore nella riga
			int found = line.find_first_of( _SEROTONINA_SEPARATOR_ );

			// Estraggo i valori di ingresso
			if ( values_from_string( line.substr( 0, found ), inputs_samples ) != this->network->layers.front()->n_neurons ) {

				// E se trovo un errore lo communico all'utente
				fprintf( stderr, " (W) Syntax error on training file '%s', at line %lu!\n", path.c_str(), (unsigned long) n_line );
			}

			// Estraggo i valori di uscita
			if ( values_from_string( line.substr( found + 1 ), outputs_samples ) != this->network->layers.back()->n_neurons ) {

				// E se trovo un errore lo communico all'utente
				fprintf( stderr, " (W) Syntax error on training file '%s', at line %lu!\n", path.c_str(), (unsigned long) n_line );
			}

			// Incremento il numero dei campioni caricati
			n_samples++;

			// Incremento il contatore della linea
			n_line++;
		}
	}

	// Chiudo lo stream al file
	file.close();

	// Richiamo la funzione di addestramento principale
	this->Train( train_fun, &inputs_samples[0], &outputs_samples[0], n_samples, desired_error, max_epochs, epochs_between_reports );
}

} // Chiudo il namespace di Serotonina

#endif
