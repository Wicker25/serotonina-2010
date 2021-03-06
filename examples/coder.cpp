/* 
	Title --- coder.cpp

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
    along with Serotonina. If not, see <http://www.gnu.org/licenses/>.

    To compile: g++ `pkg-config --libs --cflags serotonina` coder.cpp -o coder
*/

#include <iostream>

// Includo l'intestazione di Serotonina
#include <serotonina.hpp>

// Uso il namespace di Serotonina
using namespace serotonina;

int main( void ) {

	// Creo la rete neurale
	Network network( 3, 3, 10, 6 );

	// Creo l'addestratore della rete neurale
	Trainer trainer( network );

	// Addestro la rete neurale con il metodo Batch
	trainer.setParams( 0.5, 0.8 );
	trainer.trainOnFile< algorithms::Batch >( "train/coder.train", 0.000001, 100000, 5000 );

	// Eseguo la rete neurale
	T_Precision in[3] = { 0, 1, 1 };
	const std::vector< T_Precision > &out = network.run( in );

	// Iteratore
	size_t i;

	// Stampo le uscite della rete neurale
	std::cout.setf( std::ios::fixed, std::ios::floatfield );
	std::cout << "Outputs: ";

	for ( i = 0; i < 6; i++ ) {

		std::cout << out[i] << " ";
	}

	std::cout << std::endl;

	return 0;
}

