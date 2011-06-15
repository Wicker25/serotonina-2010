/* 
	Title --- coder.cpp

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

#include <iostream>

// Includo l'intestazione di Serotonina
#include <serotonina.hpp>

// Uso il namespace di Serotonina
using namespace Serotonina;

int main( void ) {

	// Creo la rete neurale
	Network network( 3, 3, 10, 6 );

	// Creo l'addestratore della rete neurale
	Trainer trainer( &network );

	// Addestro la rete neurale con il metodo Batch
	trainer.SetParameters( 0.5, 0.8 );
	trainer.TrainOnFile< Algorithms::Batch >( "train/coder.train", 0.000001, 100000, 5000 );

	// Eseguo la rete neurale
	Serotonina::T_Precision in[3] = { 0, 1, 1 };
	const Serotonina::T_Precision *out = network.Run( in );

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

