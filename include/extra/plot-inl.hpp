/* 
    Title --- plot-inl.hpp

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

#ifndef _SEROTONINA_PLOT_INL_HPP_
#define _SEROTONINA_PLOT_INL_HPP_

#include "extra/plot.hpp"

inline void
Plot::SetMargins( float left, float top, float right, float bottom ) {

	// Imposto i margini del grafico
	this->margins[0] = left;
	this->margins[1] = top;
	this->margins[2] = right;
	this->margins[3] = bottom;
}


inline void
Plot::SetMinValue( float min ) {

	// Imposto il valore minimo del grafico
	this->min_value = min;
}

inline void
Plot::SetMaxValue( float max ) {

	// Imposto il valore massimo del grafico
	this->max_value = max;
}

inline void
Plot::SetXRulerStep( float value ) {

	// Imposto il passo dell'asse delle ascisse
	this->x_ruler_step = value;
}

template < typename Type > void
Plot::PushData( std::vector< Type > data_, const ColorRGBub &color, bool points ) {

	// Controllo che sia stato passato almeno un valore
	if ( !data_.empty() ) {

		// Creo il nuovo insieme di valori
		PlotData new_data = {
								new float[ data_.size() ],
								data_.size(),
								color,
								points
							};

		// Cerco il numero massimo dei dati in un insieme
		if ( this->max_data < new_data.size ) this->max_data = new_data.size;

		// Iteratore
		size_t i = 0;

		// Imposto i dati del grafico
		for ( ; i < new_data.size; i++ ) {

			// Copio i dati del grafico
			new_data.data[i] = data_[i];

			// Cerco i valori massimo e minimo dei dati
			if ( this->min_value > new_data.data[i] ) this->min_value = new_data.data[i];
			if ( this->max_value < new_data.data[i] ) this->max_value = new_data.data[i];
		}

		// Aggiungo i nuovi dati alla lista
		this->data_set.push_back( new_data );
	}
}

template < typename Type > void
Plot::PushData( Type *data_, size_t size, const ColorRGBub &color, bool points ) {

	// Controllo che sia stato passato almeno un valore
	if ( size > 0 ) {

		// Creo il nuovo insieme di valori
		PlotData new_data = {
								new float[ size ],
								size,
								color,
								points
							};

		// Cerco il numero massimo dei dati in un insieme
		if ( this->max_data < new_data.size ) this->max_data = new_data.size;

		// Iteratore
		size_t i = 0;

		// Imposto i dati del grafico
		for ( ; i < new_data.size; i++ ) {

			// Copio i dati del grafico
			new_data.data[i] = data_[i];

			// Cerco i valori massimo e minimo dei dati
			if ( this->min_value > new_data.data[i] ) this->min_value = new_data.data[i];
			if ( this->max_value < new_data.data[i] ) this->max_value = new_data.data[i];
		}

		// Aggiungo i nuovi dati alla lista
		this->data_set.push_back( new_data );
	}

} // Chiudo il namespace di Serotonina

#endif
