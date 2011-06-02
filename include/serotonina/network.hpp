/* 
    Title --- network.hpp

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

#ifndef _SEROTONINA_NETWORK_HPP_
#define _SEROTONINA_NETWORK_HPP_

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#include <vector>

#include <cmath>
#include <ctime>

#include <serotonina/version.hpp>

#define _COMMENT_		'#'
#define _SEPARATOR_		':'

#define __ABS__(x)			((x > 0) ? x : -x)
#define __MIN__(x,y)		((x < y) ? x : y)
#define __MAX__(x,y)		((x > y) ? x : y)
#define __BETWEEN__(x,y,z)	((x < y) ? y : ((x > z) ? z : x))
#define __SIGN__(x)			((x == 0) ? 0 : (x > 0) ? +1 : -1)

#define __POW2__(x)			(x * x)
#define __POW3__(x)			(x * x * x)

#define __SIGMOID__(x)		1.0 / ( 1.0 + std::exp(-x) )
#define __D_SIGMOID__(x)	x * ( 1 - x )

namespace Serotonina { // Namespace di Serotonina

// Algoritmi di apprendimento
enum TrainAlgorithm {

	TRAIN_BATCH,
	TRAIN_RPROP,
	TRAIN_RPROP_PLUS,
	TRAIN_RPROP_MINUS,
	TRAIN_IRPROP_PLUS,
	TRAIN_IRPROP_MINUS
};

// Prototipi
class Network;

// Precisione dei valori
//typedef float Precision;
typedef double T_Precision;

// Segnale di apprendimento
typedef int (*ReportSignal)( Network *, size_t, T_Precision, const T_Precision *, size_t, void * );

// Genera un numero casuale tra 0 e 1
T_Precision get_rand();

// Alloca dinamicamente un vettore a due dimensioni
template < typename TYPE >
TYPE **malloc_2dim( size_t n, size_t m );

// Dealloca dinamicamente un vettore a due dimensioni
template < typename TYPE >
void dealloc_2dim( TYPE **vect, size_t size );

// Estrae i valori numerici da una stringa di testo
size_t values_on_string( const std::string &str, std::vector< T_Precision > &vect );


// Struttura di un neurone della rete
struct Neuron {

	T_Precision value;	// Valore di trasferimento
	T_Precision dEdy;	// Errore del neurone
};

// Struttura contenente i dati per l'addestramento
struct TrainData {

	T_Precision delta_weight;	// Spostamento del peso della connessione
	T_Precision dEdw;			// Errore della connessione
	T_Precision prev_dEdw;		// Errore precedente della connessione
	T_Precision learning_rate;	// Tasso di apprendimento della connessione (per i metoti RPROP)

	// Metodo costruttore
	TrainData() {

		// Inizializzo i parametri dell'addestramento
		delta_weight	= 0.0;
		dEdw			= 0.0;
		prev_dEdw		= 0.0;
		learning_rate	= 0.1;
	}
};

// Struttura di una connessione tra due neuroni (sinapsi)
struct Synapse {

	T_Precision weight;		// Peso della connessione
	TrainData *train;	// Dati per l'addestramento

	// Metodo costruttore
	Synapse() {

		// Inizializzo il peso sinaptico con un valore casuale
		weight = get_rand();
	}
};

// Struttura di uno strato della rete
struct Layer {

	// Dimensione dello strato
	size_t size;

	// Neuroni dello strato (e puntatore al primo neurone dello strato)
	union { Neuron *neurons, *first_neuron; };

	// Puntatore all'ultimo neurone dello strato
	Neuron *last_neuron;

	// Metodo costruttore
	Layer( size_t size_ ) {

		// Memorizzo la dimensione dello strato
		this->size = size_;

		// Creo i neuroni dello strato
		this->neurons = new Neuron[this->size];

		// Imposto il puntatore all'ultimo neurone
		this->last_neuron = &this->neurons[this->size - 1];
	}

	// Metodo distruttore
	~Layer() {

		// Elimino i neuroni dello strato
		delete this->neurons;
	}
};

// Struttura di una connessione tra due strati
struct LayerConnections {

	// Dimensione dello strato
	size_t size;

	// Sinapsi tra i due strati (e puntatore alla prima sinapsi)
	union { Synapse *synapses, *first_synapse; };

	// Puntatore all'ultima sinapsi
	Synapse *last_synapse;

	// Metodo costruttore
	LayerConnections( size_t first_layer_size, size_t second_layer_size ) {

		// Memorizzo la dimensione dello strato
		this->size = first_layer_size * second_layer_size;

		// Creo le sinapsi tra i due strati
		this->synapses = new Synapse[this->size];

		// Imposto il puntatore all'ultima prima sinapsi
		this->last_synapse = &this->synapses[this->size - 1];
	}

	// Metodo distruttore
	~LayerConnections() {

		// Elimino le sinapsi tra i due strati
		delete this->synapses;
	}
};


class Network {

	/* RETE NEURALE */

public:

	/** INIZIO METODI STATICI **/

	// Stampa i rapporti dell'addestramento 
	static int training_report(	Network *network, size_t epochs, T_Precision max_error,
									const T_Precision *outputs, size_t outputs_size, void *data );
	/** FINE METODI STATICI **/


	// Metodi costruttore e distruttore
	Network( size_t n_layers, ... );
	Network( const std::string &path );
	virtual ~Network();

	// Imposta l'algoritmo di addestramento
	void SetTrainingAlgorithm( TrainAlgorithm algorithm );

	// Imposta il tasso di apprendimento e il momentum
	void SetTrainingParameters( T_Precision learning_rate_, T_Precision momentum_ );

	// Imposta i fattori di incremento e decremento del RPROP
	void SetRpropFactor( T_Precision increase_factor_, T_Precision decrease_factor_ );

	// Ritorna l'errore della rete
	T_Precision GetError() const;

	// Reinizializza i pesi sinaptici con valori casuali
	void InizializeWeight();

	// Esegue la rete neurale
	const T_Precision *Run( const T_Precision *input );

	// Addestra la rete neurale usando degli esempi
	void Train( const T_Precision *input_samples, const T_Precision *output_samples, size_t n_samples, 
				T_Precision target_error, size_t max_epochs, size_t epochs_between_reports );

	// Addestra la rete neurale usando degli esempi in un file
	void TrainOnFile( const std::string &training_file, T_Precision target_error, size_t max_epochs, size_t epochs_between_reports );

	// Salva la rete neurale in un file
	void Save( const std::string &path );

	// Carica la rete neurale da un file
	void Load( const std::string &path );

	// Imposta la funzione di report dell'addestramento
	void SetReportSignal( ReportSignal function, void *data );

private:

	// Strati della rete
	std::vector< Layer *> layers;

	// Connessioni tra gli strati
	std::vector< LayerConnections * > connections;

	// Dati di uscita
	T_Precision *output_data;

	// Errore della rete
	T_Precision net_error, prev_net_error;

	// Tipo di appredimento
	TrainAlgorithm train_algorithm;

	// Tasso di appredimento e momentum
	T_Precision learning_rate, momentum;

	// Fattore di incremento e decremento del RPROP
	T_Precision increase_factor, decrease_factor;

	// Funzione di report dell'addestramento
	ReportSignal report_function;

	// Parametro ausiliario della funzione di report
	void *report_function_data;


	// Calcola l'errore delle uscite
	void ComputeError( const T_Precision *target );

	// Retropropaga l'errore nella rete
	void BackpropagateError();

	// Crea le strutture per l'addestramento
	void CreateTrainData();

	// Cancella le strutture per l'addestramento
	void DeleteTrainData();

	// Aggiorna i pesi sinaptici utilizzando la Regola Delta con il Momentum
	void UpdateWeightsBatch();

	// Aggiorna i pesi sinaptici utilizzando la RPROP
	void UpdateWeightsRprop();

	// Aggiorna i pesi sinaptici utilizzando la RPROP+
	void UpdateWeightsRpropPlus();

	// Aggiorna i pesi sinaptici utilizzando la RPROP-
	void UpdateWeightsRpropMinus();

	// Aggiorna i pesi sinaptici utilizzando la IRPROP+
	void UpdateWeightsIRpropPlus();

	// Aggiorna i pesi sinaptici utilizzando la IRPROP-
	void UpdateWeightsIRpropMinus();

	// Carica la rete neurale da un file (principale)
	void Load( const std::string &path, bool new_ );
};

} // Chiudo il namespace di Serotonina

#endif
