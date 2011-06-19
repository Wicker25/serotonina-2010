/* 
    Title --- rpropp.hpp

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

#ifndef _SEROTONINA_RPROPP_HPP_
#define _SEROTONINA_RPROPP_HPP_

#include <iostream>

#include <serotonina/train-algorithm.hpp>
#include <serotonina/train-algorithm-inl.hpp>

#include <serotonina/mod/rprop.hpp>

namespace Serotonina { // Namespace di Serotonina

namespace Algorithms { // Namespace degli algoritmi

class RpropPlus : public Rprop {

	/* ALGORITMO RPROP+ */

public:

	// Descrizione dell'algoritmo
	static const char *description;

	// Esegue la correzione dei pesi
	static void UpdateWeights(	Network &network, std::vector< T_Precision > &train_params,
								T_Precision net_error, T_Precision prev_net_error );
};

} // Chiudo il namespace degli algoritmi

} // Chiudo il namespace di Serotonina

#endif
