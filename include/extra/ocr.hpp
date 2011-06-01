/* 
    Title --- ocr.hpp

    Copyright (C) 2010 Giacomo Trudu - wicker25[at]gmail[dot]com

    This file is part of Serotonina.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _SEROTONINA_OCR_HPP_
#define _SEROTONINA_OCR_HPP_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cfloat>
#include <vector>
#include <fstream>

#include <pthread.h>
#include <signal.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Progress.H>
#include <FL/fl_draw.H>
#include <FL/gl.h>

#include <serotonina.hpp>

#include "extra/color.hpp"

#define _OCR_TITLE_				"Serotonina::OCR - Riconoscimento di un carattere"
#define _OCR_FPS_				30.0
#define _OCR_CHARACTER_NUM_		26

namespace Serotonina { // Namespace di Serotonina

// Struttura di un rettangolo
struct Rectangle {

	int left;
	int top;
	int right;
	int bottom;
};

class Ocr : public Fl_Gl_Window {

	/* STRUMENTO GRAFICO PER IL RICONOSCIMENTO DI UN CARATTERE */

public:

	// Costanti
	enum {

		// Dati sulla tavola da disegno
		ARTBOARD_W		= 100,
		ARTBOARD_H		= 100,
		ARTBOARD_SIZE	= ARTBOARD_W * ARTBOARD_H,
		ARTBOARD_SCALE	= 3,

		// Dimensione della matita
		PENCIL_SIZE = 5,

		// Dati sul carattere
		CHARACTER_W		= 10,
		CHARACTER_H		= 10,
		CHARACTER_SIZE	= CHARACTER_W * CHARACTER_H,
		CHARACTER_SCALE	= 7,
	};

	/** INIZIO METODI STATICI **/

	// Funzione per l'aggiornamento asincrono della finestra
	static void asynchronous_redraw( void *window );

	// Funzione di callback del pulsante "aggiungi carattere"
	static void add_char_button_callback( Fl_Widget *widget, void *data );

	// Funzione di callback del pulsante "addestra reti"
	static void train_button_callback( Fl_Widget *widget, void *data );

	// Avvia l'addestramento delle rete neurali (statico)
	static void *static_start_training( void *data );

	/** FINE METODI STATICI **/


	// Metodi costruttore e distruttore
	Ocr( int width, int height );
	virtual ~Ocr();

	// Pulisce la tavola da disegno
	void ClearArtboard();

	// Pulisce le informazioni sul carattere
	void ClearCharacter();

	// Disegna un tratto di matita nella tavola da disegno
	void DrawPencil( int x, int y );

	// Delimita l'area del carattere
	void FindCharacter();

	// Funzione per la gestione degli eventi della finestra
	virtual int handle( int event );

	// Funzione di disegno della finestra
	virtual void draw();

private:

	// Texture del carattere
	GLuint texture_id;

	// Dati della tavola da disegno
	uint8_t artboard_data[ ARTBOARD_W * ARTBOARD_H ];

	// Dati del carattere
	uint8_t character_data[ CHARACTER_W * CHARACTER_H ];

	// Area di delimitazione del carattere
	Rectangle char_boundary;

	// Rete neurale
	Network *neural_network[_OCR_CHARACTER_NUM_];

	// Id del thread dell'addestramento
	pthread_t thread_id;

	// Flag per controllare se il carattere è già stato estratto
	bool character_exist;

	// Carattere identificato
	char recognized_character;

	// Precisione del riconoscimento
	float precision_character;

	// Finestra di addestramento
	Fl_Window *training_window;

	// Scelta del carattere
	Fl_Choice *character_choice;

	// Pulsante per l'aggiunta del carattere
	Fl_Button *add_char_button;

	// Pulsante per l'addestramento delle reti
	Fl_Button *train_button;

	// Barra del progresso dell'addestramento
	Fl_Progress *train_progress;

	// Imposta la visualizzazione della finestra OpenGL
	void InitOpenGL();

	// Aggiungo un nuovo modello ad un carattere
	void AddCharacterData();

	// Addestra una rete al riconoscimento di un carattere
	void TrainNetwork( int choice );

	// Addestra tutte le reti neurali
	void TrainAllNetwork();

	// Estrae il carattere dalla tavola da disegno
	void GetCharacter();

	// Cerca il carattere con le reti neurali
	void RecognitionCharacter();

	// Disegna una texture
	void DrawTexture( size_t width, size_t height, float scale );

	// Disegna la tavola da disegno sullo schermo
	void DrawArtboard( size_t x, size_t y );

	// Disegna il carattere
	void DrawCharacter( size_t x, size_t y );

	// Disegna l'area di delimitazione del carattere
	void DrawCharBoundary( size_t x, size_t y );
};

}; // Chiudo il namespace di Serotonina

#endif
