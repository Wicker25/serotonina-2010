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

#include <serotonina/train-algorithm.hpp>
#include <serotonina/train-algorithm-inl.hpp>

namespace Serotonina { // Namespace di Serotonina

/**
 * \typedef T_ReportFun
 * \if english
 *   @brief Reference to a report function
 *   @param[in] network The neural network passed by Trainer
 *   @param[in] epoch The current epoch
 *   @param[in] elapsed_time The elapsed time since start of training
 *   @param[in] current_error The current error of the neural network
 *   @param[in] outputs The outputs of the neural network
 *   @param[in] data Pointer to report data
 * \endif
 * \if italian
 *   @brief Riferimento ad una funzione di report
 *   @param[in] network La rete neurale passata dal Trainer
 *   @param[in] epoch L'epoca corrente
 *   @param[in] elapsed_time Il tempo trascorso dall'inizio dell'addestramento
 *   @param[in] current_error L'errore corrente della rete neurale
 *   @param[in] outputs Le uscite della rete neurale
 *   @param[in] data Puntatore alle informazioni del report
 * \endif
 */

// Funzione di report dell'apprendimento
typedef int (T_ReportFun)( Network &, size_t, time_t, T_Precision, const std::vector< T_Precision > &, void * );

/*!
 * @class Trainer
 * \if english
 *   @brief Class of trainer
 * \endif
 * \if italian
 *   @brief Classe dell'addestratore
 * \endif
 */

class Trainer {

	/* ADDESTRATORE DELLA RETE NEURALE */

public:

	/* INIZIO METODI STATICI */

	/**
	 * \if english
	 *   @brief Default report's function
	 *   @param[in] network The neural network passed by Trainer
	 *   @param[in] epochs The current epoch
	 *   @param[in] elapsed_time The elapsed time since start of training
	 *   @param[in] current_error The current error of the neural network
	 *   @param[in] outputs The outputs of the neural network
	 *   @param[in] data Pointer to report data
	 * \endif
	 * \if italian
	 *   @brief Funzione predefinita per il report dell'addestramento
	 *   @param[in] network La rete neurale passata dal Trainer
	 *   @param[in] epochs L'epoca corrente
	 *   @param[in] elapsed_time Il tempo trascorso dall'inizio dell'addestramento
	 *   @param[in] current_error L'errore corrente della rete neurale
	 *   @param[in] outputs Le uscite della rete neurale
	 *   @param[in] data Puntatore alle informazioni del report
	 * \endif
	 */

	// Stampa i rapporti dell'addestramento 
	static int training_report(	Network &network, size_t epochs, time_t elapsed_time, T_Precision current_error,
								const std::vector< T_Precision > &outputs, void *data );

	/* FINE METODI STATICI */

	/**
	 * \if english
	 *   @brief Constructor method
	 *   @param[in] network_ Pointer to the neural network to be trained
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore
	 *   @param[in] network_ Puntatore alla rete neurale da addestrare
	 * \endif
	 */

	// Metodo costruttore
	Trainer( Network *network_ );

	/**
	 * \if english
	 *   @brief Constructor method
	 *   @param[in] network_ Reference to the neural network to be trained
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore
	 *   @param[in] network_ Riferimento alla rete neurale da addestrare
	 * \endif
	 */

	// Metodo costruttore
	Trainer( Network &network_ );

	/**
	 * \if english
	 *   @brief Constructor method
	 *   @param[in] network_ Pointer to the neural network to be trained
	 *   @param[in] init_weights If 'true' the weights of the neural network will be initialized
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore
	 *   @param[in] network_ Puntatore alla rete neurale da addestrare
	 *   @param[in] init_weights Se è 'true' i pesi della rete neurale verranno inizializzati
	 * \endif
	 */

	// Metodo costruttore
	Trainer( Network *network_, bool init_weights );

	/**
	 * \if english
	 *   @brief Constructor method
	 *   @param[in] network_ Reference to the neural network to be trained
	 *   @param[in] init_weights If 'true' the weights of the neural network will be initialized
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore
	 *   @param[in] network_ Riferimento alla rete neurale da addestrare
	 *   @param[in] init_weights Se è 'true' i pesi della rete neurale verranno inizializzati
	 * \endif
	 */

	// Metodo costruttore
	Trainer( Network &network_, bool init_weights );

	// Metodo distruttore
	virtual ~Trainer();

	/**
	 * \if english
	 *   @brief Set the neural network to be trained
	 *   @param[in] network_ Pointer to the neural network to be trained
	 * \endif
	 * \if italian
	 *   @brief Imposta la rete neurale da addestrare
	 *   @param[in] network_ Puntatore alla rete neurale da addestrare
	 * \endif
	 */

	// Imposta la rete neurale da addestrare
	void SetNetwork( Network *network_ );

	/**
	 * \if english
	 *   @brief Set the neural network to be trained
	 *   @param[in] network_ Reference to the neural network to be trained
	 * \endif
	 * \if italian
	 *   @brief Imposta la rete neurale da addestrare
	 *   @param[in] network_ Riferimento alla rete neurale da addestrare
	 * \endif
	 */

	// Imposta la rete neurale da addestrare
	void SetNetwork( Network &network_ );

	/**
	 * \if english
	 *   @brief Initialize the weights of the neural network
	 * \endif
	 * \if italian
	 *   @brief Inizializza i pesi della rete neurale
	 * \endif
	 */

	// Inizializzo i pesi sinaptici della rete
	void InitWeights();

	/**
	 * \if english
	 *   @brief Set parameters of training
	 *   @param[in] p0 Parameter #1
	 * \endif
	 * \if italian
	 *   @brief Imposta i parametri dell'addestramento
	 *   @param[in] p0 Parametro #1
	 * \endif
	 */

	// Imposta i parametri dell'addestramento
	void SetParameters( T_Precision p0 );

	/**
	 * \if english
	 *   @brief Set parameters of training
	 *   @param[in] p0 Parameter #1
	 *   @param[in] p1 Parameter #2
	 * \endif
	 * \if italian
	 *   @brief Imposta i parametri dell'addestramento
	 *   @param[in] p0 Parametro #1
	 *   @param[in] p1 Parametro #2
	 * \endif
	 */

	// Imposta i parametri dell'addestramento
	void SetParameters( T_Precision p0, T_Precision p1 );

	/**
	 * \if english
	 *   @brief Set parameters of training
	 *   @param[in] p0 Parameter #1
	 *   @param[in] p1 Parameter #2
	 *   @param[in] p2 Parameter #3
	 * \endif
	 * \if italian
	 *   @brief Imposta i parametri dell'addestramento
	 *   @param[in] p0 Parametro #1
	 *   @param[in] p1 Parametro #2
	 *   @param[in] p2 Parametro #3
	 * \endif
	 */

	// Imposta i parametri dell'addestramento
	void SetParameters( T_Precision p0, T_Precision p1, T_Precision p2 );

	/**
	 * \if english
	 *   @brief Set parameters of training
	 *   @param[in] p0 Parameter #1
	 *   @param[in] p1 Parameter #2
	 *   @param[in] p2 Parameter #3
	 *   @param[in] p3 Parameter #4
	 * \endif
	 * \if italian
	 *   @brief Imposta i parametri dell'addestramento
	 *   @param[in] p0 Parametro #1
	 *   @param[in] p1 Parametro #2
	 *   @param[in] p2 Parametro #3
	 *   @param[in] p3 Parametro #4
	 * \endif
	 */

	// Imposta i parametri dell'addestramento
	void SetParameters( T_Precision p0, T_Precision p1, T_Precision p2, T_Precision p3 );

	/**
	 * \if english
	 *   @brief Set parameters of training
	 *   @param[in] p0 Parameter #1
	 *   @param[in] p1 Parameter #2
	 *   @param[in] p2 Parameter #3
	 *   @param[in] p3 Parameter #4
	 *   @param[in] p4 Parameter #5
	 * \endif
	 * \if italian
	 *   @brief Imposta i parametri dell'addestramento
	 *   @param[in] p0 Parametro #1
	 *   @param[in] p1 Parametro #2
	 *   @param[in] p2 Parametro #3
	 *   @param[in] p3 Parametro #4
	 *   @param[in] p4 Parametro #5
	 * \endif
	 */

	// Imposta i parametri dell'addestramento
	void SetParameters( T_Precision p0, T_Precision p1, T_Precision p2, T_Precision p3, T_Precision p4 );

	/**
	 * \if english
	 *   @brief Set parameters of training
	 *   @param[in] params Vector containing parameters of training
	 * \endif
	 * \if italian
	 *   @brief Imposta i parametri dell'addestramento
	 *   @param[in] params Vettore contenente i parametri dell'addestramento
	 * \endif
	 */

	// Imposta i parametri dell'addestramento
	void SetParameters( const std::vector< T_Precision > &params );

	/**
	 * \if english
	 *   @brief Returns parameters of training
	 *   @return Vector containing parameters of training
	 * \endif
	 * \if italian
	 *   @brief Ritorna i parametri dell'addestramento
	 *   @return Vettore contenente i parametri dell'addestramento
	 * \endif
	 */

	// Ritorna i parametri dell'addestramento
	const std::vector< T_Precision > &GetParameters() const;

	/**
	 * \if english
	 *   @brief Returns a parameter of training
	 *   @return The parameter of training
	 * \endif
	 * \if italian
	 *   @brief Ritorna un parametro dell'addestramento
	 *   @return Il parametro dell'addestramento
	 * \endif
	 */

	// Ritorna un parametro dell'addestramento
	const T_Precision GetParameter( size_t n ) const;

	/**
	 * \if english
	 *   @brief Set the report's function
	 *   @param[in] fun Reference to the report's function
	 * \endif
	 * \if italian
	 *   @brief Imposta la funzione di report
	 *   @param[in] fun Riferimento alla funzione di report
	 * \endif
	 */

	// Imposta la funzione di report dell'addestramento
	void SetReportFun( T_ReportFun &fun );

	/**
	 * \if english
	 *   @brief Returns the report's function
	 * \endif
	 * \if italian
	 *   @brief Ritorna la funzione di report
	 * \endif
	 */

	// Ritorna la funzione di report dell'addestramento
	T_ReportFun *GetReportFun() const;

	/**
	 * \if english
	 *   @brief Set data used by report's function
	 *   @param[in] data Pointer to data used by report's function
	 * \endif
	 * \if italian
	 *   @brief Imposta le informazioni usate dalla funzione di report
	 *   @param[in] data Puntatore alle informazioni usate dalla funzione di report
	 * \endif
	 */

	// Imposta il parametro ausiliario della funzione di report
	void SetReportFunData( void *data );

	/**
	 * \if english
	 *   @brief Returns data used by report's function
	 * \endif
	 * \if italian
	 *   @brief Ritorna le informazioni usate dalla funzione di report
	 * \endif
	 */

	// Ritorna le informazioni usate per la funzione di report
	void *GetReportFunData() const;

	/**
	 * \if english
	 *   @brief Train the neural network with examples
	 *   @tparam train_algorithm Training's algorithm
	 *   @param[in] input_samples Vector containing the examples of input
	 *   @param[in] output_samples Vector containing the examples of output
	 *   @param[in] n_samples Number of examples
	 *   @param[in] desired_error Desired error for the neural network
	 *   @param[in] max_epochs Numero massimo di epoche
	 *   @param[in] epochs_between_reports Numero di epoche tra un report e l'altro
	 * \endif
	 * \if italian
	 *   @brief Addestra la rete neurale usando degli esempi
	 *   @tparam train_algorithm Algoritmo dell'apprendimento
	 *   @param[in] input_samples Vettore contenente gli esempi di ingresso
	 *   @param[in] output_samples Vettore contenente gli esempi di uscita
	 *   @param[in] n_samples Numero degli esempi
	 *   @param[in] desired_error Errore desiderato per la rete neurale
	 *   @param[in] max_epochs Numero massimo di epoche
	 *   @param[in] epochs_between_reports Numero di epoche tra un report e l'altro
	 * \endif
	 */

	// Addestra la rete neurale usando degli esempi
	template < class train_algorithm >
	void Train( const T_Precision *input_samples, const T_Precision *output_samples, size_t n_samples, 
				T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports );

	/**
	 * \if english
	 *   @brief Train the neural network with examples
	 *   @tparam train_algorithm Training's algorithm
	 *   @param[in] input_samples Vector containing the examples of input
	 *   @param[in] output_samples Vector containing the examples of output
	 *   @param[in] desired_error Desired error for the neural network
	 *   @param[in] max_epochs Numero massimo di epoche
	 *   @param[in] epochs_between_reports Numero di epoche tra un report e l'altro
	 * \endif
	 * \if italian
	 *   @brief Addestra la rete neurale usando degli esempi
	 *   @tparam train_algorithm Algoritmo dell'apprendimento
	 *   @param[in] input_samples Vettore contenente gli esempi di ingresso
	 *   @param[in] output_samples Vettore contenente gli esempi di uscita
	 *   @param[in] desired_error Errore desiderato per la rete neurale
	 *   @param[in] max_epochs Numero massimo di epoche
	 *   @param[in] epochs_between_reports Numero di epoche tra un report e l'altro
	 * \endif
	 */

	// Addestra la rete neurale usando degli esempi
	template < class train_algorithm >
	void Train( const std::vector< T_Precision > &input_samples, const std::vector< T_Precision > &output_samples, 
				T_Precision desired_error, size_t max_epochs, size_t epochs_between_reports );

	/**
	 * \if english
	 *   @brief Train the neural network with examples from a file
	 *   @tparam train_algorithm Training's algorithm
	 *   @param[in] training_file Path to the file containing training set
	 *   @param[in] desired_error Desired error for the neural network
	 *   @param[in] max_epochs Numero massimo di epoche
	 *   @param[in] epochs_between_reports Numero di epoche tra un report e l'altro
	 * \endif
	 * \if italian
	 *   @brief Addestra la rete neurale usando degli esempi da un file
	 *   @tparam train_algorithm Algoritmo dell'apprendimento
	 *   @param[in] training_file Percorso al file contenente l'insieme di addestramento
	 *   @param[in] desired_error Errore desiderato per la rete neurale
	 *   @param[in] max_epochs Numero massimo di epoche
	 *   @param[in] epochs_between_reports Numero di epoche tra un report e l'altro
	 * \endif
	 */

	// Addestra la rete neurale usando degli esempi da un file
	template < class train_algorithm >
	void TrainOnFile(	const std::string &training_file, T_Precision desired_error,
						size_t max_epochs, size_t epochs_between_reports );

	/**
	 * \if english
	 *   @brief Returns the error of the neural network
	 *   @return The error of the neural network
	 * \endif
	 * \if italian
	 *   @brief Ritorna l'errore della rete neurale
	 *   @return L'errore corrente della rete neurale
	 * \endif
	 */

	// Ritorna l'errore della rete neurale
	T_Precision GetError() const;

protected:

	/**
	 * \if english
	 *   @brief Pointer to the neural network to be trained
	 * \endif
	 * \if italian
	 *   @brief Puntatore alla rete neurale da addestrare
	 * \endif
	 */

	// Puntatore alla rete da addestrare
	Network *network;

	/**
	 * \if english
	 *   @brief The error of the neural network
	 * \endif
	 * \if italian
	 *   @brief L'errore della rete neurale
	 * \endif
	 */

	// Errore della rete neurale
	T_Precision net_error;

	/**
	 * \if english
	 *   @brief The error of the neural network at previous epoch
	 * \endif
	 * \if italian
	 *   @brief L'errore della rete neurale nell'epoca precedente
	 * \endif
	 */

	// Errore della rete neurale nell'epoca precedente
	T_Precision prev_net_error;

	/**
	 * \if english
	 *   @brief Vector containing parameters of training
	 * \endif
	 * \if italian
	 *   @brief Vettore contenente i parametri dell'addestramento
	 * \endif
	 */

	// Parametri dell'addestramento
	std::vector< T_Precision > train_params;

	/**
	 * \if english
	 *   @brief Pointer to the report's function
	 * \endif
	 * \if italian
	 *   @brief Puntatore alla funzione di report
	 * \endif
	 */

	// Funzione di report dell'addestramento
	T_ReportFun *report_fun;

	/**
	 * \if english
	 *   @brief Pointer to the data of the report's function
	 * \endif
	 * \if italian
	 *   @brief Puntatore alle informazioni della funzione di report
	 * \endif
	 */

	// Parametro ausiliario della funzione di report
	void *report_fun_data;

	/**
	 * \if english
	 *   @brief Compute the error of the network's outputs
	 *   @param[in] desired_output Vector containing the examples of output
	 * \endif
	 * \if italian
	 *   @brief Calcola l'errore delle uscite
	 *   @param[in] desired_output Vettore contenente gli esempi di uscita
	 * \endif
	 */

	// Calcola l'errore delle uscite
	void ComputeOutputError( const T_Precision *desired_output );

	/**
	 * \if english
	 *   @brief Backpropagate the error of the neural network
	 * \endif
	 * \if italian
	 *   @brief Retropropaga l'errore della rete neurale
	 * \endif
	 */

	// Retropropaga l'errore della rete neurale
	void BackpropagateError();
};

} // Chiudo il namespace di Serotonina

#endif
