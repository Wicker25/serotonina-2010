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
    along with Serotonina. If not, see <http://www.gnu.org/licenses/>.
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
#include <serotonina/exception.hpp>
#include <serotonina/string.hpp>

#include <serotonina/neuron.hpp>
#include <serotonina/synapse.hpp>
#include <serotonina/layer.hpp>

#define _SEROTONINA_COMMENT_		'#'
#define _SEROTONINA_SEPARATOR_		':'

namespace serotonina { // Namespace di Serotonina

/*!
 * @class Network
 * \if english
 *   @brief Neural network's class
 * \endif
 * \if italian
 *   @brief Classe della rete neurale
 * \endif
 */

class Network {

	/* RETE NEURALE */

public:

	/**
	 * \if english
	 *   @brief Constructor method
	 *   @param[in] n_layers Number of layers
	 *   @param[in] ... Sequence of unsigned integers containing the number of neurons in each layer
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore
	 *   @param[in] n_layers Numero degli strati
	 *   @param[in] ... Sequenza di interi senza segno contenente il numero dei neuroni di ogni strato
	 * \endif
	 */

	// Metodo costruttore
	Network( size_t n_layers, ... );

	/**
	 * \if english
	 *   @brief Constructor method
	 *   @param[in] n_layers Number of layers
	 *   @param[in] layers_struct Vector containing the number of neurons in each layer
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore
	 *   @param[in] n_layers Numero degli strati
	 *   @param[in] layers_struct Vettore contenente il numero dei neuroni di ogni strato
	 * \endif
	 */

	// Metodo costruttore
	Network( size_t n_layers, const size_t *layers_struct );

	/**
	 * \if english
	 *   @brief Constructor method
	 *   @param[in] layers_struct Vector containing the number of neurons in each layer
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore
	 *   @param[in] layers_struct Vettore contenente il numero dei neuroni di ogni strato
	 * \endif
	 */

	// Metodo costruttore
	Network( const std::vector< size_t > &layers_struct );

	/**
	 * \if english
	 *   @brief Constructor method. Load the neural network from a file
	 *   @param[in] path Path to file of the neural network
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore. Carica la rete neurale da un file
	 *   @param[in] path Percorso al file di una rete neurale
	 * \endif
	 */

	// Metodo costruttore
	Network( const std::string &path );

	/**
	 * \if english
	 *   @brief Copy constructor method.
	 *   @param[in] other Another neural network
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore di copia.
	 *   @param[in] other Un'altra rete neurale
	 * \endif
	 */

	// Metodo costruttore di copia
	Network( Network &other );

	// Metodo distruttore
	virtual ~Network();

	/**
	 * \if english
	 *   @brief Set inputs of the network
	 *   @param[in] inputs Vector containing the inputs of the network
	 * \endif
	 * \if italian
	 *   @brief Imposta gli ingressi della rete
	 *   @param[in] inputs Vettore contenente gli ingressi
	 * \endif
	 */

	// Imposta gli ingressi della rete
	void setInputs( const T_Precision *inputs );

	/**
	 * \if english
	 *   @brief Set inputs of the network
	 *   @param[in] inputs Vector containing the inputs of the network
	 * \endif
	 * \if italian
	 *   @brief Imposta gli ingressi della rete
	 *   @param[in] inputs Vettore contenente gli ingressi
	 * \endif
	 */

	// Imposta gli ingressi della rete
	void setInputs( const std::vector< T_Precision > &inputs );

	/**
	 * \if english
	 *   @brief Returns the number of the inputs
	 *   @return Number of the inputs
	 * \endif
	 * \if italian
	 *   @brief Ritorna il numero degli ingressi
	 *   @return Il numero degli ingressi
	 * \endif
	 */

	// Ritorna il numero degli ingressi della rete
	size_t getNumOfInputs() const;

	/**
	 * \if english
	 *   @brief Compute the network's outputs
	 *   @return Vector containing the outputs of the network
	 * \endif
	 * \if italian
	 *   @brief Calcola le uscite della rete
	 *   @return Vettore contenente le uscite della rete
	 * \endif
	 */

	// Esegue la rete neurale
	const std::vector< T_Precision > &run();

	/**
	 * \if english
	 *   @brief Compute the network's outputs
	 *   @param[in] inputs Vector containing the inputs of the network
	 *   @return Vector containing the outputs of the network
	 * \endif
	 * \if italian
	 *   @brief Calcola le uscite della rete
	 *   @param[in] inputs Vettore contenente gli ingressi della rete
	 *   @return Vettore contenente le uscite della rete
	 * \endif
	 */

	// Esegue la rete neurale
	const std::vector< T_Precision > &run( const T_Precision *inputs );

	/**
	 * \if english
	 *   @brief Compute the network's outputs
	 *   @param[in] inputs Vector containing the inputs of the network
	 *   @return Vector containing the outputs of the network
	 * \endif
	 * \if italian
	 *   @brief Calcola le uscite della rete
	 *   @param[in] inputs Vettore contenente gli ingressi della rete
	 *   @return Vettore contenente le uscite della rete
	 * \endif
	 */

	// Esegue la rete neurale
	const std::vector< T_Precision > &run( const std::vector< T_Precision > &inputs );

	/**
	 * \if english
	 *   @brief Returns the outputs of the network 
	 *   @return Vector containing the outputs of the network
	 * \endif
	 * \if italian
	 *   @brief Ritorna le uscite della rete
	 *   @return Vettore contenente le uscite della rete
	 * \endif
	 */

	// Ritorna le uscite della rete
	const std::vector< T_Precision > &getOutputs() const;

	/**
	 * \if english
	 *   @brief Returns an output of the network 
	 *   @return Output of the network
	 * \endif
	 * \if italian
	 *   @brief Ritorna un'uscita della rete
	 *   @return Uscita della rete
	 * \endif
	 */

	// Ritorna un uscita della rete
	const T_Precision getOutput( size_t n ) const;

	/**
	 * \if english
	 *   @brief Returns the number of the outputs
	 *   @return Number of the outputs
	 * \endif
	 * \if italian
	 *   @brief Ritorna il numero delle uscite
	 *   @return Il numero delle uscite
	 * \endif
	 */

	// Ritorna il numero delle uscite della rete
	size_t getNumOfOutputs() const;

	/**
	 * \if english
	 *   @brief Returns the layers of the network
	 *   @return Vector containing the layers of the network
	 * \endif
	 * \if italian
	 *   @brief Ritorna gli strati della rete
	 *   @return Vettore contenente gli strati della rete
	 * \endif
	 */

	// Ritorna gli strati della rete
	std::vector< Layer * > &getLayers();

	/**
	 * \if english
	 *   @brief Returns a layer of the network
	 *   @return Layer of the network
	 * \endif
	 * \if italian
	 *   @brief Ritorna uno strato della rete
	 *   @return Strato della rete
	 * \endif
	 */

	// Ritorna uno strato specifico della rete
	Layer &getLayer( size_t n );

	/**
	 * \if english
	 *   @brief Save the network into a file
	 *   @param[in] path Path to the file
	 * \endif
	 * \if italian
	 *   @brief Salva la rete neurale in un file
	 *   @param[in] path Percorso al file
	 * \endif
	 */

	// Salva la rete neurale in un file
	void save( const std::string &path );

	/**
	 * \if english
	 *   @brief Load the network from a file
	 *   @param[in] path Path to the file
	 * \endif
	 * \if italian
	 *   @brief Carica la rete neurale da un file
	 *   @param[in] path Percorso al file
	 * \endif
	 */

	// Carica la rete neurale da un file
	void load( const std::string &path );

private:

	/**
	 * \if english
	 *   @brief Vector containing the layers of the network
	 * \endif
	 * \if italian
	 *   @brief Vettore contenente gli strati della rete
	 * \endif
	 */

	// Strati della rete
	std::vector< Layer * > layers;

	/**
	 * \if english
	 *   @brief Vector containing the outputs of the network
	 * \endif
	 * \if italian
	 *   @brief Vettore contenente le uscite della rete
	 * \endif
	 */

	// Dati di uscita
	std::vector< T_Precision > output_data;

	/**
	 * \if english
	 *   @brief Make structures of the new neural network
	 * \endif
	 * \if italian
	 *   @brief Costruisce le strutture della nuova rete neurale
	 * \endif
	 */

	// Costruisce le strutture della nuova rete neurale
	void makeStructs( size_t n_layers, const size_t *layers_struct );
};

/**
 * \if english
 *   @brief Write a network on a stream
 *   @param[in] out Stream
 *   @param[in] network The neural network
 * \endif
 * \if italian
 *   @brief Scrive la struttura di una rete su uno stream
 *   @param[in] out Stream
 *   @param[in] network La rete neurale
 * \endif
 */

// Scrive la struttura della rete neurale in uno stream
std::ostream &operator<<( std::ostream &out, Network &network );

} // Chiudo il namespace di Serotonina

#endif
