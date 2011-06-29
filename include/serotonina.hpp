/* 
    Title --- serotonina.hpp

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

#ifndef _SEROTONINA_HPP_
#define _SEROTONINA_HPP_

/*!
 * @namespace Serotonina
 * \if english
 *   @brief Main namespace of the library.
 * \endif
 * \if italian
 *   @brief Namespace principale della libreria
 * \endif
 */

// Includo tutti gli headers della libreria

#include <serotonina/version.hpp>
#include <serotonina/types.hpp>
#include <serotonina/macros.hpp>
#include <serotonina/string.hpp>

#include <serotonina/neuron.hpp>
#include <serotonina/synapse.hpp>
#include <serotonina/layer.hpp>

#include <serotonina/network.hpp>
#include <serotonina/network-inl.hpp>

#include <serotonina/trainer.hpp>
#include <serotonina/trainer-inl.hpp>

#include <serotonina/train-algorithm.hpp>
#include <serotonina/train-algorithm.hpp>

// Includo i moduli per l'addestramento

#include <serotonina/mod/batch.hpp>
#include <serotonina/mod/rprop.hpp>
#include <serotonina/mod/rpropp.hpp>
#include <serotonina/mod/rpropm.hpp>
#include <serotonina/mod/irpropp.hpp>
#include <serotonina/mod/irpropm.hpp>

#endif
