/* 
    Title --- batch.hpp

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

#ifndef _SEROTONINA_BATCH_HPP_
#define _SEROTONINA_BATCH_HPP_

#include <iostream>

#include <serotonina/trainer.hpp>
#include <serotonina/trainer-inl.hpp>

namespace Serotonina { // Namespace di Serotonina

namespace Algorithms { // Namespace degli algoritmi

void Batch(	Network *network, const std::vector< T_Precision > &train_params,
			T_Precision net_error, T_Precision prev_net_error );

} // Chiudo il namespace degli algoritmi

} // Chiudo il namespace di Serotonina

#endif
