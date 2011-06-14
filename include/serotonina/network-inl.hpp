/* 
    Title --- network-inl.hpp

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

#ifndef _SEROTONINA_NETWORK_INL_HPP_
#define _SEROTONINA_NETWORK_INL_HPP_

#include <serotonina/network.hpp>

namespace Serotonina { // Namespace di Serotonina

inline void
Network::SetTrainingAlgorithm( TrainAlgorithm algorithm ) {

	// Imposto l'algoritmo di addestramento
	this->train_algorithm = algorithm;
}

inline void
Network::SetTrainingParameters( T_Precision learning_rate_, T_Precision momentum_ ) {

	// Imposto il tasso di apprendimento e il momentum
	this->learning_rate	= between( learning_rate_, 0.01, 0.99 );
	this->momentum		= between( momentum_, 0.01, 0.99 );
}

inline void
Network::SetRpropFactor( T_Precision increase_factor_, T_Precision decrease_factor_ ) {

	// Imposto i fattori di incremento e decremento del RPROP
	this->increase_factor	= between( increase_factor_, 1.01, 5.0 );
	this->decrease_factor	= between( decrease_factor_, 0.01, 0.99 );
}

inline T_Precision
Network::GetError() const {

	// Ritorno l'errore della rete
	return this->net_error;
}

inline void
Network::Load( const std::string &path ) {

	// Richiamo la funzione principale
	this->Load( path, false );
}

inline void
Network::SetReportSignal( ReportSignal function, void *data ) {

	// Imposto la funzione di report dell'addestramento
	this->report_function = function;

	// Imposto il parametro ausiliario della funzione di report
	this->report_function_data = data;
}

} // Chiudo il namespace di Serotonina

#endif
