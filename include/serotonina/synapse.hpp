/* 
    Title --- synapse.hpp

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

#ifndef _SEROTONINA_SYNAPSE_HPP_
#define _SEROTONINA_SYNAPSE_HPP_

namespace Serotonina { // Namespace di Serotonina

// Prototipi
struct TrainData;

// Struttura di una connessione tra due neuroni (sinapsi)
struct Synapse {

	T_Precision weight;		// Peso della connessione
	TrainData *train;		// Dati per l'addestramento
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
		delta_weight = dEdw = prev_dEdw = 0.0;
		learning_rate	= 0.1;
	}
};

} // Chiudo il namespace di Serotonina

#endif
