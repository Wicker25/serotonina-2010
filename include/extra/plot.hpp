/* 
    Title --- plot.hpp

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

#ifndef _SEROTONINA_PLOT_HPP_
#define _SEROTONINA_PLOT_HPP_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cfloat>
#include <vector>
#include <cmath>

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/fl_draw.H>
#include <FL/gl.h>

#include "extra/version.hpp"
#include "extra/color.hpp"

// Struttura di un insieme di dati del grafico
struct PlotData {

	float *data;		// Dati grezzi
	size_t size;		// Numero dei dati

	ColorRGBub color;	// Colore dei dati nel grafico
	bool points;		// Flag per l'evidenziazione dei punti
};

class Plot : public Fl_Gl_Window {

	/* GRAFICO PER VETTORI MONODIMENSIONALI */

public:

	// Metodi costruttore e distruttore
	Plot( int x, int y, int width, int height );
	virtual ~Plot();

	// Aggiunge un insieme di dati al grafico
	template < typename Type >
	void PushData( std::vector< Type > data_, const ColorRGBub &color, bool points );
	template < typename Type >
	void PushData( Type *data_, size_t size, const ColorRGBub &color, bool points );

	// Pulisce tutti i dati del grafico
	void ClearData();

	// Imposta i margini del grafico
	void SetMargins( float left, float top, float right, float bottom );

	// Imposta il valore minimo del grafico
	void SetMinValue( float min );
	// Imposta il valore massimo del grafico
	void SetMaxValue( float max );

	// Imposta il passo dell'asse delle ascisse
	void SetXRulerStep( float value );

	// Funzione di disegno della finestra
	virtual void draw();

private:

	// Vettore dei dati
	std::vector< PlotData > data_set;

	// Valori minimo, massimo e medio dei dati del grafico (in millesimi)
	float min_value;
	float max_value;

	// Passo dell'asse delle ascisse
	float x_ruler_step;

	// Numero massimo dei dati in un insieme
	size_t max_data;

	// Margini del grafico
	float margins[4];

	// Imposta la visualizzazione della finestra OpenGL
	void InitOpenGL();

	// Disegna gli assi del piano cartesiano
	void DrawAxis( float offx, float offy, float min, float max, float xdiv, float ydiv, float step_x, float step_y );

	// Disegna i dati del grafico
	void DrawData( size_t index, float offx, float offy, float xdiv, float ydiv );
};

#endif
