/* 
    Title --- trainer-inl.hpp

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

	// Richiamo la funzione inferiore
	this->SetParameters( p0 );

	// Memorizzo i parametri dell'addestramento
	this->train_params.push_back( p1 );
}

inline void
Trainer::SetParameters( T_Precision p0, T_Precision p1, T_Precision p2 ) {

	// Cancello i parametri esistenti
	this->train_params.clear();

	// Richiamo la funzione inferiore
	this->SetParameters( p0, p1 );

	// Memorizzo i parametri dell'addestramento
	this->train_params.push_back( p2 );
}

inline void
Trainer::SetParameters( T_Precision p0, T_Precision p1, T_Precision p2, T_Precision p3 ) {

	// Cancello i parametri esistenti
	this->train_params.clear();

	// Richiamo la funzione inferiore
	this->SetParameters( p0, p1, p2 );

	// Memorizzo i parametri dell'addestramento
	this->train_params.push_back( p3 );
}

inline void
Trainer::SetParameters( T_Precision p0, T_Precision p1, T_Precision p2, T_Precision p3, T_Precision p4 ) {

	// Cancello i parametri esistenti
	this->train_params.clear();

	// Richiamo la funzione inferiore
	this->SetParameters( p0, p1, p2, p3 );

	// Memorizzo i parametri dell'addestramento
	this->train_params.push_back( p4 );
}

inline void
Trainer::SetParameters( const std::vector< T_Precision > &params ) {

	// Cancello i parametri esistenti
	this->train_params.clear();

	// Memorizzo i parametri dell'addestramento
	copy( params.begin(), params.end(), this->train_params.begin() );
}

inline const std::vector< T_Precision > &
Trainer::GetParameters() const {

	// Ritorno i parametri dell'addestramento
	return this->train_params;
}

inline const T_Precision
Trainer::GetParameter( size_t n ) const {

	// Ritorno un parametro dell'addestramento
	return this->train_params.at(n);
}

inline void
Trainer::SetReportFun( T_ReportFun &fun ) {

	// Imposto la funzione di report dell'addestramento
	this->report_fun = &fun;
}

inline T_ReportFun *
Trainer::GetReportFun() const {

	// Ritorno la funzione di report dell'addestramento
	return this->report_fun;
}

inline void
Trainer::SetReportFunData( void *data ) {

	// Imposto il parametro ausiliario della funzione di report
	this->report_fun_data = data;
}

inline void *
Trainer::GetReportFunData() const {

	// Ritorno il parametro ausiliario della funzione di report
	return this->report_fun_data;
}

template < class train_algorithm >
inline void
Trainer::Train(	const std::vector< T_Precision > &input_samples, const std::vector< T_Precision > &output_samples,
				T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports ) {

	// Controllo la correttezza dell'insieme di addestramento
	if ( (float) input_samples.size() / (float) output_samples.size() == (float) this->network->GetNumOfInputs() / (float) this->network->GetNumOfOutputs() ) {

		// Richiamo la funzione di addestramento principale
		this->Train< train_algorithm >(	&input_samples[0], &output_samples[0], ( input_samples.size() / this->network->GetLayers()[0]->n_neurons ), 
										desired_error, max_epochs, epochs_between_reports );

	// Altrimenti sollevo un eccezione
	} else throw Exception( "Wrong training set" );
}

template < class train_algorithm >
void
Trainer::Train(	const T_Precision *input_samples, const T_Precision *output_samples, size_t n_samples,
				T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports ) {

	// Log di lavoro
	std::cout << "Start training with " << n_samples << " samples.\n";
	std::cout << "Set algorithm: '" << train_algorithm::description << "'\n";

	// Verifico i parametri dell'addestramento
	if ( train_algorithm::CheckParams( this->train_params ) ) {

		// Creo le strutture per l'addestramento
		train_algorithm::InitTraining( *this->network );

		// Flag di uscita forzata dall'addestramento
		int interrupt_flag = 0;

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
				this->network->Run( &input_samples[ i * this->network->GetLayers().front()->n_neurons ] );

				// Calcolo l'errore delle uscite
				this->ComputeOutputError( &output_samples[ i * this->network->GetLayers().back()->n_neurons ] );

				// Retropropago l'errore nella rete
				this->BackpropagateError();
			}

			// Calcolo l'errore quadratico medio della rete (MSE)
			// E(x) = SUM( e^2 ) / n_samples
			this->net_error /= ( this->network->GetLayers().back()->n_neurons * n_samples );

			// Controllo se non si è raggiunto l'errore desiderato
			if ( this->net_error > desired_error ) {

				// Aggiorno i pesi sinaptici della rete
				train_algorithm::UpdateWeights( *this->network, this->train_params, this->net_error, this->prev_net_error );

				// Log dell'addestramento ogni N epoche
				if ( epochs_between_reports && epochs % epochs_between_reports == 0 ) {

					// Calcolo quanto tempo è passato dall'inizio dell'addestramento
					t_elapsed = ( time( NULL ) - t_start );

					// Passo le informazioni alla funzione di report
					interrupt_flag = this->report_fun(	*this->network, epochs, t_elapsed, this->net_error,
														this->network->GetOutputs(), this->report_fun_data );
				}

				// Incremento il contatore delle epoche
				epochs++;
			}

		// Se non si è raggiunto l'errore massimo voluto e le epoche sono entro i limiti continuo l'addestramento
		} while ( this->net_error > desired_error && epochs < max_epochs && !interrupt_flag );


		// Report conclusivo
		if ( epochs_between_reports && epochs % epochs_between_reports != 0 ) {

			// Calcolo quanto tempo è passato dall'inizio dell'addestramento
			t_elapsed = ( time( NULL ) - t_start );

			// Passo le informazioni alla funzione di report
			this->report_fun(	*this->network, epochs, t_elapsed, this->net_error,
								this->network->GetOutputs(), this->report_fun_data );
		}

		// Cancello le strutture per l'addestramento
		train_algorithm::EndTraining( *this->network );

		// Controllo il modo in cui è terminato l'addestramento
		if ( interrupt_flag ) {
					
			// Log di lavoro
			std::cout << "Training is interrupted.\n";

		} else {

			// Log di lavoro
			std::cout << "Training completed in " << epochs << " epochs.\n";
		}

	// Sollevo un'eccezione
	} else throw Exception( "Wrong parameters of training" );
}

template < class train_algorithm >
void
Trainer::TrainOnFile(	const std::string &path, T_Precision desired_error,
						size_t max_epochs, size_t epochs_between_reports ) {

	// Apre uno stream al file dell'addestramento
	std::ifstream file( path.c_str() );

	// Controllo se il file è stato aperto correttamente
	if ( !file.is_open() ) {

		// Sollevo un'eccezione
		throw Exception( "Couldn't open file '", path, "'" );
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
			if ( this->network->GetLayers().front()->n_neurons != (size_t) info[0] || this->network->GetLayers().back()->n_neurons != (size_t) info[1] ) {

				// Sollevo un'eccezione
				throw Exception( "Incompatible training file '", path, "'" );
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
			if ( values_from_string( line.substr( 0, found ), inputs_samples ) != this->network->GetLayers().front()->n_neurons ) {

				// E se trovo un errore lo communico all'utente
				fprintf( stderr, " (W) Syntax error on training file '%s', at line %lu!\n", path.c_str(), (unsigned long) n_line );
			}

			// Estraggo i valori di uscita
			if ( values_from_string( line.substr( found + 1 ), outputs_samples ) != this->network->GetLayers().back()->n_neurons ) {

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
	this->Train< train_algorithm >( &inputs_samples[0], &outputs_samples[0], n_samples, desired_error, max_epochs, epochs_between_reports );
}

inline T_Precision
Trainer::GetError() const {

	// Ritorno l'errore della rete
	return this->net_error;
}

} // Chiudo il namespace di Serotonina

#endif
