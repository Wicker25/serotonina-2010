/* 
    Title --- gym.hpp

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

#ifndef _SEROTONINA_GYM_HPP_
#define _SEROTONINA_GYM_HPP_

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <pthread.h>
#include <signal.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_File_Chooser.H>

#include <serotonina.hpp>

#include "extra/version.hpp"
#include "extra/plot.hpp"
#include "extra/plot-inl.hpp"

#define _GYM_TITLE_		"Serotonina::Gym - Tool grafico per l'addestramento delle reti neurali"
#define _GYM_FPS_		30.0

// Linea di comando
#define _GYM_CMD_TRAINING_SET_	"--training-set"

class Gym : public Fl_Window {

	/* STRUMENTO GRAFICO PER L'ADDESTRAMENTO DELLE RETI NEURALI */

public:

	/** INIZIO METODI STATICI **/

	// Funzione per l'aggiornamento asincrono della finestra
	static void asynchronous_redraw( void *window );

	// Funzione di callback del contatore del numero degli strati nascosti
	static void set_hidden_layers_button_callback( Fl_Widget *widget, void *data );

	// Funzione di callback del selettore dell'algoritmo di apprendimento
	static void set_training_algorithm_button_callback( Fl_Widget *widget, void *data );

	// Funzione di callback del pulsante "carica training set"
	static void load_training_set_button_callback( Fl_Widget *widget, void *data );

	// Funzione di callback del pulsante "carica test set"
	static void load_test_set_button_callback( Fl_Widget *widget, void *data );

	// Funzione di callback del pulsante "addestra"
	static void training_button_callback( Fl_Widget *widget, void *data );

	// Funzione di callback del pulsante "salva"
	static void save_button_callback( Fl_Widget *widget, void *data );

	// Avvia l'addestramento della rete neurale (statico)
	static void *static_start_training( void *data );

	// Aggiorna i grafici della rete neurale (statico)
	static int static_update_plot(	Serotonina::Network *network, size_t epochs, time_t elapsed_time, Serotonina::T_Precision max_error,
									const Serotonina::T_Precision *outputs, size_t outputs_size, void *data );

	/** FINE METODI STATICI **/


	// Metodi costruttore e distruttore
	Gym( int width, int height );
	virtual ~Gym();

	// Interpreta la linea di comando
	void CommandLine( int argc, char **argv );

	// Avvia l'addestramento della rete neurale
	void StartTraining();

	// Carica i campioni per l'addestramento della rete
	void LoadTrainingSet( const char *path );

	// Carica i campioni per il test della rete
	void LoadTestSet( const char *path );

	// Salva la rete neurale in un file
	void SaveNeuralNetwork( const char *path );

	// Aggiorna i grafici della rete neurale
	int UpdatePlot(	Serotonina::Network *network, size_t epochs, time_t elapsed_time, Serotonina::T_Precision max_error,
					const Serotonina::T_Precision *outputs, size_t outputs_size );

	// Funzione per la gestione degli eventi della finestra
	virtual int handle( int event );

private:

	// Rete neurale
	Serotonina::Network *neural_network;

	// Id del thread dell'addestramento
	pthread_t thread_id;

	// Percorso al training set
	std::string train_set_path;

	// Numero dei campioni del test set
	size_t n_test_samples;

	// Dati sulla rete da addestrare
	size_t input_size;
	size_t output_size;

	// Ingressi del training set
	std::vector< Serotonina::T_Precision > inputs_data;

	// Uscite del training set
	std::vector< Serotonina::T_Precision > outputs_data;

	// Dati sull'errore massimo
	std::vector< Serotonina::T_Precision > error_data;

	// Uscita mostrata nel grafico
	size_t graph_output;

	// Grafico delle uscite
	Plot *outputs_plot;

	// Grafico dell'errore massimo
	Plot *error_plot;

	// Casella del numero degli strati
	Fl_Spinner *n_layers_input;

	// Caselle del numero dei neuroni nascosti
	Fl_Spinner *hidden_neurons_input0;
	Fl_Spinner *hidden_neurons_input1;
	Fl_Spinner *hidden_neurons_input2;

	// Casella dell'algoritmo di apprendimento
	Fl_Choice *train_algorithm_input;

	// Casella dell'errore desiderato
	Fl_Spinner *desired_error_input;

	// Casella del tasso di apprendimento
	Fl_Spinner *learning_rate_input;

	// Casella del momentum
	Fl_Spinner *momentum_input;

	// Casella del fattore di incremento
	Fl_Spinner *increase_factor_input;

	// Casella del fattore di decremento
	Fl_Spinner *decrease_factor_input;

	// Casella delle epoche massime
	Fl_Spinner *max_epochs_input;

	// Casella della frequenza di report (in numero di epoche)
	Fl_Spinner *report_frequency_input;

	// Riquadro del numero delle epoche
	Fl_Value_Output *epochs_box;

	// Riquadro dell'errore massimo
	Fl_Value_Output *error_box;

	// Pulsante per il caricamento del training set
	Fl_Button *load_training_set_button;

	// Pulsante per il caricamento del test set
	Fl_Button *load_test_set_button;

	// Pulsante per l'addestramento
	Fl_Button *training_button;

	// Pulsante per salvare la rete neurale
	Fl_Button *save_button;

	// Buffer del log di lavoro
	Fl_Text_Buffer *log_buffer;

	// Finestra del log di lavoro
	Fl_Text_Display *log_display;
};

#endif
