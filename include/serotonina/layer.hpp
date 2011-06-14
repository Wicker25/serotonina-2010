/* 
    Title --- layer.hpp

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

#ifndef _SEROTONINA_LAYER_HPP_
#define _SEROTONINA_LAYER_HPP_

#include <serotonina/neuron.hpp>
#include <serotonina/synapse.hpp>

namespace Serotonina { // Namespace di Serotonina

// Struttura di uno strato della rete
struct Layer {

	// Numero dei neuroni dello strato
	size_t n_neurons;

	// Neuroni dello strato (e puntatore al primo neurone dello strato)
	union { Neuron *neurons, *first_neuron; };

	// Puntatore all'ultimo neurone dello strato
	Neuron *last_neuron;

	// Numero delle sinapsi dello strato
	size_t n_synapses;

	// Sinapsi tra i due strati (e puntatore alla prima sinapsi)
	union { Synapse *synapses, *first_synapse; };

	// Puntatore all'ultima sinapsi
	Synapse *last_synapse;

	// Numero di sinapsi per riga
	size_t synapses_per_row;


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
