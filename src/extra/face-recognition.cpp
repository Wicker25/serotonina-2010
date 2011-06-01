/* 
    Title --- face-recognition.cpp

	Copyright (C) 2010 Giacomo Trudu - wicker25[at]gmail[dot]com

	This file is part of Serotonina.

	This library is free software; you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
*/

#ifndef _SEROTONINA_FACE_RECOGNITION_CPP_
#define _SEROTONINA_FACE_RECOGNITION_CPP_

#include "extra/face-recognition.hpp"

#include "extra/color.hpp"
#include "extra/color-inl.hpp"

#include "extra/face-recognition-inl.hpp"

namespace Serotonina { // Namespace di Serotonina

/** INIZIO METODI STATICI **/

void
FaceRecognition::asynchronous_redraw( void *window ) {

	// Ricavo la finestra principale
	FaceRecognition *opengl_window = (FaceRecognition *) window;

	// Forzo il ridisegno della finestra OpenGL
	opengl_window->redraw();

	// Incremento il cronometro
	opengl_window->timer += 1.0 / _FACE_RECOGNITION_FPS_;

	// Riavvio la funzione per l'aggiornamento asincrono della finestra
	Fl::repeat_timeout( 1.0 / _FACE_RECOGNITION_FPS_, FaceRecognition::asynchronous_redraw, window );
}

/** FINE METODI STATICI **/


FaceRecognition::FaceRecognition( int width, int height ) : Fl_Gl_Window( width, height, _FACE_RECOGNITION_TITLE_ ) {

	// Inizializzo le strutture di lavoro
	this->frame = 0;
	this->photo_counter = -1;

	// Creo la rete neurale dal file
	this->neural_network = new Network( "data/face/face.net" );

	// Inizializzo la periferica video
	this->capture = cvCaptureFromCAM( -1 );

	// Controllo se ci sono stati dei problemi durante l'inizializzazione
	if( !this->capture ) {

		// Communico l'errore all'utente
		fprintf( stderr, "(!!) Errore fatale in `cvCreateCameraCapture(%d)`\n", -1 );

		// Esco dall'applicazione
		exit(1);
	}

	// Imposto la dimensione del fotogramma in ingresso
	cvSetCaptureProperty( this->capture, CV_CAP_PROP_FRAME_WIDTH, FRAME_W );
	cvSetCaptureProperty( this->capture, CV_CAP_PROP_FRAME_HEIGHT, FRAME_H );

	// Genero una texture
	glGenTextures( 1, &this->texture_id );

	// Avvio la funzione per l'aggiornamento asincrono della finestra
	Fl::add_timeout( 1.0 / _FACE_RECOGNITION_FPS_, FaceRecognition::asynchronous_redraw, (void *) this );
}

FaceRecognition::~FaceRecognition() {

	// Elimino la rete neurale
	delete this->neural_network;

	// Elimino la texture
	glDeleteTextures( 1, &this->texture_id );

	// Rilascio la periferica video
	cvReleaseCapture( &this->capture );
}

void
FaceRecognition::InitOpenGL() {

	// Imposto la visualizzazione
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport( 0.0, 0.0, (GLsizei) this->w(), (GLsizei) this->h() );
	glOrtho( 0.0, (GLdouble) this->w(), (GLfloat) this->h(), 0.0, -1.0f, 1.0f );

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

uint8_t
FaceRecognition::GetSample( size_t x, size_t y, size_t simple_w, size_t simple_h ) {

	// Luminosità media del campione
	uint8_t luminance;

	// Ricavo le informazioni sul fotogramma
	size_t width	= this->frame->width;
	size_t height	= this->frame->height;
	size_t step		= this->frame->widthStep;
	size_t channels	= this->frame->nChannels;

	// Ricavo un puntatore ai dati del fotogramma
	uint8_t *data = (uint8_t *) this->frame->imageData;

	// Somma totale dei canale del colore
	size_t color_sum = 0;

	// Numero dei pixel previsti per campione
	const size_t pixels_per_sample = simple_w * simple_h;

	// Iteratore
	size_t i = 0;

	// Calcolo la somma totale del colore dei pixel nel campione
	for( ; i < pixels_per_sample; i++ ) {

		// Ricavo le coordinate del pixel corrente
		size_t px = x + ( i % simple_w ) - simple_w;
		size_t py = y + ( i / simple_w ) - simple_h;

		// Controllo l'esistenza del pixel
		if ( px < width && py < height ) {

			// Ricavo l'indice del pixel nel buffer
			size_t pixel_index = py * step + px * channels;

			// Incremento la somma totale dei canale del colore
			color_sum += ( data[ pixel_index + 2 ] + data[ pixel_index + 1 ] + data[ pixel_index ] );
		}
	}

	// Calcolo la luminosità media del campione
	luminance = (uint8_t) ( color_sum / pixels_per_sample / 3 );

	// Ritorno la luminosità media del campione
	return luminance;
}

void
FaceRecognition::GetFace() {

	// Ricavo la dimensione del volto
	size_t width = FACE_W;

	// Luminosità media del volto
	//float avg = 0.0;

	// Iteratori
	size_t i = 0;

	// Eseguo il sottocampionamento del volto riflettendolo orizzontalmente
	for ( ; i < FACE_SIZE; i++ ) {

		// Calcolo il valore del pixel del volto
		this->face_data[i] = this->GetSample(	( FRAME_W - SRC_W - SRC_X ) + ( width - i % width ) * SAMPLE_W,
												SRC_Y + ( i / width ) * SAMPLE_H, SAMPLE_W, SAMPLE_H );

		// Conto la luminosità del pixel
		//avg += this->face_data[i];
	}

	/*
	// Calcolo la media della luminosità
	avg /= (float) FACE_SIZE;

	// Valore del pixel con la correzione del contrasto
	int contrast;

	// Parametro del contrasto
	float delta = 20.0 / 100.0;

	// Correggo il contrasto dell'immagine
	for ( i = 0; i < FACE_SIZE; i++ ) {

		// Calcolo il nuovo valore del pixel
		contrast = ( ( ( ( (float) this->face_data[i] + avg ) / 2.0 / 255.0 ) - 0.5 ) * __POW2__( 1.0 + delta ) + 0.5 ) * 255;

		// Memorizzo il nuovo valore del pixel
		this->face_data[i] = (uint8_t) __BETWEEN__( contrast, 0, 255 );
	}
	*/
}

void
FaceRecognition::FindFace() {

	// Alloco la memoriza necessaria a contenere gli input della rete
	T_Precision *input = new T_Precision[FACE_SIZE];

	// Iteratore
	size_t i = 0;

	// Preparo l'input della rete
	for ( ; i < FACE_SIZE; i++ ) {

		input[i] = (T_Precision) this->face_data[i] / 255.0;
	}

	// Precisione del riconoscimento
	T_Precision check;

	// Analizzo il volto
	if ( ( check = this->neural_network->Run( input )[0] ) > 0.80 ) {

		// Imposto il colore
		glColor3ub( 255u, 0u, 0u );

		// Costruisco il messaggio di successo
		char buffer[50];
		snprintf( buffer, 50, "Volto riconosciuto! (%.0f%%)\n", check * 100 );

		// Disegno il messaggio
		gl_draw( buffer, 40, 40 ); 
	}

	// Libero la memoria
	delete input;
}

int
FaceRecognition::handle( int event ) {

	// Controllo gli eventi della finestra
	switch( event ) {

		// Pressione di un tasto
		case FL_KEYDOWN: {

			// Controllo il tasto premuto
			switch ( Fl::event_key() ) { 

				// Funzione di uscita
				case FL_Escape: {

					exit(0);
				}

				// Funzione di acquisizione delle foto
				case 'a': {

					// Controllo se è già in corso una sequenza di acquisizione delle foto
					if ( this->photo_counter >= _FACE_RECOGNITION_PHOTO_NUMBER_ ) {

						// Azzero il contatore delle foto ed avvio la nuova raccolta
						this->photo_counter = 0;

						// Reimposto il cronometro
						this->timer = 0;

						// Apro il file di destinazione
						this->photo_file.open( "data/face/train/face.train", std::ios_base::out );

						// Imposto la precisione dei numeri con la virgola
						this->photo_file.setf( std::ios::fixed, std::ios::floatfield );
						this->photo_file.precision( 4 );
					}

					break;
				}

				default: break;
			}

			return 1;
		}

		// Richiamo la funzione superiore
		default: return Fl_Gl_Window::handle( event );
	}
}

void
FaceRecognition::DrawTexture( size_t width, size_t height, float scale ) {

	// Abilito l'utilizzo delle texture
	glEnable( GL_TEXTURE_2D );

	// Imposto i parametri della texture
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	// Disegno la texture
	glBegin( GL_TRIANGLE_STRIP );

		glTexCoord2f( 0.0, 1.0f );	glVertex2f( 0.0, (GLfloat) height * (GLfloat) scale );
		glTexCoord2f( 0.0, 0.0 );	glVertex2f( 0.0, 0.0 );
		glTexCoord2f( 1.0f, 1.0f );	glVertex2f( (GLfloat) width * (GLfloat) scale, (GLfloat) height * (GLfloat) scale );
		glTexCoord2f( 1.0f, 0.0 );	glVertex2f( (GLfloat) width * (GLfloat) scale, 0.0 );

	glEnd();

	// Disabilito l'utilizzo delle texture
	glDisable( GL_TEXTURE_2D );
}

void
FaceRecognition::DrawFrame( size_t x, size_t y ) {

	// Scelgo la texture
	glBindTexture( GL_TEXTURE_2D, this->texture_id );

	// Costruisco la texture usando un immagine in scala di grigio
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGB, this->frame->width, this->frame->height,
					0, GL_BGR, GL_UNSIGNED_BYTE, this->frame->imageData );

	// Memorizzo la matrice corrente
	glPushMatrix();

	// Imposto la posizione
	glTranslatef( (GLfloat) x, (GLfloat) y, 0.f );

	// Imposto la scala della cornice
	glScalef( (GLfloat) 1.0 / (GLfloat) FRAME_RATIO, (GLfloat) 1.0 / (GLfloat) FRAME_RATIO, 1.0 );

	// Imposto il colore
	glColor3ub( 255u, 255u, 255u );

	// Abilito l'utilizzo delle texture
	glEnable( GL_TEXTURE_2D );

	// Imposto i parametri della texture
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	// Disegno il la texture
	glBegin( GL_QUADS );

		glTexCoord2f( 1.0f, 0.0 );	glVertex2f( 0.0, 0.0 );
		glTexCoord2f( 0.0, 0.0 );	glVertex2f( (GLfloat) this->frame->width, 0.0 );
		glTexCoord2f( 0.0, 1.0f );	glVertex2f( (GLfloat) this->frame->width, (GLfloat) this->frame->height );
		glTexCoord2f( 1.0f, 1.0f );	glVertex2f( 0.0, (GLfloat) this->frame->height );

	glEnd();

	// Disabilito l'utilizzo delle texture
	glDisable( GL_TEXTURE_2D );

	// Imposto lo spessore della linea
	glLineWidth( 2 );

	// Imposto il colore
	glColor3ub( 80u, 200u, 0u );

	// Disegno il contorno
	glBegin( GL_LINE_LOOP );

		glVertex2f(	0.0, 0.0 );
		glVertex2f(	0.0, (GLfloat) this->frame->height );
		glVertex2f(	(GLfloat) this->frame->width, (GLfloat) this->frame->height );
		glVertex2f(	(GLfloat) this->frame->width, 0.0 );

	glEnd();

	// Ripristino la matrice precedente
	glPopMatrix();
}

void
FaceRecognition::DrawFaceBorder( size_t x, size_t y ) {

	// Memorizzo la matrice corrente
	glPushMatrix();

	// Imposto la posizione
	glTranslatef( (GLfloat) x, (GLfloat) y, 0.f );

	// Imposto la scala della cornice
	glScalef( (GLfloat) 1.0 / (GLfloat) FRAME_RATIO, (GLfloat) 1.0 / (GLfloat) FRAME_RATIO, 1.0 );

	// Imposto lo spessore della linea
	glLineWidth( 1 );

	// Imposto il colore
	glColor3ub( 255u, 0u, 0u );

	// Disegno la cornice del volto
	glBegin( GL_LINE_LOOP );

		glVertex2f( (GLfloat) SRC_X, (GLfloat) SRC_Y );
		glVertex2f( (GLfloat) SRC_X + (GLfloat) SRC_W, (GLfloat) SRC_Y );
		glVertex2f( (GLfloat) SRC_X + (GLfloat) SRC_W, (GLfloat) SRC_Y + (GLfloat) SRC_H );
		glVertex2f( (GLfloat) SRC_X, (GLfloat) SRC_Y + (GLfloat) SRC_H );

	glEnd();

	// Imposto il colore
	glColor3ub( 255u, 255u, 0u );

	// Disegno il contorno dell'occhio sinistro
	glBegin( GL_LINE_LOOP );

		glVertex2f( (GLfloat) SRC_X, (GLfloat) SRC_Y + (GLfloat) SRC_H * 0.20 );
		glVertex2f(	(GLfloat) SRC_X + (GLfloat) SRC_W * 0.40, (GLfloat) SRC_Y + (GLfloat) SRC_H * 0.20 );
		glVertex2f(	(GLfloat) SRC_X + (GLfloat) SRC_W * 0.40, (GLfloat) SRC_Y + (GLfloat) SRC_H * 0.50 );
		glVertex2f(	(GLfloat) SRC_X, (GLfloat) SRC_Y + (GLfloat) SRC_H * 0.50 );

	glEnd();

	// Disegno il contorno dell'occhio destro
	glBegin( GL_LINE_LOOP );

		glVertex2f( (GLfloat) SRC_X + (GLfloat) SRC_W, (GLfloat) SRC_Y + (GLfloat) SRC_H * 0.20 );
		glVertex2f( (GLfloat) SRC_X + (GLfloat) SRC_W - (GLfloat) SRC_W * 0.40, (GLfloat) SRC_Y + (GLfloat) SRC_H * 0.20 );
		glVertex2f( (GLfloat) SRC_X + (GLfloat) SRC_W - (GLfloat) SRC_W * 0.40, (GLfloat) SRC_Y + (GLfloat) SRC_H * 0.50 );
		glVertex2f( (GLfloat) SRC_X + (GLfloat) SRC_W, (GLfloat) SRC_Y + (GLfloat) SRC_H * 0.50 );

	glEnd();

	// Imposto il colore
	glColor3ub( 0u, 0u, 255u );

	// Disegno il contorno del naso
	glBegin( GL_LINE_LOOP );

	// Disegno il contorno dell'occhio sinistro
	glBegin( GL_LINE_LOOP );

		glVertex2f(	(GLfloat) SRC_X + (GLfloat) SRC_W * 0.35, (GLfloat) SRC_Y + (GLfloat) SRC_H * 0.50 );
		glVertex2f(	(GLfloat) SRC_X + (GLfloat) SRC_W * 0.65, (GLfloat) SRC_Y + (GLfloat) SRC_H * 0.50 );
		glVertex2f(	(GLfloat) SRC_X + (GLfloat) SRC_W * 0.65, (GLfloat) SRC_Y + (GLfloat) SRC_H * 0.95 );
		glVertex2f(	(GLfloat) SRC_X + (GLfloat) SRC_W * 0.35, (GLfloat) SRC_Y + (GLfloat) SRC_H * 0.95 );

	glEnd();

	// Ripristino la matrice precedente
	glPopMatrix();
}

void
FaceRecognition::DrawFace( size_t x, size_t y ) {

	// Scelgo la texture
	glBindTexture( GL_TEXTURE_2D, this->texture_id );

	// Costruisco la texture usando un immagine in scala di grigio
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_LUMINANCE, FACE_W, FACE_H,
					0, GL_LUMINANCE, GL_UNSIGNED_BYTE, this->face_data );

	// Memorizzo la matrice corrente
	glPushMatrix();

	// Imposto la posizione della texture
	glTranslatef( (GLfloat) x, (GLfloat) y, 0.0 );

	// Imposto la scala della cornice
	glScalef( (GLfloat) 1.0 / (GLfloat) FACE_RATIO, (GLfloat) 1.0 / (GLfloat) FACE_RATIO, 1.0 );

	// Imposto il colore
	glColor3ub( 255u, 255u, 255u );

	// Disegno la texture della luminosità del volto
	this->DrawTexture( FACE_W, FACE_H, SRC_RATIO );

	// Imposto lo spessore della linea
	glLineWidth( 2 );

	// Imposto il colore
	glColor3ub( 80u, 200u, 0u );

	// Disegno il contorno
	glBegin( GL_LINE_LOOP );

		glVertex2f(	0.0, 0.0 );
		glVertex2f(	0.0, (GLfloat) FACE_H * (GLfloat) SRC_RATIO );
		glVertex2f(	(GLfloat) FACE_W * (GLfloat) SRC_RATIO, (GLfloat) FACE_H* (GLfloat) SRC_RATIO );
		glVertex2f(	(GLfloat) FACE_W * (GLfloat) SRC_RATIO, 0.0 );

	glEnd();

	// Ripristino la matrice precedente
	glPopMatrix();
}

void
FaceRecognition::draw() {

	// Controllo se è stata impostata la visualizzazione
	if ( !this->valid() ) {

		// Imposto la visualizzazione della finestra OpenGL
		this->InitOpenGL();

		// Modifico il flag di controllo
		this->valid( true );

		// Imposto il carattere del testo
		gl_font( FL_COURIER, 14 );
	}

	// Pulisco la finestra delle OpenGL
	glClear( GL_COLOR_BUFFER_BIT );

	// Catturo il fotogramma dalla camera
	this->frame = cvQueryFrame( this->capture );

	// Verifico l'esistenza del fotogramma
	if ( this->frame ) {

		// Disegno il fotogramma
		this->DrawFrame( 20, 20 );

		// Disegno il confine del volto
		this->DrawFaceBorder( 20, 20 );

		// Estraggo la luminosità del volto
		this->GetFace();

		// Disegno la luminosità del volto
		this->DrawFace( 20 + FRAME_W / 2 + 20, 20 );

		this->FindFace();
	}

	// Controllo se è stata avviata la sequenza di acquisizione delle foto e se è finito l'intervallo di attesa
	if (	this->photo_counter < _FACE_RECOGNITION_PHOTO_NUMBER_ &&
			this->timer > _FACE_RECOGNITION_PHOTO_WAIT_ ) {

		printf( "Salvata la foto numero %zu!\n", this->photo_counter + 1 );

		// Reimposto il cronometro
		this->timer = 0;

		// Iteratore
		size_t i = 0;

		// Converto e memorizzo la foto acquisita
		for ( ; i < FACE_SIZE; i++ ) {

			this->photo_file << ( (float) this->face_data[i] / 255.0 ) << " ";
		}

		// Aggiungo la risposta desiderata
		this->photo_file << std::endl;

		// Incremento il numero delle foto acquisite
		this->photo_counter++;

		// Controllo se è terminata la sequenza di acquisizione delle foto
		if ( this->photo_counter == _FACE_RECOGNITION_PHOTO_NUMBER_ ) {

			// Chiudo lo stream al file delle foto
			this->photo_file.close();
		}
	}

	// Richiamo la funzione originale
	Fl_Gl_Window::draw();
}

}; // Chiudo il namespace di Serotonina


/* INIZIO FUNZIONE PRINCIPALE */

int
main( void ) {

	// Creo la finestra principale
	Serotonina::FaceRecognition face_recognition_window( 480, 320 );

	//face_recognition_window.resizable( face_recognition_window );
	face_recognition_window.show();

	// Abilito il multithreading con le FLTK
	Fl::lock();

	// Richiamo il loop principale
	return Fl::run();
}

/* FINE FUNZIONE PRINCIPALE */

#endif
