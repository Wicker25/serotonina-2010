/* 
    Title --- gym.cpp

	Copyright (C) 2010 Giacomo Trudu - wicker25[at]gmail[dot]com

	This file is part of Serotonina.

	This library is free software; you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
*/

#ifndef _SEROTONINA_GYM_CPP_
#define _SEROTONINA_GYM_CPP_

#include "extra/gym.hpp"

#include "extra/plot.hpp"
#include "extra/plot-inl.hpp"

#include "extra/gym-inl.hpp"

namespace Serotonina { // Namespace di Serotonina

/** INIZIO METODI STATICI **/

void
Gym::asynchronous_redraw( void *window ) {

	// Ricavo la finestra principale
	Gym *main_window = (Gym *) window;

	// Forzo il ridisegno di tutte le finestre OpenGL
	main_window->outputs_plot->redraw();
	main_window->error_plot->redraw();

	// Riavvio la funzione per l'aggiornamento asincrono della finestra
	Fl::repeat_timeout( 1.0 / _GYM_FPS_, Gym::asynchronous_redraw, window );
}

void
Gym::training_button_callback( Fl_Widget *widget, void *data ) {

	// Ricavo un puntatore alla finestra principale
	Gym *gym = static_cast< Gym * >( data );

	// Controllo se l'addestramento è già stato avviato
	if ( gym->thread_id == 0 ) { 

		// Controllo sia stato scelto un training file un test set con almeno un dato di verifica
		if ( gym->n_test_samples && !gym->train_set_path.empty() ) {

			// Modifico l'etichetta del pulsante
			widget->label( "Interrompi" );

			// Avvio il thread per l'addestramento della rete
			pthread_create( &gym->thread_id, NULL, Gym::static_start_training, data );
		}

	} else {

		// Modifico l'etichetta del pulsante
		widget->label( "Addestra" );

		// Termino il thread per l'addestramento
		if ( gym->thread_id )
			pthread_cancel( gym->thread_id );

		// Aggiorno il log di lavoro
		gym->log_buffer->append( "\nAddestramento interrotto." );
		gym->log_display->insert_position( gym->log_buffer->length() );
		gym->log_display->show_insert_position();

		// Memorizzo la fine del thread
		gym->thread_id = 0;
	}
}

void
Gym::set_hidden_layers_button_callback( Fl_Widget *widget, void *data ) {

	// Ricavo un puntatore alla finestra principale
	Gym *gym = static_cast< Gym * >( data );

	// Ricavo il numero di strati nascosti impostati
	const size_t hidden_layers = ((Fl_Spinner *) widget)->value();

	// Abilito/disabilito le caselle dei neuroni degli strati nascosti
	if ( hidden_layers >= 3 )

		gym->hidden_neurons_input0->activate();
	else
		gym->hidden_neurons_input0->deactivate();

	if ( hidden_layers >= 4 )

		gym->hidden_neurons_input1->activate();
	else
		gym->hidden_neurons_input1->deactivate();


	if ( hidden_layers >= 5 )

		gym->hidden_neurons_input2->activate();
	else
		gym->hidden_neurons_input2->deactivate();
}

void
Gym::set_training_algorithm_button_callback( Fl_Widget *widget, void *data ) {

	// Ricavo un puntatore alla finestra principale
	Gym *gym = static_cast< Gym * >( data );

	// Ricavo l'algoritmo di addestramento
	const size_t train_algorithm = gym->train_algorithm_input->value();

	// Controllo l'algoritmo di addestramento della rete neurale
	switch ( train_algorithm ) {

		// BATCH
		case TRAIN_BATCH: {

			// Abilito/disabilito i widget con i parametri apprendimento
			gym->learning_rate_input->activate();
			gym->momentum_input->activate();
			gym->increase_factor_input->deactivate();
			gym->decrease_factor_input->deactivate();

			break;
		}

		// RPROP
		case TRAIN_RPROP:
		// RPROP+
		case TRAIN_RPROP_PLUS:
		// RPROP-
		case TRAIN_RPROP_MINUS:
		// IRPROP+
		case TRAIN_IRPROP_PLUS:
		// IRPROP-
		case TRAIN_IRPROP_MINUS: {

			// Abilito/disabilito i widget con i parametri apprendimento
			gym->learning_rate_input->deactivate();
			gym->momentum_input->deactivate();
			gym->increase_factor_input->activate();
			gym->decrease_factor_input->activate();

			break;
		}

		default: break;
	}
}

/** FINE METODI STATICI **/


Gym::Gym( int width, int height ) : Fl_Window( width, height, _GYM_TITLE_ ) {

	// Inizializzo le strutture
	this->neural_network	= NULL;
	this->thread_id			= 0;

	// Imposto l'uscita mostrata nel grafico
	this->graph_output = 0;

	// Inizio la costruzione della finestra
	this->begin();

	// Creo il grafico delle uscite
	this->outputs_plot = new Plot( 15, 15, 300, 200 );
	this->outputs_plot->SetXRulerStep( 0.0 );
	
	// Creo il grafico dell'errore massimo
	this->error_plot = new Plot( 325, 15, 300, 200 );

	// Creo la casella del numero degli strati
	this->n_layers_input = new Fl_Spinner( 30, 240, 120, 25, "Numero strati" );
	this->n_layers_input->align( FL_ALIGN_TOP );
	this->n_layers_input->format( "%.0f" );
	this->n_layers_input->labelcolor( FL_DARK_CYAN );
	this->n_layers_input->range( 2, 5 );
	this->n_layers_input->step( 1.0 );
	this->n_layers_input->value( 3.0 );
	this->n_layers_input->callback( (Fl_Callback *) Gym::set_hidden_layers_button_callback, (void *) this );

	// Creo la casella del numero dei neuroni dello strato nascosto 1
	this->hidden_neurons_input0 = new Fl_Spinner( 180, 240, 120, 25, "Strato nascosto 1" );
	this->hidden_neurons_input0->align( FL_ALIGN_TOP );
	this->hidden_neurons_input0->format( "%.0f" );
	this->hidden_neurons_input0->labelcolor( FL_DARK_CYAN );
	this->hidden_neurons_input0->range( 1, 1000 );
	this->hidden_neurons_input0->step( 1.0 );
	this->hidden_neurons_input0->value( 10.0 );

	// Creo la casella del numero dei neuroni dello strato nascosto 2
	this->hidden_neurons_input1 = new Fl_Spinner( 330, 240, 120, 25, "Strato nascosto 2" );
	this->hidden_neurons_input1->align( FL_ALIGN_TOP );
	this->hidden_neurons_input1->format( "%.0f" );
	this->hidden_neurons_input1->labelcolor( FL_DARK_CYAN );
	this->hidden_neurons_input1->range( 1, 1000 );
	this->hidden_neurons_input1->step( 1.0 );
	this->hidden_neurons_input1->value( 10.0 );
	this->hidden_neurons_input1->deactivate();

	// Creo la casella del numero dei neuroni dello strato nascosto 3
	this->hidden_neurons_input2 = new Fl_Spinner( 480, 240, 120, 25, "Strato nascosto 3" );
	this->hidden_neurons_input2->align( FL_ALIGN_TOP );
	this->hidden_neurons_input2->format( "%.0f" );
	this->hidden_neurons_input2->labelcolor( FL_DARK_CYAN );
	this->hidden_neurons_input2->range( 1, 1000 );
	this->hidden_neurons_input2->step( 1.0 );
	this->hidden_neurons_input2->value( 10.0 );
	this->hidden_neurons_input2->deactivate();

	// Creo la casella delle epoche massime
	this->max_epochs_input = new Fl_Spinner( 180, 300, 120, 25, "Limite epoche" );
	this->max_epochs_input->align( FL_ALIGN_TOP );
	this->max_epochs_input->format( "%.0f" );
	this->max_epochs_input->labelcolor( FL_BLUE );
	this->max_epochs_input->range( 1.0, 5000000.0 );
	this->max_epochs_input->step( 100 );
	this->max_epochs_input->value( 500000 );

	// Creo la casella della frequenza di report
	this->report_frequency_input = new Fl_Spinner( 330, 300, 120, 25, "Frequenza report" );
	this->report_frequency_input->align( FL_ALIGN_TOP );
	this->report_frequency_input->format( "%.0f" );
	this->report_frequency_input->labelcolor( FL_DARK_MAGENTA );
	this->report_frequency_input->range( 1.0, 50000.0 );
	this->report_frequency_input->step( 250 );
	this->report_frequency_input->value( 500 );

	// Casella dell'algoritmo di apprendimento
	this->train_algorithm_input = new Fl_Choice( 30, 300, 120, 25, "Algoritmo" );
	this->train_algorithm_input->align( FL_ALIGN_TOP );
	this->train_algorithm_input->labelcolor( FL_BLACK );
	this->train_algorithm_input->add( "BATCH" );
	this->train_algorithm_input->add( "RPROP" );
	this->train_algorithm_input->add( "RPROP+" );
	this->train_algorithm_input->add( "RPROP-" );
	this->train_algorithm_input->add( "IRPROP+" );
	this->train_algorithm_input->add( "IRPROP-" );
	this->train_algorithm_input->value(1);
	this->train_algorithm_input->callback( (Fl_Callback *) Gym::set_training_algorithm_button_callback, (void *) this );

	// Creo la casella del tasso di apprendimento
	this->learning_rate_input = new Fl_Spinner( 30, 360, 120, 25, "Delta" );
	this->learning_rate_input->align( FL_ALIGN_TOP );
	this->learning_rate_input->format( "%.2f" );
	this->learning_rate_input->labelcolor( FL_DARK_RED );
	this->learning_rate_input->range( 0.1, 0.9 );
	this->learning_rate_input->step( 0.01 );
	this->learning_rate_input->value( 0.5 );
	this->learning_rate_input->deactivate();

	// Creo la casella del momentum
	this->momentum_input = new Fl_Spinner( 180, 360, 120, 25, "Momentum" );
	this->momentum_input->align( FL_ALIGN_TOP );
	this->momentum_input->format( "%.2f" );
	this->momentum_input->labelcolor( FL_DARK_RED );
	this->momentum_input->range( 0.0, 0.99 );
	this->momentum_input->step( 0.01 );
	this->momentum_input->value( 0.0 );
	this->momentum_input->deactivate();

	// Creo la casella del fattore di incremento
	this->increase_factor_input = new Fl_Spinner( 330, 360, 120, 25, "Incremento" );
	this->increase_factor_input->align( FL_ALIGN_TOP );
	this->increase_factor_input->format( "%.2f" );
	this->increase_factor_input->labelcolor( FL_DARK_GREEN );
	this->increase_factor_input->range( 1.01, 5.0 );
	this->increase_factor_input->step( 0.01 );
	this->increase_factor_input->value( 1.2 );

	// Creo la casella del fattore di decremento
	this->decrease_factor_input = new Fl_Spinner( 480, 360, 120, 25, "Decremento" );
	this->decrease_factor_input->align( FL_ALIGN_TOP );
	this->decrease_factor_input->format( "%.2f" );
	this->decrease_factor_input->labelcolor( FL_DARK_GREEN );
	this->decrease_factor_input->range( 0.01, 0.99 );
	this->decrease_factor_input->step( 0.01 );
	this->decrease_factor_input->value( 0.5 );

	// Creo la casella dell'errore desiderato
	this->desired_error_input = new Fl_Spinner( 480, 300, 120, 25, "Errore desiderato" );
	this->desired_error_input->align( FL_ALIGN_TOP );
	this->desired_error_input->format( "%.5f" );
	this->desired_error_input->labelcolor( FL_RED );
	this->desired_error_input->range( 0.0, 1.0 );
	this->desired_error_input->step( 0.00001 );
	this->desired_error_input->value( 0.000001 );
	this->desired_error_input->format( "%.8F" );

	// Cornice delle informazioni
	Fl_Box *info_frame = new Fl_Box( 325, 400, 300, 70 );
	info_frame->box( FL_FRAME_BOX );

	// Formato dell'errore e del numero delle epoche
	char error_format[]		= "%.10F";
	char epochs_format[]	= "%.0F";

	// Creo il riquadro dell'errore massimo
	this->error_box = new Fl_Value_Output( 460, 410, 140, 25, "Errore massimo" );
	this->error_box->labelsize( 12 );
	this->error_box->format( error_format );

	// Riquadro del numero delle epoche
	this->epochs_box = new Fl_Value_Output( 460, 435, 140, 25, "Epoche" );
	this->epochs_box->labelsize( 12 );
	this->epochs_box->format( epochs_format );

	// Creo il pulsante per il caricamento del training set
	this->load_training_set_button = new Fl_Button( 15, 410, 170, 25, "Carica training set..." );
	this->load_training_set_button->callback( (Fl_Callback *) Gym::load_training_set_button_callback, (void *) this );

	// Creo il pulsante per il caricamento del test set
	this->load_test_set_button = new Fl_Button( 15, 440, 170, 25, "Carica test set..." );
	this->load_test_set_button->callback( (Fl_Callback *) Gym::load_test_set_button_callback, (void *) this );

	// Creo il pulsante per l'avvio dell'addestramento
	this->training_button = new Fl_Button( 190, 410, 120, 25, "Addestra" );
	this->training_button->callback( (Fl_Callback *) Gym::training_button_callback, (void *) this );

	// Creo il pulsante per salvare la rete neurale
	this->save_button = new Fl_Button( 190, 440, 120, 25, "Salva la rete..." );
	this->save_button->callback( (Fl_Callback *) Gym::save_button_callback, (void *) this );

	// Creo il buffer del log di lavoro
	this->log_buffer = new Fl_Text_Buffer();

	// Creo la finestra del log di lavoro
	this->log_display = new Fl_Text_Display( 15, 500, 610, 125, "Log di lavoro" );
	this->log_display->buffer( this->log_buffer );
	this->log_display->scrollbar_align( FL_ALIGN_RIGHT );

	// Termino la costruzione della finestra
	this->end();

	// Avvio la funzione per l'aggiornamento asincrono della finestra
	Fl::add_timeout( 1.0 / _GYM_FPS_, Gym::asynchronous_redraw, (void *) this );
}

Gym::~Gym() {

	// Termino il thread per l'addestramento
	if ( this->thread_id )
		pthread_cancel( this->thread_id );

	// Distruggo la rete neurale
	if ( this->neural_network != NULL )
		delete this->neural_network;
}

int
Gym::handle( int event ) {

	// Controllo gli eventi della finestra
	switch( event ) {

		// Pressione di un tasto
		case FL_KEYDOWN: {

			// Controllo il tasto premuto
			switch ( Fl::event_key() ) { 

				// Funzione di uscita
				case FL_Escape: {

					exit(0);
				}

				// Mostra l'uscita #1 nel grafico
				case '1': {

					// Imposto l'uscita mostrata nel grafico
					if ( this->output_size >= 1 ) this->graph_output = 0;
					break;
				}

				// Mostra l'uscita #2 nel grafico
				case '2': {

					// Imposto l'uscita mostrata nel grafico
					if ( this->output_size >= 2 ) this->graph_output = 1;
					break;
				}

				// Mostra l'uscita #3 nel grafico
				case '3': {

					// Imposto l'uscita mostrata nel grafico
					if ( this->output_size >= 3 ) this->graph_output = 2;
					break;
				}

				// Mostra l'uscita #4 nel grafico
				case '4': {

					// Imposto l'uscita mostrata nel grafico
					if ( this->output_size >= 4 ) this->graph_output = 3;
					break;
				}

				// Mostra l'uscita #5 nel grafico
				case '5': {

					// Imposto l'uscita mostrata nel grafico
					if ( this->output_size >= 5 ) this->graph_output = 4;
					break;
				}

				// Mostra l'uscita #6 nel grafico
				case '6': {

					// Imposto l'uscita mostrata nel grafico
					if ( this->output_size >= 6 ) this->graph_output = 5;
					break;
				}

				// Mostra l'uscita #7 nel grafico
				case '7': {

					// Imposto l'uscita mostrata nel grafico
					if ( this->output_size >= 7 ) this->graph_output = 6;
					break;
				}

				// Mostra l'uscita #8 nel grafico
				case '8': {

					// Imposto l'uscita mostrata nel grafico
					if ( this->output_size >= 8 ) this->graph_output = 7;
					break;
				}

				// Mostra l'uscita #9 nel grafico
				case '9': {

					// Imposto l'uscita mostrata nel grafico
					if ( this->output_size >= 9 ) this->graph_output = 8;
					break;
				}

				// Mostra l'uscita successiva nel grafico
				case FL_Page_Up:
				case FL_Up:
				case '+': {

					// Imposto l'uscita mostrata nel grafico
					if ( this->graph_output + 1 < this->output_size ) this->graph_output++;
					break;
				}

				// Mostra l'uscita precedente nel grafico
				case FL_Page_Down:
				case FL_Down:
				case '-': {

					// Imposto l'uscita mostrata nel grafico
					if ( (float) this->graph_output - 1 > 0 ) this->graph_output--;
					break;
				}

				default: break;
			}

			return 1;
		}

		// Richiamo la funzione superiore
		default: return Fl_Window::handle( event );
	}
}

void
Gym::LoadTrainingSet() {

	// Termino il thread per l'addestramento
	if ( this->thread_id )
		pthread_cancel( this->thread_id );

	// Memorizzo la fine del thread
	this->thread_id = 0;

	// Percorso al file del training set
	const char *path;

	// Chiedo all'utente di scegliere il file da cui caricare il training set
	if ( ( path = fl_file_chooser( "Carica un training set...", "Training Set (*.train)\tTest Set (*.test)", "" ) ) != NULL ) {

		// Salvo il percorso al file del training set
		this->train_set_path = path;

		// Apre uno stream al file dell'addestramento
		std::ifstream file( path );

		// Controllo se il file è stato aperto correttamente
		if ( !file.is_open() ) {

			// Communico l'errore all'utente
			fprintf( stderr, "(W) Couldn't open file '%s'!\n", path );

			// Termino l'esecuzione del programma
			exit(1);
		}

		// Azzero il numero dei campioni del test set
		this->n_test_samples = 0;

		// Rimuovo le vecchie informazioni
		this->inputs_data.clear();
		this->outputs_data.clear();

		// Numero della riga letta
		size_t n_line = 0;

		// Flag di uscita
		bool end = false;

		// Buffer contenente la riga letta
		std::string line;

		// Leggo le informazioni sul tipo di addestramento
		while ( !end && file.good() ) {

			// Leggo la nuova riga
			std::getline( file, line );

			// Controllo che la linea non sia vuota o un commento
			if ( !line.empty() && line.at(0) != _COMMENT_ ) {

				// Valori estratti
				std::vector< Precision > value;

				// Estraggo i valori di ingresso
				if ( values_on_string( line, value ) != 2 ) {

					// E se trovo un errore lo communico all'utente
					fprintf( stderr, " (W) Syntax error on training file '%s', at line %zu!\n", path, n_line );
				}

				// Memorizzo le dimensioni della rete
				this->input_size	= (size_t) value[0];
				this->output_size	= (size_t) value[1];

				// Imposto il flag di uscita
				end = true;
			}

			// Incremento il contatore della linea
			n_line++;
		}

		// Aggiorno il log di lavoro
		this->log_buffer->append( "\nTraining set caricato con successo." );
		this->log_display->insert_position( this->log_buffer->length() );
		this->log_display->show_insert_position();

		// Chiudo lo stream al file
		file.close();
	}
}

void
Gym::LoadTestSet() {

	// Termino il thread per l'addestramento
	if ( this->thread_id )
		pthread_cancel( this->thread_id );

	// Memorizzo la fine del thread
	this->thread_id = 0;

	// Percorso al file del training set
	const char *path;

	// Chiedo all'utente di scegliere il file da cui caricare il training set
	if ( ( path = fl_file_chooser( "Carica un test set...", "Test Set (*.test)\tTraining Set (*.train)", "" ) ) != NULL ) {

		// Apre uno stream al file dell'addestramento
		std::ifstream file( path );

		// Controllo se il file è stato aperto correttamente
		if ( !file.is_open() ) {

			// Communico l'errore all'utente
			fprintf( stderr, "(W) Couldn't open file '%s'!\n", path );

			// Termino l'esecuzione del programma
			exit(1);
		}

		// Imposto l'uscita mostrata nel grafico
		this->graph_output = 0;

		// Azzero il numero dei campioni del test set
		this->n_test_samples = 0;

		// Rimuovo le vecchie informazioni
		this->inputs_data.clear();
		this->outputs_data.clear();

		// Numero della riga letta
		size_t n_line = 0;

		// Flag di uscita
		bool end = false;

		// Buffer contenente la riga letta
		std::string line;

		// Leggo le informazioni sul tipo di test set
		while ( !end && file.good() ) {

			// Leggo la nuova riga
			std::getline( file, line );

			// Controllo che la linea non sia vuota o un commento
			if ( !line.empty() && line.at(0) != _COMMENT_ ) {

				// Valori estratti
				std::vector< Precision > value;

				// Estraggo i valori di ingresso
				if ( values_on_string( line, value ) != 2 ) {

					// E se trovo un errore lo communico all'utente
					fprintf( stderr, " (W) Syntax error on test file '%s', at line %zu!\n", path, n_line );
				}

				// Controllo che il test set sia compatibile alla rete corrente
				if ( this->input_size != (size_t) value[0] || this->output_size != (size_t) value[1] ) {

					// Aggiorno il log di lavoro
					this->log_buffer->append( "\nImpossibile caricare il test set: incompatibile con il training set corrente." );
					this->log_display->insert_position( this->log_buffer->length() );
					this->log_display->show_insert_position();

					return;
				}

				// Imposto il flag di uscita
				end = true;
			}

			// Incremento il contatore della linea
			n_line++;
		}

		// Leggo i dati per l'addestramento una riga per volta
		while ( file.good() ) {

			// Leggo la nuova riga
			std::getline( file, line );

			// Controllo che la linea non sia vuota o un commento
			if ( !line.empty() && line.at(0) != _COMMENT_ ) {

				// Cerco il separatore nella riga
				int found = line.find_first_of( _SEPARATOR_ );

				// Estraggo i valori di ingresso
				if ( values_on_string( line.substr( 0, found ), this->inputs_data ) != this->input_size ) {

					// E se trovo un errore lo communico all'utente
					fprintf( stderr, " (W) Syntax error on test file '%s', at line %zu!\n", path, n_line );
				}

				// Estraggo i valori di uscita
				if ( values_on_string( line.substr( found + 1 ), this->outputs_data ) != this->output_size ) {

					// E se trovo un errore lo communico all'utente
					fprintf( stderr, " (W) Syntax error on test file '%s', at line %zu!\n", path, n_line );
				}

				// Incremento il numero dei campioni del test set
				this->n_test_samples++;

				// Incremento il contatore della linea
				n_line++;
			}
		}

		// Chiudo lo stream al file
		file.close();

		// Aggiorno il log di lavoro
		this->log_buffer->append( "\nTest set caricato con successo." );
		this->log_display->insert_position( this->log_buffer->length() );
		this->log_display->show_insert_position();
	}
}

void
Gym::StartTraining() {

	// Imposto la cancellazione asincrona del thread
	pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );

	// Prendo il controllo sulle FLTK
	Fl::lock();

	// Aggiorno il log di lavoro
	this->log_buffer->append( "\n\nInizio l'addestramento." );
	this->log_display->insert_position( this->log_buffer->length() );
	this->log_display->show_insert_position();

	// Rimuovo le vecchie informazioni
	this->error_data.clear();

	// Modifico il colore della casella dell'errore massimo
	this->error_box->color( FL_RED );
	this->error_box->redraw();

	// Rilascio il controllo sulle FLTK
	Fl::unlock();

	// Cancello la vecchia rete neurale
	if ( this->neural_network != NULL )
		delete this->neural_network;

	// Ricavo il numero degli strati
	const size_t n_layers = (size_t) this->n_layers_input->value();

	// Ricavo il numero dei neuroni degli strati nascosti
	const size_t hidden_neurons_0 = (size_t) this->hidden_neurons_input0->value();
	const size_t hidden_neurons_1 = (size_t) this->hidden_neurons_input1->value();
	const size_t hidden_neurons_2 = (size_t) this->hidden_neurons_input2->value();

	// Creo la nuova rete neurale
	if ( n_layers == 2 )
		this->neural_network = new Network( 2, this->input_size, this->output_size );

	else if ( n_layers == 3 )
		this->neural_network = new Network( 3, this->input_size, hidden_neurons_0, this->output_size );

	else if ( n_layers == 4 )
		this->neural_network = new Network( 4, this->input_size, hidden_neurons_0, hidden_neurons_1, this->output_size );

	else if ( n_layers == 5 )
		this->neural_network = new Network( 5, this->input_size, hidden_neurons_0, hidden_neurons_1, hidden_neurons_2, this->output_size );


	// Ricavo l'algoritmo di addestramento
	const size_t train_algorithm = this->train_algorithm_input->value();

	// Imposto l'algoritmo di addestramento della rete neurale
	this->neural_network->SetTrainingAlgorithm( (TrainAlgorithm) train_algorithm );

	// Imposto la funzione di report dell'addestramento
	this->neural_network->SetReportSignal( &Gym::static_update_plot, (void *) this );

	// Controllo l'algoritmo di addestramento della rete neurale
	switch ( train_algorithm ) {

		// BATCH
		case TRAIN_BATCH: {

			// Ricavo il tasso di apprendimento
			const Precision eps = (Precision) this->learning_rate_input->value();

			// Ricavo il momentum
			const Precision momentum = (Precision) this->momentum_input->value();

			// Imposto il tasso di apprendimento e il momentum
			this->neural_network->SetTrainingParameters( eps, momentum );

			break;
		}

		// RPROP
		case TRAIN_RPROP: {

			// Ricavo il fatto di incremento
			const Precision increase_factor = (Precision) this->increase_factor_input->value();

			// Ricavo il fatto di decremento
			const Precision decrease_factor = (Precision) this->decrease_factor_input->value();

			// Imposto i fattori di incremento e decremento del RPROP+
			this->neural_network->SetRpropFactor( increase_factor, decrease_factor );

			break;
		}

		// RPROP+
		case TRAIN_RPROP_PLUS: {

			// Ricavo il fatto di incremento
			const Precision increase_factor = (Precision) this->increase_factor_input->value();

			// Ricavo il fatto di decremento
			const Precision decrease_factor = (Precision) this->decrease_factor_input->value();

			// Imposto i fattori di incremento e decremento del RPROP+
			this->neural_network->SetRpropFactor( increase_factor, decrease_factor );

			break;
		}

		// RPROP-
		case TRAIN_RPROP_MINUS: {

			// Ricavo il fatto di incremento
			const Precision increase_factor = (Precision) this->increase_factor_input->value();

			// Ricavo il fatto di decremento
			const Precision decrease_factor = (Precision) this->decrease_factor_input->value();

			// Imposto i fattori di incremento e decremento del RPROP-
			this->neural_network->SetRpropFactor( increase_factor, decrease_factor );

			break;
		}

		// IRPROP+
		case TRAIN_IRPROP_PLUS: {

			// Ricavo il fatto di incremento
			const Precision increase_factor = (Precision) this->increase_factor_input->value();

			// Ricavo il fatto di decremento
			const Precision decrease_factor = (Precision) this->decrease_factor_input->value();

			// Imposto i fattori di incremento e decremento del RPROP+
			this->neural_network->SetRpropFactor( increase_factor, decrease_factor );

			break;
		}

		// IRPROP-
		case TRAIN_IRPROP_MINUS: {

			// Ricavo il fatto di incremento
			const Precision increase_factor = (Precision) this->increase_factor_input->value();

			// Ricavo il fatto di decremento
			const Precision decrease_factor = (Precision) this->decrease_factor_input->value();

			// Imposto i fattori di incremento e decremento del RPROP-
			this->neural_network->SetRpropFactor( increase_factor, decrease_factor );

			break;
		}

		default: break;
	}

	// Ricavo l'errore desiderato
	const Precision desired_error = (Precision) this->desired_error_input->value();

	// Ricavo le epoche massime
	const size_t max_epochs = (size_t) this->max_epochs_input->value();

	// Ricavo la frequenza di report
	const size_t report_frequency = (size_t) this->report_frequency_input->value();

	// Imposto il passo dell'asse delle ascisse del grafico dell'errore
	this->error_plot->SetXRulerStep( (float) report_frequency );

	// Addestro la rete neurale
	this->neural_network->TrainOnFile( this->train_set_path.c_str(), desired_error, max_epochs, report_frequency );

	// Prendo il controllo sulle FLTK
	Fl::lock();

	// Modifico il colore della casella dell'errore massimo
	this->error_box->color( FL_GREEN );
	this->error_box->redraw();

	// Rilascio il controllo sulle FLTK
	Fl::unlock();

	// Prendo il controllo sulle FLTK
	Fl::lock();

	// Modifico l'etichetta del pulsante
	this->training_button->label( "Addestra" );

	// Memorizzo la fine del thread
	this->thread_id = 0;

	// Aggiorno il log di lavoro
	this->log_buffer->append( "\nAddestramento terminato con successo." );
	this->log_display->insert_position( this->log_buffer->length() );
	this->log_display->show_insert_position();

	// Rilascio il controllo sulle FLTK
	Fl::unlock();
}

int
Gym::UpdatePlot(	Network *network, size_t epochs, Precision max_error,
					const Precision *outputs, size_t outputs_size ) {

	// Prendo il controllo sulle FLTK
	Fl::lock();

	// Pulisco i grafici
	this->outputs_plot->ClearData();
	this->error_plot->ClearData();

	// Rilascio il controllo sulle FLTK
	Fl::unlock();

	// Uscite della rete
	std::vector< float > net_data;

	// Iteratore
	size_t j, i = 0;

	// Calcolo i valori di uscita della rete
	for ( ; i < this->n_test_samples; i++ ) {

		// Ingressi del campione
		std::vector< Precision > inputs_sample;

		// Preparo il vettore con gli ingressi del campione
		for ( j = 0; j < this->input_size; j++ ) {

			inputs_sample.push_back( (Precision) this->inputs_data[ i * this->input_size + j ] );
		}

		// Eseguo la rete neurale con gli ingressi dell'esempio
		const Precision *out = this->neural_network->Run( &inputs_sample[0] );

		// Memorizzo le uscite della rete
		for ( j = 0; j < this->output_size; j++ ) {

			net_data.push_back( (float) out[j] );
		}
	}

	// Aggiungo l'errore massimo ai dati
	this->error_data.push_back( (float) max_error );

	// Prendo il controllo sulle FLTK
	Fl::lock();

	// Uscite desiderate e della rete del campione
	std::vector< Precision > desired, net;

	// Preparo i vettori con le uscite del campione
	for ( j = 0; j < this->n_test_samples; j++ ) {

		desired.push_back( (Precision) this->outputs_data[ this->graph_output + j * this->output_size ] );
		net.push_back( (Precision) net_data[ this->graph_output + j * this->output_size ] );
	}

	// Aggiungo i dati al grafico delle uscite
	this->outputs_plot->PushData( desired, ColorRGBub( 0, (uint8_t) ( this->graph_output * 50.0 ), 255 ), true );
	this->outputs_plot->PushData( net, ColorRGBub( (uint8_t) ( this->graph_output * 50.0 ), 255, 0 ), true );

	// Imposto i valori minimo e massimo del grafico delle uscite
	this->outputs_plot->SetMinValue( 0.0 );
	this->outputs_plot->SetMaxValue( 1.0 );

	// Aggiungo i dati al grafico dell'errore massimo
	this->error_plot->PushData( this->error_data, ColorRGBub( 255, 0, 0 ), false );

	// Imposto il valore minimo del grafico dell'errore
	this->error_plot->SetMinValue( 0.0 );
	this->error_plot->SetMaxValue( 1.0 );

	// Aggiorno il numero delle epoche
	this->epochs_box->value( (double) epochs );

	// Aggiorno il riquadro dell'errore massimo
	this->error_box->value( (double) max_error );

	// Costruisco il testo del nuovo log di lavoro
	char str_buffer[200];
	snprintf( str_buffer, 200, "\n  Epochs #%zu, current error: %.10F", epochs, (double) max_error );

	// Aggiorno il log di lavoro
	this->log_buffer->append( str_buffer );
	this->log_display->insert_position( this->log_buffer->length() );
	this->log_display->show_insert_position();

	// Rilascio il controllo sulle FLTK
	Fl::unlock();

	return 0;
}

}; // Chiudo il namespace di Serotonina


/* INIZIO FUNZIONE PRINCIPALE */

int
main( void ) {

	// Creo la finestra principale
	Serotonina::Gym gym_window( 640, 640 );

	gym_window.resizable( gym_window );
	gym_window.show();

	// Abilito il multithreading con le FLTK
	Fl::lock();

	// Richiamo il loop principale
	return Fl::run();
}

/* FINE FUNZIONE PRINCIPALE */

#endif