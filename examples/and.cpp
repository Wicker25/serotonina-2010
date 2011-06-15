/* 
	Title --- and.cpp

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
	Network network( 2, 2, 1 );

	// Creo l'addestratore della rete neurale
	Trainer trainer( &network );

	// Addestro la rete neurale con il metodo Batch
	trainer.SetParameters( 0.5, 0.8 );
	trainer.TrainOnFile< Algorithms::Batch >( "train/and.train", 0.000001, 100000, 5000 );

	// Ingressi ed uscite della rete neurale
	Serotonina::T_Precision in[2];
	const Serotonina::T_Precision *out;

	// Iteratori
	size_t i, j = 0;

	// Eseguo alcuni test con la rete neurale
	for ( ; j <= 1; j++ ) {

		for ( i = 0; i <= 1; i++ ) {

			// Imposto gli ingressi
			in[0] = (T_Precision) j;
			in[1] = (T_Precision) i;

			// Calcolo l'uscita della rete neurale
			out = network.Run( in );

			// Stampo i risultati
			std::cout.setf( std::ios::fixed, std::ios::floatfield );
			std::cout << "Run ( " << in[0] << " AND " << in[1] << " ) = " << out[0] << std::endl;
		}
	}

	return 0;
}

