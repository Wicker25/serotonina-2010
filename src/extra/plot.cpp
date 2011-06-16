/* 
    Title --- plot.cpp

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
    along with Serotonina.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SEROTONINA_PLOT_CPP_
#define _SEROTONINA_PLOT_CPP_

#include "extra/plot.hpp"

#include "extra/color.hpp"
#include "extra/color-inl.hpp"

#include "extra/plot-inl.hpp"

Plot::Plot( int x, int y, int width, int height ) : Fl_Gl_Window( x, y, width, height ) {

	// Inizializzo i parametri
	this->max_data = 0;

	// Imposto i margini del grafico
	this->SetMargins( 0.10 + 0.02, 0.1, 0.1, 0.10 + 0.02 );

	// Imposto il valore minimo del grafico
	this->SetMinValue( FLT_MAX );
	// Imposto il valore massimo del grafico
	this->SetMaxValue( -FLT_MAX );

	// Imposto il passo dell'asse delle ascisse
	this->SetXRulerStep( 1.0 );
}

Plot::~Plot() {

	// Libero la memoria
	this->ClearData();
}

void
Plot::ClearData() {

	// Reinizializzo i parametri del grafico
	this->min_value	= FLT_MAX;
	this->max_value	= -FLT_MAX;
	this->max_data	= 0;

	// Iteratore
	size_t i = 0;

	// Libero la memoria occupata dai dati del grafico
	for ( ; i < this->data_set.size(); i++ ) {

		// Dealloco la memoria dei dati
		delete this->data_set[i].data;
	}

	// Svuoto la lista dei dati
	this->data_set.clear();
}

void
Plot::InitOpenGL() {

	// Imposto la visualizzazione
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport( 0.0, 0.0, (GLsizei) this->w(), (GLsizei) this->h() );
	glOrtho( 0.0, (GLdouble) this->w(), 0.0, (GLdouble) this->h(), -1.0f, 1.0f );

	// Disabilito la gestione automatica della profondità
	glDisable( GL_DEPTH_TEST );

	// Abilito l'alpha blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_ALPHA_TEST );
	glAlphaFunc( GL_GREATER, 0.0 );

	// Imposto il colore di background
	glClearColor( 0.0, 0.0, 0.0, 0.0 );

	// Imposto l'antialiasing
	glHint( GL_POINT_SMOOTH_HINT, true );
	glHint( GL_LINE_SMOOTH_HINT, true );
}

void
Plot::DrawAxis( float offx, float offy, float min, float max, float xdiv, float ydiv, float step_x, float step_y ) {

	// Calcolo la proporzione del grafico
	float eps = (float) this->w() / 300.0;

	// Calcolo la lunghezza degli assi
	float x_axis = (float) this->max_data * xdiv;

	// Imposto il colore
	glColor3ub( 255u, 255u, 255u );

	// Imposto la dimensione della linea
	glLineWidth( (GLfloat) eps );

	// Disegno gli assi del piano cartesiano
	glBegin( GL_LINES );

		// Controllo se l'asse delle ascisse è visibile
		if ( min <= 0 && max >= 0 ) {

			// Asse delle ascisse
			glVertex2f( (GLfloat) offx, (GLfloat) offy );
			glVertex2f( (GLfloat) offx + (GLfloat) x_axis, (GLfloat) offy );

			glVertex2f( (GLfloat) offx + (GLfloat) x_axis, (GLfloat) offy - 4 );
			glVertex2f( (GLfloat) offx + (GLfloat) x_axis, (GLfloat) offy + 3 );
		}

		// Asse delle ordinate
		glVertex2f( (GLfloat) offx, (GLfloat) offy + max * ydiv );
		glVertex2f( (GLfloat) offx, (GLfloat) offy + min * ydiv );

	glEnd();

	// Imposto il carattere del testo
	gl_font( FL_HELVETICA, (int) ( 8.0 * eps ) );

	// Controllo se l'asse delle ascisse è visibile
	if ( min <= 0 && max >= 0 ) {

		// Disegno la nomenclatura dell'asse delle ascisse
		gl_draw( "x", offx + x_axis - (float) gl_width( "x" ) + 8.0 * (GLfloat) eps, offy );
	}

	// Disegno la nomenclatura dell'asse delle ordinate
	gl_draw( "y", offx + 6.0 * (GLfloat) eps, offy + max * ydiv + (float) gl_width( "y" ) );

	// Imposto il colore
	glColor3ub( 255u, 255u, 255u );

	// Imposto la dimensione della linea
	glLineWidth( (GLfloat) eps );

	// Imposto il carattere del testo
	gl_font( FL_HELVETICA, (int) ( 8.0 * eps ) );

	// Buffer del testo		
	char buffer[20];

	// Iteratore
	float i;

	// Controllo se si vuole disegnare il righello nell'asse delle ascisse
	if ( this->x_ruler_step ) {

		// Flag di controllo
		bool alternate = false;

		// Disegno le tacche dell'asse delle ascisse
		for ( i = 0; i <= (float) this->data_set[0].size; i += step_x ) {

			// Calcolo la posizione verticale
			float xpos = offx + i * xdiv;

			// Disegno le tacche dell'asse
			glBegin( GL_LINES );

				glVertex2f( (GLfloat) xpos, (GLfloat) offy );
				glVertex2f( (GLfloat) xpos, (GLfloat) offy - 5 * eps );

			glEnd();

			// Controllo se si stanno usando numeri molto piccoli
			if ( step_x < 1.0 )

				snprintf( buffer, 20, "%.2f", i * this->x_ruler_step );
			else
				snprintf( buffer, 20, "%.1f", i * this->x_ruler_step );

			// Disegno il valore della tacca nell'asse
			if ( (float) this->data_set[0].size / step_x < 5.0 || !alternate )
				gl_draw( buffer, (float) xpos - (float) gl_width( buffer ) / 2.0f, offy - 17.0 * eps ); 

			// Visualizzo le etichette a sbalzi
			alternate = ( alternate ) ? false : true;
		}
	}

	// Disegno le tacche dell'asse delle ordinate
	for ( i = min; i <= max; i += step_y ) {

		// Calcolo la posizione verticale
		float ypos = offy + i * ydiv;

		// Disegno le tacche dell'asse
		glBegin( GL_LINES );

			glVertex2f( (GLfloat) offx, (GLfloat) ypos );
			glVertex2f( (GLfloat) offx + 5 * eps, (GLfloat) ypos );

		glEnd();

		// Controllo se si stanno usando numeri molto piccoli
		if ( step_y < 1.0 )

			snprintf( buffer, 20, "%.2f", i );
		else
			snprintf( buffer, 20, "%.1f", i );

		// Disegno il valore della tacca nell'asse
		gl_draw( buffer, (float) offx - (float) gl_width( buffer ) - 10, ypos - 3 ); 
	}
}

void
Plot::DrawData( size_t index, float offx, float offy, float xdiv, float ydiv ) {

	// Iteratore
	size_t i;

	// Calcolo la proporzione del grafico
	float eps = (float) this->w() / 300.0;

	// Imposto il colore
	glColor3ub( this->data_set[index].color.red,
				this->data_set[index].color.green,
				this->data_set[index].color.blue );

	// Imposto la dimensione della linea
	glLineWidth( (GLfloat) eps * 1.5 );

	// Imposto la sfumatura delle linee
	glEnable( GL_LINE_SMOOTH );

	// Disegno i dati del grafico
	glBegin( GL_LINE_STRIP );

		for ( i = 0; i < this->data_set[index].size; i++ ) {

			glVertex2f(	(GLfloat) offx + (GLfloat) eps * 1.5 + (GLfloat) i * xdiv,
						(GLfloat) offy + (GLfloat) ( this->data_set[index].data[i] * ydiv ) );
		}

	glEnd();

	// Disabilito la sfumatura delle linee
	glDisable( GL_LINE_SMOOTH );

	// Controllo se sono stati abilitati i punti
	if ( this->data_set[index].points ) {

		// Imposto la dimensione della linea
		glLineWidth( (GLfloat) eps );

		// Imposto la dimensione dei punti
		glPointSize( 3.5 * (GLfloat) eps );

		// Imposto la sfumatura dei punti
		glEnable( GL_POINT_SMOOTH );

		// Evidenzio i punti conosciuti
		glBegin( GL_POINTS );

			for ( i = 0; i < this->data_set[index].size; i++ ) {

				glVertex2f(	(GLfloat) offx + (GLfloat) eps * 1.5 + (GLfloat) i * xdiv,
							(GLfloat) offy + (GLfloat) ( this->data_set[index].data[i] * ydiv ) );
			}

		glEnd();

		// Disabilito la sfumatura dei punti
		glDisable( GL_POINT_SMOOTH );
	}
}

void
Plot::draw() {

	// Controllo se è stata impostata la visualizzazione
	if ( !this->valid() ) {

		// Imposto la visualizzazione della finestra OpenGL
		this->InitOpenGL();

		// Modifico il flag di controllo
		this->valid( true );
	}

	// Pulisco la finestra delle OpenGL
	glClear( GL_COLOR_BUFFER_BIT );

	// Controllo che sia almeno un dato da disegnare
	if ( !this->data_set.empty() ) {

		// Calcolo il passo aprossimato degli assi
		float step_x = std::pow( 10.0, std::floor( std::log10( (float) this->data_set[0].size ) ) );
		float step_y = std::pow( 10.0, std::floor( std::log10( this->max_value - this->min_value ) ) ) / 2.0;

		// Calcolo il range di valori che verranno utilizzati nella rappresentazione
		float max = std::ceil( this->max_value / step_y ) * step_y;
		float min = std::floor( this->min_value / step_y ) * step_y;

		// Calcolo lo spazio riservato ai grafici tenendo conto dei margini
		float graph_width	= (float) this->w() - ( this->margins[0] + this->margins[2] ) * (float) this->w();
		float graph_height	= (float) this->h() - ( this->margins[1] + this->margins[3] ) * (float) this->h();

		// Calcolo le distanze tra un valore e un altro
		float xdiv = graph_width / (float) this->max_data;
		float ydiv = graph_height / (float) ( max - min );

		// Calcolo l'offset dei punti
		float offx = (float) this->margins[0] * (float) this->w();
		float offy = (float) this->margins[3] * (float) this->h() - min * ydiv;

		// Disegno gli assi del piano cartesiano
		this->DrawAxis( offx, offy, min, max, xdiv, ydiv, step_x, step_y );

		// Iteratore
		size_t i = 0;

		// Disegno i dati del grafico
		for ( ; i < this->data_set.size(); i++ ) {

			this->DrawData( i, offx, offy, xdiv, ydiv );
		}
	}

	// Richiamo la funzione originale
	Fl_Gl_Window::draw();
}

#endif
