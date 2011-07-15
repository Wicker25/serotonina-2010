/* 
    Title --- trainer.cpp

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

#ifndef _SEROTONINA_TRAINER_CPP_
#define _SEROTONINA_TRAINER_CPP_

#include <serotonina/trainer.hpp>
#include <serotonina/trainer-inl.hpp>

namespace Serotonina { // Namespace di Serotonina

/* INIZIO METODI STATICI */

int
Trainer::training_report(	Network &network, size_t epochs, time_t elapsed_time, T_Precision current_error,
							const std::vector< T_Precision > &outputs, void *data ) {

	// Calcolo le ore, i minuti e i secondi del tempo trascorso
	size_t hours, mins, secs;

	secs	= elapsed_time % 60;
	mins	= ( elapsed_time / 60 ) % 60;
	hours	= ( elapsed_time / 3600 );

	// Stampo il rapporto dell'addestramento
	printf(	" - Epoch #%lu, time %luh%lum%lus, error %.10f\n",
			(unsigned long) epochs, (unsigned long) hours, (unsigned long) mins, (unsigned long) secs, (double) current_error );

	return 0;
}

/* FINE METODI STATICI */


Trainer::Trainer( Network *network_ ) {

	// Memorizzo l'indirizzo della rete neurale
	this->SetNetwork( network_ );

	// Inizializzo i pesi sinaptici della rete neurale
	this->InitWeights();

	// Imposto la funzione di report dell'addestramento
	this->report_fun = &Trainer::training_report;
}

Trainer::Trainer( Network &network_ ) {

	// Memorizzo l'indirizzo della rete neurale
	this->SetNetwork( network_ );

	// Inizializzo i pesi sinaptici della rete neurale
	this->InitWeights();

	// Imposto la funzione di report dell'addestramento
	this->report_fun = &Trainer::training_report;
}

Trainer::Trainer( Network *network_, bool init_weights ) {

	// Memorizzo l'indirizzo della rete neurale
	this->SetNetwork( network_ );

	// Controllo se si è scelto di inizializzare i pesi sinaptici della rete
	if ( init_weights ) {

		// Inizializzo i pesi sinaptici della rete neurale
		this->InitWeights();
	}

	// Imposto la funzione di report dell'addestramento
	this->report_fun = &Trainer::training_report;
}

Trainer::Trainer( Network &network_, bool init_weights ) {

	// Memorizzo l'indirizzo della rete neurale
	this->SetNetwork( network_ );

	// Controllo se si è scelto di inizializzare i pesi sinaptici della rete
	if ( init_weights ) {

		// Inizializzo i pesi sinaptici della rete neurale
		this->InitWeights();
	}

	// Imposto la funzione di report dell'addestramento
	this->report_fun = &Trainer::training_report;
}

Trainer::~Trainer() {

}

void
Trainer::InitWeights() {

	// Iteratori
	short int t = ( this->network->GetLayers().size() - 1 ) - 1;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *end_synapse_t;

	// Aggiorno tutti i pesi sinaptici della rete usando la Regola Delta con il Momentum
	for ( ; t >= 0; t-- ) {

		// Preparo l'iteratore delle sinapsi
		synapse_t = this->network->GetLayer(t + 1).first_synapse;

		// Ricavo la sinapsi finale
		end_synapse_t = this->network->GetLayer(t + 1).last_synapse;

		// Ciclo per tutti i pesi sinaptici tra i due strati
		for ( ; synapse_t <= end_synapse_t; synapse_t++ ) {

			// Inizializzo il peso sinaptico
			synapse_t->weight = rand0to1< T_Precision >();
		}
	}
}

void
Trainer::ComputeOutputError( const T_Precision *desired_output ) {

	// Iteratore
	size_t i = 0;

	// Iteratori dei neuroni
	Neuron *neuron_j, *last_neuron_j;
	Neuron *neuron_i, *last_neuron_i;

	// Iteratori delle sinapsi
	Synapse *synapse_t;

	// Preparo l'iteratore dei neuroni
	last_neuron_i	= this->network->GetLayers().back()->last_neuron;
	neuron_i		= this->network->GetLayers().back()->first_neuron;

	// Preparo l'iteratore delle sinapsi
	synapse_t = this->network->GetLayers().back()->first_synapse;

	// Calcolo l'errore dello strato di uscita
	for ( ; neuron_i <= last_neuron_i; i++, neuron_i++ ) {

		// Calcolo l'errore dell'uscita
		// dE/dy_i = -(D_i - Y_i)
		neuron_i->dEdy = - ( desired_output[i] - neuron_i->value );

		// Preparo l'iteratore dei neuroni
		last_neuron_j	= this->network->GetLayer( this->network->GetLayers().size() - 2 ).last_neuron;
		neuron_j		= this->network->GetLayer( this->network->GetLayers().size() - 2 ).first_neuron;

		// Calcolo l'errore dei pesi sinaptici del neurone
		// dE/dw_ji += dE/dy_i * dy_i/dP_i * dP_i/dw_ji

		// Ciclo per tutti i neuroni del penultimo strato
		for ( ; neuron_j <= last_neuron_j; neuron_j++, synapse_t++ ) {

			synapse_t->dEdw += neuron_i->dEdy * d_sigmoid( neuron_i->value ) * neuron_j->value;
		}

		// Aggiungo l'errore del bias al peso sinaptico del neurone
		// dE/dw_ji += dE/dy_i * dy_i/dP_i
		synapse_t->dEdw += neuron_i->dEdy * d_sigmoid( neuron_i->value );

		// Sposto l'iteratore oltre la sinapsi del bias, nella riga contente le sinapsi del neurone successivo
		synapse_t++;

		// Aggiungo il quadrato dell'errore all'errore totale della rete (per il calcolo dell'errore quadratico medio)
		this->net_error += pow2( neuron_i->dEdy );
	}
}

void
Trainer::BackpropagateError() {

	// Iteratori
	short int t = ( (short int) this->network->GetLayers().size() - 1 ) - 2;

	// Delta per la correzione del peso sinaptico
	T_Precision delta;

	// Iteratori dei neuroni
	Neuron *neuron_j, *last_neuron_j;
	Neuron *neuron_i, *last_neuron_i;
	Neuron *neuron_k, *last_neuron_k;

	// Iteratori delle sinapsi
	Synapse *synapse_t;
	Synapse *synapse_row_t1, *synapse_t1;

	// Retropropago l'errore nella rete
	for ( ; t >= 0; t-- ) {

		// Preparo gli iteratori delle sinapsi
		synapse_t		= this->network->GetLayer(t + 1).first_synapse;
		synapse_row_t1	= this->network->GetLayer(t + 2).first_synapse;

		// Preparo l'iteratore dei neuroni
		last_neuron_j	= this->network->GetLayer(t + 1).last_neuron;
		neuron_j		= this->network->GetLayer(t + 1).first_neuron;

		// Ciclo per tutti i neuroni dello strato 't + 1'
		for ( ; neuron_j <= last_neuron_j; neuron_j++, synapse_row_t1++ ) {

			// Azzero l'errore sull'uscita 
			neuron_j->dEdy = 0;

			// Preparo l'iteratore delle sinapsi
			synapse_t1 = synapse_row_t1;

			// Preparo l'iteratore dei neuroni
			last_neuron_k	= this->network->GetLayer(t + 2).last_neuron;
			neuron_k		= this->network->GetLayer(t + 2).first_neuron;

			// Calcolo l'errore retropropagato dai neuroni dello strato successivo
			// dE/dz_k = SUM( dE/dy_j * dy_j/dP_j * dP_j/dz_k )

			// Ciclo per tutti i neuroni dello strato 't + 2'
			for ( ; neuron_k <= last_neuron_k; neuron_k++, synapse_t1 += this->network->GetLayer(t + 2).synapses_per_row ) {

				neuron_j->dEdy += neuron_k->dEdy * d_sigmoid( neuron_k->value ) * synapse_t1->weight;
			}

			// Calcolo il delta per la correzione del peso sinaptico
			delta = neuron_j->dEdy * d_sigmoid( neuron_j->value );

			// Preparo l'iteratore dei neuroni
			last_neuron_i	= this->network->GetLayer(t).last_neuron;
			neuron_i		= this->network->GetLayer(t).first_neuron;

			// Ciclo per tutti i neuroni dello strato 't'
			for ( ; neuron_i <= last_neuron_i; neuron_i++, synapse_t++ ) {

				// dE/dw_ij += dE/dy_j * dy_j/dP_j * Z_i
				synapse_t->dEdw += delta * neuron_i->value;
			}

			// Aggiungo l'errore del bias al peso sinaptico del neurone
			// dE/dw_ij += dE/dy_j * dy_j/dP_j
			synapse_t->dEdw += delta;

			// Sposto l'iteratore oltre la sinapsi del bias, nella riga contente le sinapsi del neurone successivo
			synapse_t++;
		}
	}
}

std::ostream &
operator<<( std::ostream &out, Trainer &trainer ) {

	// Scrivo la struttura dell'addestratore nello stream
	out << "<Trainer>";

	return out;
}

} // Chiudo il namespace di Serotonina

#endif
