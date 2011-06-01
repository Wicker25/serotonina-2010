/* 
	Title --- training_from_array.cpp

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

	const Serotonina::T_Precision inputs[] =	{
												0, 0,
												0, 1,
												1, 0,
												1, 1
											};

	const Serotonina::T_Precision outputs[] =	{
												0,
												1,
												1,
												0
											};

	// Creo la rete neurale
	Serotonina::Network network( 3, 2, 5, 1 );

	// Addestro la rete neurale
	network.Train( inputs, outputs, 4, 0.00001, 100000, 5000 );

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

