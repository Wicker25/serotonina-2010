/* 
    Title --- layer.hpp

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

#ifndef _SEROTONINA_LAYER_HPP_
#define _SEROTONINA_LAYER_HPP_

#include <serotonina/neuron.hpp>
#include <serotonina/synapse.hpp>

namespace Serotonina { // Namespace di Serotonina

/*!
 * @struct Layer
 * \if english
 *   @brief Layer's structure
 * \endif
 * \if italian
 *   @brief Struttura di uno strato
 * \endif
 */

// Struttura di uno strato della rete
struct Layer {

	/**
	 * \if english
	 *   @brief Number of neurons
	 * \endif
	 * \if italian
	 *   @brief Numero dei neuroni
	 * \endif
	 */

	// Numero dei neuroni dello strato
	size_t n_neurons;

	// Neuroni dello strato (e puntatore al primo neurone dello strato)
	union {

		/**
		 * \if english
		 *   @brief Neurons of the layer
		 * \endif
		 * \if italian
		 *   @brief Neuroni dello strato
		 * \endif
		 */

		Neuron *neurons;

		/**
		 * \if english
		 *   @brief Pointer to the first neuron of the layer
		 * \endif
		 * \if italian
		 *   @brief Puntatore al primo neurone dello strato
		 * \endif
		 */

		Neuron *first_neuron;
	};

	/**
	 * \if english
	 *   @brief Pointer to the last neuron in the layer
	 * \endif
	 * \if italian
	 *   @brief Puntatore all'ultimo neurone nello strato
	 * \endif
	 */

	// Puntatore all'ultimo neurone dello strato
	Neuron *last_neuron;

	/**
	 * \if english
	 *   @brief Number of synpases in the layer
	 * \endif
	 * \if italian
	 *   @brief Numero delle sinapsi nello strato
	 * \endif
	 */

	// Numero delle sinapsi nello strato
	size_t n_synapses;

	// Sinapsi tra i due strati (e puntatore alla prima sinapsi)
	union {

		/**
		 * \if english
		 *   @brief Synapses of the layer
		 * \endif
		 * \if italian
		 *   @brief Sinapsi dello strato
		 * \endif
		 */

		Synapse *synapses;

		/**
		 * \if english
		 *   @brief Pointer to the first synapse of the layer
		 * \endif
		 * \if italian
		 *   @brief Puntatore alla prima sinapsi dello strato
		 * \endif
		 */

		Synapse *first_synapse;
	};

	/**
	 * \if english
	 *   @brief Pointer to the last synapse in the layer
	 * \endif
	 * \if italian
	 *   @brief Puntatore all'ultima sinapsi nello strato
	 * \endif
	 */

	// Puntatore all'ultima sinapsi
	Synapse *last_synapse;

	/**
	 * \if english
	 *   @brief Number of synapses per row
	 * \endif
	 * \if italian
	 *   @brief Numero di sinapsi per riga
	 * \endif
	 */

	// Numero di sinapsi per riga
	size_t synapses_per_row;

	/**
	 * \if english
	 *   @brief Constructor method
	 *   @param[in] n_neurons_ Numbers of neurons in the layer
	 *   @param[in] prev_n_neurons Numbers of neurons in the previous layer
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore
	 *   @param[in] n_neurons_ Numero dei neuroni nello strato
	 *   @param[in] prev_n_neurons Numero dei neuroni nello strato precedente
	 * \endif
	 */

	// Metodo costruttore
	Layer( size_t n_neurons_, size_t prev_n_neurons ) {

		// Memorizzo il numero dei neuroni
		this->n_neurons = n_neurons_;

		// Creo i neuroni dello strato
		this->neurons = new Neuron[this->n_neurons];

		// Imposto il puntatore dell'ultimo neurone
		this->last_neuron = &this->neurons[this->n_neurons - 1];

		// Calcolo il numero di sinapsi da allocare (più la sinapsi del bias)
		this->n_synapses = n_neurons_ * ( prev_n_neurons + 1 );

		// Creo le sinapsi tra i due strati
		this->synapses = new Synapse[this->n_synapses];

		// Imposto il puntatore dell'ultima prima sinapsi
		this->last_synapse = &this->synapses[this->n_synapses - 1];

		// Memorizzo il passo della righa
		this->synapses_per_row = prev_n_neurons + 1;
	}

	// Metodo distruttore
	~Layer() {

		// Elimino i neuroni dello strato
		if ( this->n_neurons )
			delete this->neurons;

		// Elimino le sinapsi dello strato
		if ( this->n_synapses )
			delete this->synapses;
	}
};

} // Chiudo il namespace di Serotonina

#endif
