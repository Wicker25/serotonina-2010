/* 
    Title --- trainer.hpp

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

#ifndef _SEROTONINA_TRAINER_HPP_
#define _SEROTONINA_TRAINER_HPP_

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#include <vector>
#include <ctime>

#include <serotonina/version.hpp>
#include <serotonina/types.hpp>
#include <serotonina/macros.hpp>
#include <serotonina/string.hpp>

#include <serotonina/neuron.hpp>
#include <serotonina/synapse.hpp>
#include <serotonina/layer.hpp>

#include <serotonina/network.hpp>
#include <serotonina/network-inl.hpp>

namespace Serotonina { // Namespace di Serotonina

// Funzione di addestramento
typedef void (T_TrainFun)(	Network *network, const std::vector< T_Precision > &train_params,
							T_Precision net_error, T_Precision prev_net_error );

// Funzione di report dell'apprendimento
typedef int (T_ReportFun)( Network *, size_t, time_t, T_Precision, const T_Precision *, size_t, void * );

class Trainer {

	/* ADDESTRATORE DELLA RETE NEURALE */

public:

	/** INIZIO METODI STATICI **/

	// Stampa i rapporti dell'addestramento 
	static int training_report(	Network *network, size_t epochs, time_t elapsed_time, T_Precision max_error,
								const T_Precision *outputs, size_t outputs_size, void *data );

	/** FINE METODI STATICI **/


	// Metodi costruttore e distruttore
	Trainer( Network *network_ );
	Trainer( Network &network_ );
	Trainer( Network *network_, bool init_weights );
	Trainer( Network &network_, bool init_weights );
	virtual ~Trainer();

	// Imposta la rete neurale da addestrare
	void SetNetwork( Network *network_ );
	void SetNetwork( Network &network_ );

	// Inizializzo i pesi sinaptici della rete
	void InitWeights();

	// Imposta i parametri dell'addestramento
	void SetParameters( T_Precision p0 );
	void SetParameters( T_Precision p0, T_Precision p1 );
	void SetParameters( T_Precision p0, T_Precision p1, T_Precision p2 );
	void SetParameters( const std::vector< T_Precision > &params );

	// Imposta la funzione di report dell'addestramento
	void SetReportFun( T_ReportFun &fun, void *data );

	// Addestra la rete neurale usando degli esempi
	template < T_TrainFun train_fun >
	void Train( const T_Precision *input_samples, const T_Precision *output_samples, size_t n_samples, 
				T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports );

	template < T_TrainFun train_fun >
	void Train( const std::vector< T_Precision > &input_samples, const std::vector< T_Precision > &output_samples, 
				T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports );

	// Addestra la rete neurale usando degli esempi da un file
	template < T_TrainFun train_fun >
	void TrainOnFile(	const std::string &training_file, T_Precision desired_error,
						size_t max_epochs, size_t epochs_between_reports );

	// Ritorna l'errore della rete
	T_Precision GetError() const;

protected:

	// Puntatore alla rete da addestrare
	Network *network;

	// Errore della rete
	T_Precision net_error, prev_net_error;

	// Parametri dell'addestramento
	std::vector< T_Precision > train_params;

	// Funzione di report dell'addestramento
	T_ReportFun *report_fun;

	// Parametro ausiliario della funzione di report
	void *report_fun_data;


	// Crea le strutture per l'addestramento
	void CreateTrainData();

	// Cancella le strutture per l'addestramento
	void DeleteTrainData();

	// Calcola l'errore delle uscite
	void ComputeError( const T_Precision *target );

	// Retropropaga l'errore nella rete
	void BackpropagateError();
};

} // Chiudo il namespace di Serotonina

#endif
