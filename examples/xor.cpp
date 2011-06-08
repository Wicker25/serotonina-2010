/* 
	Title --- xor.cpp

    Copyright (C) 2010 Giacomo Trudu - wicker25[at]gmail[dot]com

    This file is part of Serotonina.

    Serotonina is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Serotonina is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Serotonina.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <serotonina.hpp>

int main( int argc, char **argv ) {

	// Creo la rete neurale
	Serotonina::Network network( 3, 2, 5, 1 );

	// Addestro la rete neurale
	network.SetTrainingAlgorithm( Serotonina::TRAIN_BATCH );
	network.TrainOnFile( "train/xor.train", 0.000001, 100000, 5000 );

	// Avvio la rete neurale
	Serotonina::T_Precision in[2];
	const Serotonina::T_Precision *out;

	// Eseguo alcuni test con la rete neurale
	in[0] = 0;
	in[1] = 0;
	out = network.Run( in );
	printf( "Run neural network ( %.0f XOR %.0f ) = %f\n", in[0], in[1], out[0] );

	in[0] = 0;
	in[1] = 1;
	out = network.Run( in );
	printf( "Run neural network ( %.0f XOR %.0f ) = %f\n", in[0], in[1], out[0] );

	in[0] = 1;
	in[1] = 0;
	out = network.Run( in );
	printf( "Run neural network ( %.0f XOR %.0f ) = %f\n", in[0], in[1], out[0] );

	in[0] = 1;
	in[1] = 1;
	out = network.Run( in );
	printf( "Run neural network ( %.0f XOR %.0f ) = %f\n", in[0], in[1], out[0] );

	return 0;
}

