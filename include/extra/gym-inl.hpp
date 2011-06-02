/* 
    Title --- gym-inl.hpp

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

#ifndef _SEROTONINA_GYM_INL_HPP_
#define _SEROTONINA_GYM_INL_HPP_

#include "extra/gym.hpp"

namespace Serotonina { // Namespace di Serotonina

/** INIZIO METODI STATICI **/

inline void
Gym::load_training_set_button_callback( Fl_Widget *widget, void *data ) {

	// Ricavo un puntatore alla finestra principale
	Gym *gym = static_cast< Gym * >( data );

	// Chiama il metodo corrispondente
	gym->LoadTrainingSet();
}

inline void
Gym::load_test_set_button_callback( Fl_Widget *widget, void *data ) {

	// Ricavo un puntatore alla finestra principale
	Gym *gym = static_cast< Gym * >( data );

	// Chiama il metodo corrispondente
	gym->LoadTestSet();
}

inline void
Gym::save_button_callback( Fl_Widget *widget, void *data ) {

	// Ricavo un puntatore alla finestra principale
	Gym *gym = static_cast< Gym * >( data );

	// Chiama il metodo corrispondente
	gym->SaveNeuralNetwork();
}

inline void *
Gym::static_start_training( void *data ) {

	// Ricavo un puntatore alla finestra principale
	Gym *gym = static_cast< Gym * >( data );

	// Chiama il metodo corrispondente
	gym->StartTraining();

	return NULL;
}

inline int
Gym::static_update_plot(	Network *network, size_t epochs, T_Precision max_error,
							const T_Precision *outputs, size_t outputs_size, void *data ) {

	// Ricavo un puntatore alla finestra principale
	Gym *gym = static_cast< Gym * >( data );

	// Avvio la funzione di aggiornamento del grafico
	return gym->UpdatePlot( network, epochs, max_error, outputs, outputs_size );
}

/** FINE METODI STATICI **/

inline void
Gym::SaveNeuralNetwork() {

	// Controllo se la rete neurale è stata creata
	if ( this->neural_network != NULL ) {

		// Percorso al file del training set
		const char *path;

		// Chiedo all'utente di scegliere il file di destinazione
		if ( ( path = fl_file_chooser( "Salva in un file...", "Neural Network (*.net)", "" ) ) != NULL ) {

			// Salvo la rete neurale in un file
			this->neural_network->Save( path );
		}
	}
}

} // Chiudo il namespace di Serotonina

#endif
