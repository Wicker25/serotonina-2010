/* 
    Title --- face-recognition.hpp

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

#ifndef _SEROTONINA_FACE_RECOGNITION_HPP_
#define _SEROTONINA_FACE_RECOGNITION_HPP_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cfloat>
#include <vector>
#include <fstream>

#include <opencv/highgui.h>

#include <pthread.h>
#include <signal.h>

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/fl_draw.H>
#include <FL/gl.h>

#include <serotonina.hpp>

#include "extra/version.hpp"
#include "extra/color.hpp"

#define _FACE_RECOGNITION_TITLE_			"Serotonina::FaceRecognition - Riconoscimento di un volto"
#define _FACE_RECOGNITION_FPS_				30.0
#define _FACE_RECOGNITION_PHOTO_NUMBER_		15
#define _FACE_RECOGNITION_PHOTO_WAIT_		0.4

// Uso il namespace di serotonina;
using namespace Serotonina;

class FaceRecognition : public Fl_Gl_Window {

	/* STRUMENTO GRAFICO PER IL RICONOSCIMENTO DI UN VOLTO */

public:

	// Costanti
	enum {

		// Dimensione del fotogramma
		FRAME_W		= 640,
		FRAME_H		= 480,
		FRAME_SIZE	= FRAME_W * FRAME_H,
		FRAME_RATIO	= 2,

		// Dimensione del volto
		FACE_W		= 20,
		FACE_H		= 14,
		FACE_SIZE	= FACE_W * FACE_H,
		FACE_RATIO	= 2,

		// Rapporto tra volto e sorgente
		SRC_RATIO = 10,

		// Dimensione della sorgente nel fotogramma
		SRC_W = FACE_W * SRC_RATIO,
		SRC_H = FACE_H * SRC_RATIO,

		// Offset della sorgente
		SRC_X = 200,
		SRC_Y = 200,

		// Dimensione dei campioni
		SAMPLE_W = SRC_W / FACE_W,
		SAMPLE_H = SRC_H / FACE_H
	};

	/** INIZIO METODI STATICI **/

	// Funzione per l'aggiornamento asincrono della finestra
	static void asynchronous_redraw( void *window );

	/** FINE METODI STATICI **/


	// Metodi costruttore e distruttore
	FaceRecognition( int width, int height );
	virtual ~FaceRecognition();

	// Funzione per la gestione degli eventi della finestra
	virtual int handle( int event );

	// Funzione di disegno della finestra
	virtual void draw();

private:

	// Periferica del video in ingresso
	CvCapture *capture;

	// Fotogramma corrente
	IplImage *frame;

	// Luminosità del volto
	uint8_t face_data[ FACE_SIZE ];

	// Texture del fotogramma
	GLuint texture_id;

	// Rete neurale
	Serotonina::Network *neural_network;

	// Cronometro dell'applicazione
	float timer;

	// Contatore delle foto
	size_t photo_counter;

	// Stream al file delle foto
	std::ofstream photo_file;

	// Imposta la visualizzazione della finestra OpenGL
	void InitOpenGL();

	// Ritorna la luminosità media di un campione
	uint8_t GetSample( size_t x, size_t y, size_t simple_w, size_t simple_h );

	// Estrae il volto dal fotogramma
	void GetFace();

	// Cerco di riconoscere la faccia
	void FindFace();

	// Disegna una texture
	void DrawTexture( size_t width, size_t height, float scale );

	// Disegna il frame originale
	void DrawFrame( size_t x, size_t y );

	// Disegna il confine del volto
	void DrawFaceBorder( size_t x, size_t y );

	// Disegna la luminosità del volto
	void DrawFace( size_t x, size_t y );
};

#endif
