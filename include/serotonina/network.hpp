/* 
    Title --- network.hpp

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

#ifndef _SEROTONINA_NETWORK_HPP_
#define _SEROTONINA_NETWORK_HPP_

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdarg>

#include <ctime>

#include <serotonina/version.hpp>
#include <serotonina/types.hpp>
#include <serotonina/macros.hpp>
#include <serotonina/string.hpp>

#include <serotonina/neuron.hpp>
#include <serotonina/synapse.hpp>
#include <serotonina/layer.hpp>

#define _SEROTONINA_COMMENT_		'#'
#define _SEROTONINA_SEPARATOR_		':'

namespace Serotonina { // Namespace di Serotonina

// Prototipi
class Network;

class Network {

	/* RETE NEURALE */

public:

	// Metodi costruttore e distruttore
	Network( size_t n_layers, ... );
	Network( size_t n_layers, const size_t *layers_struct );
	Network( const std::vector< size_t > &layers_struct );
	Network( const std::string &path );
	virtual ~Network();

	// Esegue la rete neurale
	const std::vector< T_Precision > &Run( const T_Precision *input );
	const std::vector< T_Precision > &Run( const std::vector< T_Precision > &input );

	// Ritorna le uscite della rete
	const std::vector< T_Precision > &GetOutputs() const;

	// Ritorna gli strati della rete
	std::vector< Layer * > &GetLayers();

	// Ritorna uno strato specifico della rete
	Layer &GetLayer( size_t n );

	// Salva la rete neurale in un file
	void Save( const std::string &path );

	// Carica la rete neurale da un file
	void Load( const std::string &path );

private:

	// Strati della rete
	std::vector< Layer * > layers;

	// Dati di uscita
	std::vector< T_Precision > output_data;

	// Costruisce le strutture della nuova rete neurale
	void MakeStructures( size_t n_layers, const size_t *layers_struct );
};

} // Chiudo il namespace di Serotonina

#endif
