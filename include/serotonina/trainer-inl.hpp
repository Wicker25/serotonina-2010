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

inline void
Trainer::Train(	T_TrainFun &train_fun, const std::vector< T_Precision > &input_samples, const std::vector< T_Precision > &output_samples,
				T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports ) {

	// Richiamo la funzione di addestramento principale
	this->Train(	train_fun, &input_samples[0], &output_samples[0], ( input_samples.size() / this->network->layers[0]->n_neurons ), 
					desired_error, max_epochs, epochs_between_reports );
}

inline T_Precision
Trainer::GetError() const {

	// Ritorno l'errore della rete
	return this->net_error;
}

} // Chiudo il namespace di Serotonina

#endif
