/* 
    Title --- trainer-inl.hpp

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

#ifndef _SEROTONINA_TRAINER_INL_HPP_
#define _SEROTONINA_TRAINER_INL_HPP_

#include <serotonina/trainer.hpp>

namespace Serotonina { // Namespace di Serotonina

inline void
Trainer::SetNetwork( Network *network_ ) {

	// Imposto la rete neurale da addestrare
	this->network = network_;
}

inline void
Trainer::SetNetwork( Network &network_ ) {

	// Imposto la rete neurale da addestrare
	this->network = &network_;
}

inline void
Trainer::SetParameters( T_Precision p0 ) {

	// Cancello i parametri esistenti
	this->train_params.clear();

	// Memorizzo i parametri dell'addestramento
	this->train_params.push_back( p0 );
}

inline void
Trainer::SetParameters( T_Precision p0, T_Precision p1 ) {

	// Cancello i parametri esistenti
	this->train_params.clear();

	// Memorizzo i parametri dell'addestramento
	this->train_params.push_back( p0 );
	this->train_params.push_back( p1 );
}

inline void
Trainer::SetParameters( T_Precision p0, T_Precision p1, T_Precision p2 ) {

	// Cancello i parametri esistenti
	this->train_params.clear();

	// Memorizzo i parametri dell'addestramento
	this->train_params.push_back( p0 );
	this->train_params.push_back( p1 );
	this->train_params.push_back( p2 );
}

inline void
Trainer::SetParameters( const std::vector< T_Precision > &params ) {

	// Cancello i parametri esistenti
	this->train_params.clear();

	// Memorizzo i parametri dell'addestramento
	copy( params.begin(), params.end(), this->train_params.begin() );
}

inline void
Trainer::SetReportFun( T_ReportFun &fun, void *data ) {

	// Imposto la funzione di report dell'addestramento
	this->report_fun = &fun;

	// Imposto il parametro ausiliario della funzione di report
	this->report_fun_data = data;
}

inline T_Precision
Trainer::GetError() const {

	// Ritorno l'errore della rete
	return this->net_error;
}

template < T_TrainFun train_fun > void
Trainer::Train(	const T_Precision *input_samples, const T_Precision *output_samples, size_t n_samples,
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
			train_fun( this->network, this->train_params, this->net_error, this->prev_net_error );
		} 

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

template < T_TrainFun train_fun > void
Trainer::Train(	const std::vector< T_Precision > &input_samples, const std::vector< T_Precision > &output_samples,
				T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports ) {

	// Richiamo la funzione di addestramento principale
	this->Train< train_fun >(	&input_samples[0], &output_samples[0], ( input_samples.size() / this->network->layers[0]->n_neurons ), 
								desired_error, max_epochs, epochs_between_reports );
}

template < T_TrainFun train_fun > void
Trainer::TrainOnFile( const std::string &path, T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports ) {

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
	this->Train< train_fun >( &inputs_samples[0], &outputs_samples[0], n_samples, desired_error, max_epochs, epochs_between_reports );
}

} // Chiudo il namespace di Serotonina

#endif
