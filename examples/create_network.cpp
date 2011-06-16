/* 
	Title --- create_network.cpp

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

    To compile: g++ `pkg-config --libs --cflags serotonina` create_network.cpp -o create_network
*/

#include <iostream>
#include <vector>

// Includo l'intestazione di Serotonina
#include <serotonina.hpp>

// Uso il namespace di Serotonina
using namespace Serotonina;

int main( void ) {

	// Creo una rete neurale 2x1
	Network network0( 2, 2, 1 );

	/* ------------------------ */

	// Creo l'array che descrive la struttura della rete
	size_t layers_struct1[3] = { 5, 20, 5 };

	// Creo una rete neurale 5x20x5
	Network network1( 3, layers_struct1 );

	/* ------------------------ */

	// Creo il vettore che descrive la struttura della rete
	std::vector< size_t > layers_struct2;

	layers_struct2.push_back(20);
	layers_struct2.push_back(10);
	layers_struct2.push_back(10);
	layers_struct2.push_back(1);

	// Creo una rete neurale 20x10x10x1
	Network network2( layers_struct2 );
}

