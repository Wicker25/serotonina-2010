/* 
    Title --- network-inl.hpp

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

#ifndef _SEROTONINA_NETWORK_INL_HPP_
#define _SEROTONINA_NETWORK_INL_HPP_

#include <serotonina/network.hpp>

namespace Serotonina { // Namespace di Serotonina

template < typename TYPE > TYPE **
malloc_2dim( size_t n, size_t m ) {

	// Creo il vettore di puntatori delle righe
	TYPE **vect = new TYPE *[n];

	// Iteratore
	size_t i = 0;

	// Creo gli elementi delle righe
	for ( ; i < n ; i++ )
		vect[i] = new TYPE[m];

	return vect;
}

template < typename TYPE > void
dealloc_2dim( TYPE **vect, size_t size ) {

	// Iteratore
	size_t i = 0;

	// Dealloco gli elementi delle righe
	for ( ; i < size; i++ )
		delete vect[i];

	// Dealloco le righe
	delete vect;
}

inline void
Network::SetTrainingAlgorithm( TrainAlgorithm algorithm ) {

	// Imposto l'algoritmo di addestramento
	this->train_algorithm = algorithm;
}

inline void
Network::SetTrainingParameters( Precision learning_rate_, Precision momentum_ ) {

	// Imposto il tasso di apprendimento e il momentum
	this->learning_rate	= __BETWEEN__( learning_rate_, 0.01, 0.99 );
	this->momentum		= __BETWEEN__( momentum_, 0.01, 0.99 );
}

inline void
Network::SetRpropFactor( Precision increase_factor_, Precision decrease_factor_ ) {

	// Imposto i fattori di incremento e decremento del RPROP
	this->increase_factor	= __BETWEEN__( increase_factor_, 1.01, 5.0 );
	this->decrease_factor	= __BETWEEN__( decrease_factor_, 0.01, 0.99 );
}

inline Precision
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

}; // Chiudo il namespace di Serotonina

#endif
