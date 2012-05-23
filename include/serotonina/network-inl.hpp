/* 
    Title --- network-inl.hpp

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

#ifndef _SEROTONINA_NETWORK_INL_HPP_
#define _SEROTONINA_NETWORK_INL_HPP_

#include <serotonina/network.hpp>

namespace serotonina { // Namespace di Serotonina

inline const std::vector< T_Precision > &
Network::run( const T_Precision *inputs ) {

	// Imposto gli ingressi della rete neurale
	this->setInputs( inputs );

	// Richiamo la funzione principale
	return this->run();
}

inline const std::vector< T_Precision > &
Network::run( const std::vector< T_Precision > &inputs ) {

	// Imposto gli ingressi della rete neurale
	this->setInputs( inputs );

	// Richiamo la funzione principale
	return this->run();
}

inline const std::vector< T_Precision > &
Network::getOutputs() const {

	// Ritorno le uscite della rete
	return this->output_data;
}

inline const T_Precision
Network::getOutput( size_t n ) const {

	// Ritorno un uscita della rete
	return this->output_data.at(n);
}

inline std::vector< Layer * > &
Network::getLayers() {

	// Ritorno gli strati della rete
	return this->layers;
}

inline Layer &
Network::getLayer( size_t n ) {

	// Ritorno uno strato specifico della rete
	return *this->layers.at(n);
}

inline size_t
Network::getNumOfInputs() const {

	// Ritorno il numero degli ingressi della rete
	return this->layers.front()->n_neurons;
}

inline size_t
Network::getNumOfOutputs() const {

	// Ritorno il numero delle uscite della rete
	return this->layers.back()->n_neurons;
}

} // Chiudo il namespace di Serotonina

#endif
