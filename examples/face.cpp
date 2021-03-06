/* 
	Title --- face.cpp

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

    To compile: g++ `pkg-config --libs --cflags serotonina` face.cpp -o face
*/

#include <iostream>

// Includo l'intestazione di Serotonina
#include <serotonina.hpp>

// Uso il namespace di Serotonina
using namespace serotonina;

int main( void ) {

	// Creo la rete neurale
	Network network( 3, 280, 15, 1 );

	// Creo l'addestratore della rete neurale
	Trainer trainer( network );

	// Addestro la rete neurale con il metodo IRprop-
	trainer.setParams( 0.5, 1.2 );
	trainer.trainOnFile< algorithms::IRpropMinus >( "train/face.train", 0.0001, 100000, 1 );

	// Salvo la rete in un file
	network.save( "train/face.net" );

	return 0;
}

