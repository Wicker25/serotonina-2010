/* 
	Title --- xor.cpp

    Copyright (C) 2010 Giacomo Trudu - wicker25[at]gmail[dot]com

    This file is part of Serotonina.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <serotonina.hpp>

int main( int argc, char **argv ) {

	// Creo la rete neurale
	Serotonina::Network network( 3, 280, 15, 1 );

	// Addestro la rete neurale
	network.SetTrainingAlgorithm( Serotonina::TRAIN_RPROP );
	network.TrainOnFile( "train/face.train", 0.0001, 100000, 1 );

	// Salvo la rete in un file
	network.Save( "train/face.net" );

	return 0;
}

