/* 
    Title --- gym-inl.hpp

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

#ifndef _SEROTONINA_GYM_INL_HPP_
#define _SEROTONINA_GYM_INL_HPP_

#include "gym.hpp"

/** INIZIO METODI STATICI **/

inline void *
Gym::static_start_training( void *data ) {

	// Ricavo un puntatore alla finestra principale
	Gym *gym = static_cast< Gym * >( data );

	// Chiama il metodo corrispondente
	gym->StartTraining();

	return NULL;
}

inline int
Gym::static_update_plot(	Network &network, size_t epochs, time_t elapsed_time, T_Precision max_error,
							const std::vector< T_Precision > &outputs, size_t outputs_size, void *data ) {

	// Ricavo un puntatore alla finestra principale
	Gym *gym = static_cast< Gym * >( data );

	// Avvio la funzione di aggiornamento del grafico
	return gym->UpdatePlot( &network, epochs, elapsed_time, max_error, &outputs[0], outputs_size );
}

/** FINE METODI STATICI **/

#endif
