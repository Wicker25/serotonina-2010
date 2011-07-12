/* 
    Title --- exceptions.cpp

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

    To compile: g++ `pkg-config --libs --cflags serotonina` exceptions.cpp -o exceptions
*/

#include <iostream>

// Includo l'intestazione di Serotonina
#include <serotonina.hpp>

// Uso il namespace di Serotonina
using namespace Serotonina;

int main( void ) {

	// Creo la rete neurale
	Network network( 3, 2, 3, 1 );

	// Eseguo la rete neurale fornendo un numero errato di ingressi
	try {

		// Imposto gli ingressi
		std::vector< T_Precision> inputs;

		inputs.push_back(0);

		// Calcolo l'uscita della rete neurale
		const std::vector< T_Precision > &out = network.Run( inputs );

	// Catturo eventuali eccezioni
	} catch ( Exception &ex ) {

		// Stampo l'errore a schermo e continuo la normale esecuzione
		std::cout << "Runtime error: " << ex.what() << std::endl;
	}


	// Salvo la rete neurale in un percorso non valido
	try {

		network.Save( "" );

	// Catturo eventuali eccezioni
	} catch ( Exception &ex ) {

		// Stampo l'errore a schermo e continuo la normale esecuzione
		std::cout << "Runtime error: " << ex.what() << std::endl;
	}


	// Carico la rete neurale da un percorso non valido
	try {

		network.Load( "none" );

	// Catturo eventuali eccezioni
	} catch ( Exception &ex ) {

		// Stampo l'errore a schermo e continuo la normale esecuzione
		std::cout << "Runtime error: " << ex.what() << std::endl;
	}

	// Messaggio di uscita
	std::cout << "Completed" << std::endl;

	return 0;
}

