/* 
    Title --- network.hpp

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

// Segnale di apprendimento
typedef int (*ReportSignal)( Network *, size_t, time_t, T_Precision, const T_Precision *, size_t, void * );

class Network {

	/* RETE NEURALE */

public:

	/** INIZIO METODI STATICI **/

	// Stampa i rapporti dell'addestramento 
	static int training_report(	Network *network, size_t epochs, time_t elapsed_time, T_Precision max_error,
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
				T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports );

	// Addestra la rete neurale usando degli esempi in un file
	void TrainOnFile( const std::string &training_file, T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports );

	// Salva la rete neurale in un file
	void Save( const std::string &path );

	// Carica la rete neurale da un file
	void Load( const std::string &path );

	// Imposta la funzione di report dell'addestramento
	void SetReportSignal( ReportSignal function, void *data );

private:

	// Strati della rete
	std::vector< Layer *> layers;

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
