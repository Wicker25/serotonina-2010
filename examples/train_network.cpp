/* 
	Title --- train_network.cpp

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

    To compile: g++ `pkg-config --libs --cflags serotonina` train_network.cpp -o train_network
*/

#include <iostream>
#include <vector>

// Includo l'intestazione di Serotonina
#include <serotonina.hpp>

// Uso il namespace di Serotonina
using namespace serotonina;

int main( void ) {

	// Creo la rete neurale
	Network network( 3, 2, 5, 1 );


	// Creo l'addestratore della rete neurale
	Trainer trainer( network );

	// Addestro la rete neurale con il metodo Batch
	trainer.setParams( 0.5, 0.8 );

	// Esempi di ingresso
	const T_Precision inputs0[] =	{
										0, 0,
										0, 1,
										1, 0,
										1, 1
									};

	// Esempi di uscita
	const T_Precision outputs0[] =	{
										0,
										1,
										1,
										0
									};

	// Log di lavoro
	std::cout << "Training with array :" << std::endl;

	// Addestro la rete neurale usando gli esempi
	trainer.train< algorithms::Batch >( inputs0, outputs0, 4, 0.000001, 100000, 5000 );

	// Esempi di ingresso
	std::vector< T_Precision> inputs1;

	inputs1.push_back(0);
	inputs1.push_back(0);

	inputs1.push_back(0);
	inputs1.push_back(1);

	inputs1.push_back(1);
	inputs1.push_back(0);

	inputs1.push_back(1);
	inputs1.push_back(1);

	// Esempi di uscita
	std::vector< T_Precision> outputs1;

	outputs1.push_back(0);
	outputs1.push_back(1);
	outputs1.push_back(1);
	outputs1.push_back(0);

	// Log di lavoro
	std::cout << "\nTraining with std::vector :" << std::endl;

	// Addestro la rete neurale usando gli esempi
	trainer.train< algorithms::Batch >( inputs1, outputs1, 0.000001, 100000, 5000 );


	// Log di lavoro
	std::cout << "\nTraining with file.train :" << std::endl;

	// Addestro la rete neurale leggendo l'insieme di addestramento da un file
	trainer.trainOnFile< algorithms::Batch >( "train/xor.train", 0.000001, 100000, 5000 );


	// Log di lavoro
	std::cout << "\nRun network :" << std::endl;

	// Ingressi della rete neurale
	T_Precision in[2];

	// Iteratori
	size_t i, j = 0;

	// Eseguo alcuni test con la rete neurale
	for ( ; j <= 1; j++ ) {

		for ( i = 0; i <= 1; i++ ) {

			// Imposto gli ingressi
			in[0] = (T_Precision) j;
			in[1] = (T_Precision) i;

			// Calcolo l'uscita della rete neurale
			const std::vector< T_Precision > &out = network.run( in );

			// Stampo i risultati
			std::cout.setf( std::ios::fixed, std::ios::floatfield );
			std::cout << "Run ( " << in[0] << " AND " << in[1] << " ) = " << out[0] << std::endl;
		}
	}

	return 0;
}

