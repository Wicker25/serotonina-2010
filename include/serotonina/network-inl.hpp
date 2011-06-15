/* 
    Title --- network-inl.hpp

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

#ifndef _SEROTONINA_NETWORK_INL_HPP_
#define _SEROTONINA_NETWORK_INL_HPP_

#include <serotonina/network.hpp>

namespace Serotonina { // Namespace di Serotonina

inline std::vector< Layer * > &
Network::GetData() {

	// Ritorno la struttura della rete
	return this->layers;
}

} // Chiudo il namespace di Serotonina

#endif
