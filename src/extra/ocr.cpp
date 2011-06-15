/* 
    Title --- ocr.cpp

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

#ifndef _SEROTONINA_OCR_CPP_
#define _SEROTONINA_OCR_CPP_

#include "extra/ocr.hpp"

#include "extra/color.hpp"
#include "extra/color-inl.hpp"

#include "extra/ocr-inl.hpp"

/** INIZIO METODI STATICI **/

void
Ocr::asynchronous_redraw( void *window ) {

	// Ricavo la finestra principale
	Ocr *opengl_window = (Ocr *) window;

	// Forzo il ridisegno della finestra OpenGL
	opengl_window->redraw();

	// Riavvio la funzione per l'aggiornamento asincrono della finestra
	Fl::repeat_timeout( 1.0 / _OCR_FPS_, Ocr::asynchronous_redraw, window );
}

void
Ocr::train_button_callback( Fl_Widget *widget, void *data ) {

	// Ricavo un puntatore alla finestra principale
	Ocr *ocr = static_cast< Ocr * >( data );

	// Controllo se l'addestramento è già stato avviato
	if ( ocr->thread_id == 0 ) { 

		// Modifico l'etichetta del pulsante
		widget->label( "Interrompi" );

		// Avvio il thread per l'addestramento della rete
		pthread_create( &ocr->thread_id, NULL, Ocr::static_start_training, data );

	} else {

		// Termino il thread per l'addestramento
		if ( ocr->thread_id )
			pthread_cancel( ocr->thread_id );

		// Memorizzo la fine del thread
		ocr->thread_id = 0;

		// Modifico l'etichetta del pulsante
		widget->label( "Addestra le reti" );
	}
}

/** FINE METODI STATICI **/


Ocr::Ocr( int width, int height ) : Fl_Gl_Window( width, height, _OCR_TITLE_ ) {

	// Inizializzo le strutture
	this->thread_id				= 0;
	this->precision_character	= 0.0;

	// Pulisco la tavola da disegno
	this->ClearArtboard();

	// Pulisco le informazioni sul carattere
	this->ClearCharacter();

	// Percorso della rete
	char path[100] = "data/character/#.net";

	// Iteratore
	size_t i = 0;

	// Carico le reti neurali artificiali
	for ( ; i < _OCR_CHARACTER_NUM_; i++ ) {

		// Preparo il percorso al file della rete neurale
		path[15] = 'a' + i;

		// Log di lavoro
		printf( "Ricordo la lettera %c ...\n", 'A' + (char) i );

		// Carico la rete neurale dal file
		this->neural_network[i] = new Network( path );
	}

	// Avvio la funzione per l'aggiornamento asincrono della finestra
	Fl::add_timeout( 1.0 / _OCR_FPS_, Ocr::asynchronous_redraw, (void *) this );


	/** FINESTRA DI ADDESTRAMENTO **/

	// Creo la finestra di addestramento
	this->training_window = new Fl_Window( 410, 70, "Aggiungi un nuovo carattere..." );

	// Inizio la costruzione della finestra di addestramento
	this->training_window->begin();

	// Creo la scelta del carattere
	this->character_choice = new Fl_Choice( 10, 10, 120, 24 );

	// Buffer di lavoro
	char buffer[2];

	// Creo le voci del menù di scelta
	for ( i = 0; i < _OCR_CHARACTER_NUM_; i++ ) {

		// Creo la stringa della voce
		snprintf( buffer, 3, "%c", 'A' + (char) i );

		// Aggiungo la voce al menù
		this->character_choice->add( buffer );
	}

	// Aggiungo la voce "Nessun carattere" al menù
	this->character_choice->add( "- Non e' un carattere -" );

	// Creo il pulsante per l'aggiunta del carattere
	this->add_char_button = new Fl_Button( 140, 10, 140, 24, "Aggiungi carattere" );
	this->add_char_button->callback( (Fl_Callback *) Ocr::add_char_button_callback, (void *) this );

	// Creo il pulsante per l'addestramento delle reti
	this->train_button = new Fl_Button( 285, 10, 115, 24, "Addestra le reti" );
	this->train_button->callback( (Fl_Callback *) Ocr::train_button_callback, (void *) this );

	// Creo la barra del progresso dell'addestramento
	this->train_progress = new Fl_Progress( 10, 40, 390, 24 );
	this->train_progress->maximum( 1.0 );
	this->train_progress->minimum( 0.0 );
}

Ocr::~Ocr() {

	// Iteratore
	size_t i = 0;

	// Carico le reti neurali artificiali
	for ( ; i < _OCR_CHARACTER_NUM_; i++ ) {

		// Elimino la rete neurale
		delete this->neural_network[i];
	}

	// Nascondo la finestra di addestramento
	this->training_window->hide();
}

void
Ocr::InitOpenGL() {

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

void
Ocr::AddCharacterData() {

	// Ricavo il carattere selezionato
	int choice = this->character_choice->value();

	// Controllo se è stato scelto un carattere e se è stato delimitato il nuovo carattere
	if ( choice >= 0 && this->character_exist ) {

		// Percorso al file di destinazione
		char path[100];

		// Controllo se si vuole aggiungere un carattere
		if ( choice < _OCR_CHARACTER_NUM_ ) {

			// Costruisco il percorso di destinazione
			snprintf( path, 100, "data/character/train/%c.part", 'a' + (char) choice );

			// Log di lavoro
			printf( "Memorizzo la nuova lettera '%c'...\n", 'A' + (char) choice );

		} else {

			// Costruisco il percorso di destinazione
			snprintf( path, 100, "data/character/train/_noise.part" );

			// Log di lavoro
			printf( "Memorizzo il nuovo non-carattere...\n" );
		}

		// Apro un flusso al file di destinazione
		std::ofstream file( path, std::ios_base::out | std::ios_base::app );

		// Imposto la precisione dei numeri con la virgola
		file.setf( std::ios::fixed, std::ios::floatfield );
		file.precision( 4 );

		// Iteratore
		size_t i = 0;

		// Aggiungo un nuovo modello ad un carattere
		for ( ; i < CHARACTER_SIZE; i++ ) {

			file << ( 1.0 - (float) this->character_data[i] / 255.0 ) << " ";
		}

		// Aggiungo un andata a capo
		file << std::endl;

		// Chiudo il flusso al file
		file.close();

		// Nascondo la finestra di addestramento
		this->training_window->hide();
	}
}

void
Ocr::TrainNetwork( int choice ) {

	// Log di lavoro
	printf( "\nStudio la lettera '%c'...\n", 'A' + (char) choice );

	// Apro il file di addestramento
	std::ofstream out( "data/character/train/character.train", std::ios_base::out );

	// Imposto la precisione dei numeri con la virgola
	out.setf( std::ios::fixed, std::ios::floatfield );
	out.precision( 4 );

	// Scrivo l'intestazione del training set
	out << "100 1" << std::endl << std::endl;

	// Percorso della rete
	char path[100] = "data/character/train/#.part";

	// Buffer di lettura
	std::string buffer;

	// Valore di uscita
	float output_value;

	// Iteratore
	size_t i = 0;

	// Carico le reti neurali artificiali
	for ( ; i < _OCR_CHARACTER_NUM_; i++ ) {

		// Preparo il percorso al file della rete neurale
		path[21] = 'a' + (char) i;

		// Apro il file con le informazioni sul carattere
		std::ifstream in( path, std::ios_base::in );

		// Controllo se il file è stato aperto correttamente
		if ( in.is_open() ) {

			// Scrivo l'intestazione nel file di addestramento
			out << "# " << path << std::endl;

			// Imposto il valore di uscita
			output_value = (float) ( path[21] == ( 'a' + (char) choice ) );

			// Leggo tutte le informazioni sul carattere
			while ( !in.eof() ) {

				// Leggo la riga con le informazioni sul carattere ...
				getline( in, buffer );

				// ... e le copio nel file di addestramento con l'uscita desiderata
				if ( !buffer.empty() ) {

					out << buffer << ":" << output_value << std::endl;
				}
			}

			// Aggiungo un separatore nel file di addestramento
			out << std::endl << std::endl << std::endl;

			// Chiudo il file con le informazioni sul carattere
			in.close();
		}
	}


	// Apro il file con le informazioni di disturbo
	std::ifstream in( "data/character/train/_noise.part", std::ios_base::in );

	// Controllo se il file è stato aperto correttamente
	if ( in.is_open() ) {

		// Scrivo l'intestazione nel file di addestramento
		out << "# NON CARATTERI" << std::endl;

		// Leggo tutte le informazioni sul carattere
		while ( !in.eof() ) {

			// Leggo la riga con le informazioni sul disturbo ...
			getline( in, buffer );

			// ... e le copio nel file di addestramento con l'uscita desiderata
			if ( !buffer.empty() ) {

				out << buffer << ": 0.0000" << std::endl;
			}
		}

		// Chiudo il file con le informazioni sul carattere
		in.close();
	}


	// Chiudo il file di addestramento
	out.close();

	// Creo l'addestratore della rete neurale
	Trainer trainer( this->neural_network[choice], false );

	do {

		// Controllo se non si è raggiunto l'errore desiderato
		if ( trainer.GetError() > 0.00001 ) {

			// Log di lavoro
			printf( "Riprovo a studiare la lettera '%c'...\n", 'A' + (char) choice );

			// Reinizializzo i pesi sinaptici prima del nuovo addestramento
			trainer.InitWeights();
		}

		// Addestro la rete neurale
		trainer.SetParameters( 0.5, 1.2 );
		trainer.TrainOnFile< Algorithms::Rprop >( "data/character/train/character.train", 0.00001, 200, 1 );

	// Ciclo finché non raggiungo l'errore desiderato
	} while ( trainer.GetError() > 0.00001 );

	// Costruisco il percorso di destinazione
	snprintf( path, 100, "data/character/%c.net", 'a' + (char) choice );

	// Salva la rete nel file di destinazione
	this->neural_network[choice]->Save( path );
}

void
Ocr::TrainAllNetwork() {

	// Imposto la cancellazione asincrona del thread
	pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );

	// Calcolo il passo della barra del progresso
	float progress_step = 1.0 / (float) _OCR_CHARACTER_NUM_;

	// Prendo il controllo sulle FLTK
	Fl::lock();

	// Azzero la barra del progresso
	this->train_progress->value( 0.0 );

	// Rilascio il controllo sulle FLTK
	Fl::unlock();

	// Buffer di lavoro
	char buffer[100];

	// Iteratore
	size_t i = 0;

	// Addestro tutte le reti neurali
	for ( ; i < _OCR_CHARACTER_NUM_; i++ ) {

		// Preparo il messaggio della barra del progresso
		snprintf( buffer, 100, "Studio la lettera %c ... (%lu di %lu)", 'A' + (char) i, (unsigned long) i + 1, (unsigned long) _OCR_CHARACTER_NUM_ );

		// Prendo il controllo sulle FLTK
		Fl::lock();

		// Aggiorno il messaggio della barra del progresso
		this->train_progress->label( (const char *) buffer );

		// Rilascio il controllo sulle FLTK
		Fl::unlock();

		// Addestro la rete
		this->TrainNetwork( i );

		// Prendo il controllo sulle FLTK
		Fl::lock();

		// Aggiorno la barra del progresso
		this->train_progress->value( this->train_progress->value() + progress_step );

		// Rilascio il controllo sulle FLTK
		Fl::unlock();
	}

	// Memorizzo la fine del thread
	this->thread_id = 0;

	// Prendo il controllo sulle FLTK
	Fl::lock();

	// Aggiorno la barra del progresso
	this->train_progress->label( "" );

	// Modifico l'etichetta del pulsante
	this->train_button->label( "Addestra le reti" );

	// Nascondo la finestra di addestramento
	this->training_window->hide();

	// Rilascio il controllo sulle FLTK
	Fl::unlock();
}

void
Ocr::ClearArtboard() {

	// Iteratore
	size_t i = 0;

	// Pulisco la tavola da disegno
	for ( i = 0; i < ARTBOARD_SIZE; i++ ) {

		this->artboard_data[i] = 255;
	}

	// Annullo l'ultimo riconoscimento
	this->precision_character = 0.0;
}

void
Ocr::ClearCharacter() {

	// Iteratore
	size_t i = 0;

	// Pulisco la tavola da disegno
	for ( i = 0; i < CHARACTER_SIZE; i++ ) {

		this->character_data[i] = 255;
	}

	// Imposto il flag di controllo
	this->character_exist = false;

	// Annullo l'ultimo riconoscimento
	this->precision_character = 0.0;
}

void
Ocr::DrawPencil( int x, int y ) {

	// Numero di pixel del tratto della matita
	size_t size = PENCIL_SIZE * PENCIL_SIZE;

	// Iteratore
	size_t i = 0;

	// Disegno il tratto nella tavola da disegno
	for ( ; i < size; i++ ) {

		// Calcolo la posizione del pixel interessata dalla matita
		int px = x + (int) i % PENCIL_SIZE;
		int py = y + (int) i / PENCIL_SIZE;

		// Controllo se la posizione esiste
		if ( px >= 0 && px < ARTBOARD_W &&
			 py >= 0 && py < ARTBOARD_H ) {

			// Coloro il pixel interessato
			this->artboard_data[ py * ARTBOARD_W + px ] = 0;
		}
	}
}

void
Ocr::FindCharacter() {

	// Ricavo l'area di delimitazione del carattere
	Rectangle &rect = this->char_boundary;

	// Inizializzo l'area di delimitazione del carattere
	rect.left	= ARTBOARD_W - 1;
	rect.top	= ARTBOARD_H - 1;
	rect.right	= 0;
	rect.bottom	= 0;

	// Iteratore
	size_t i = 0;

	// Disegno il tratto nella tavola da disegno
	for ( ; i < ARTBOARD_SIZE; i++ ) {

		// Calcolo la posizione del pixel interessata dalla matita
		int px = i % ARTBOARD_W;
		int py = i / ARTBOARD_W;

		// Controllo se il pixel è colorato
		if ( this->artboard_data[ py * ARTBOARD_W + px ] < 255 ) {

			// Adatto l'area tenendo presente il punto corrente
			if ( px < rect.left )	rect.left	= px;
			if ( px > rect.right )	rect.right	= px;
			if ( py < rect.top ) 	rect.top	= py;
			if ( py > rect.bottom )	rect.bottom	= py;
		}
	}
}

void
Ocr::GetCharacter() {

	// Ricavo l'area di delimitazione del carattere
	Rectangle &rect = this->char_boundary;

	// Allargo l'area di delimitazione del 5%
	rect.left	-= rect.left * 0.05;
	rect.right	+= rect.right * 0.05;
	rect.top	-= rect.left * 0.05;
	rect.bottom	+= rect.bottom * 0.05;

	// Dimensioni dell'area di delimitazione
	int width	= rect.right - rect.left;
	int height	= rect.bottom - rect.top;

	// Controllo se è stato delimitato il carattere
	if ( width > 0 && height > 0 ) {

		// Calcolo il rapporto tra l'area delimitata e il carattere
		float ratio_w = (float) width / (float) CHARACTER_W;
		float ratio_h = (float) height / (float) CHARACTER_H;

		// Calcolo la dimensione dei campioni
		size_t sample_w = (size_t) std::ceil( ratio_w );
		size_t sample_h = (size_t) std::ceil( ratio_h );

		// Numero di pixel per campione
		size_t sample_size = sample_w * sample_h;

		// Numero dei pixel del campione (reali)
		size_t sample_pixels;

		// Valore medio del campione
		size_t sample;

		// Iteratori
		size_t j, i = 0;

		// Eseguo il sottocampionamento del carattere
		for( ; i < CHARACTER_SIZE; i++ ) {

			// Calcolo la posizione del pixel nella tavola da disegno
			int px = rect.left + ( i % CHARACTER_W ) * ratio_w;
			int py = rect.top + ( i / CHARACTER_W ) * ratio_h;

			// Azzero il numero dei pixel del campione
			sample_pixels = 0;

			// Azzero il valore medio del campione
			sample = 0;

			// Calcolo il valore medio del campione
			for( j = 0; j < sample_size; j++ ) {

				// Calcolo la posizione del pixel nel campione
				int sample_px = px + ( j % sample_w );
				int sample_py = py + ( j / sample_h );

				// Controllo se il pixel esiste
				if ( sample_px >= 0 && sample_px < ARTBOARD_W && sample_py >= 0 && sample_py < ARTBOARD_H ) {

					// Aggiungo il valore del pixel alla somma totale del campione
					sample += this->artboard_data[ sample_py * ARTBOARD_W + sample_px ];

					// Incremento il numero dei pixel del campione
					sample_pixels++;
				}
			}

			// Controllo se il campione conteneva almeno un pixel
			if ( sample_pixels ) {

				// Assegno il colore del campione al pixel del carattere
				this->character_data[i] = (uint8_t) ( sample / sample_pixels );

			} else {

				// Assegno il colore del campione al pixel del carattere
				this->character_data[i] = 255u;
			}
		}

		// Imposto il flag di controllo
		this->character_exist = true;
	}
}

void
Ocr::RecognitionCharacter() {

	// Alloco la memoriza necessaria a contenere gli input della rete
	T_Precision *input = new T_Precision[CHARACTER_SIZE];

	// Iteratore
	size_t i = 0;

	// Preparo l'input della rete
	for ( ; i < CHARACTER_SIZE; i++ ) {

		input[i] = ( 1.0 - (T_Precision) this->character_data[i] / 255.0 );
	}

	// Annullo il riconoscimento precedente
	this->precision_character = 0;

	// Precisione del riscontro
	T_Precision new_check, check = 0.50;

	// Analizzo il carattere
	for ( i = 0; i < _OCR_CHARACTER_NUM_; i++ ) {

		// Cerco il riscontro migliore
		if ( ( new_check = this->neural_network[i]->Run( input )[0])  > check ) {

			// Memorizzo il nuovo migliore riscontro
			check = new_check;

			// Memorizzo il carattere identificato
			this->recognized_character = 'A' + i;

			// Memorizzo la precisione del riconoscimento
			this->precision_character = check * 100;
		}
	}

	// Libero la memoria
	delete input;
}

int
Ocr::handle( int event ) {

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

				// Pulisco la tavola da disegno
				case 'c':
				case 'C': {

					// Controllo che sia non in corso l'addestramento
					if ( this->thread_id == 0 ) { 

						// Pulisco la tavola da disegno
						this->ClearArtboard();

						// Pulisco le informazioni sul carattere
						this->ClearCharacter();
					}

					break;
				}

				// Delimito l'area del carattere
				case 'm':
				case 'M': {

					// Mostro la finestra di addestramento
					this->training_window->show();

					break;
				}

				default: break;
			}

			return 1;
		}

		// Pressione e trascinamento di un tasto de mouse
		case FL_PUSH:
		case FL_DRAG: {

			// Controllo che non sia in corso l'addestramento
			if ( this->thread_id == 0 ) { 

				// Ricavo le coordinate all'interno della tavola da disegno
				int pencil_x = ( Fl::event_x() - 20 ) / ARTBOARD_SCALE;
				int pencil_y = ( Fl::event_y() - 20 ) / ARTBOARD_SCALE;

				// Disegno il tratto della matita
				this->DrawPencil( pencil_x, pencil_y );
			}

			return 1;
		}

		// Richiamo la funzione superiore
		default: return Fl_Gl_Window::handle( event );
	}
}

void
Ocr::DrawTexture( size_t width, size_t height, float scale ) {

	// Abilito l'utilizzo delle texture
	glEnable( GL_TEXTURE_2D );

	// Imposto i parametri della texture
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	// Disegno la texture
	glBegin( GL_QUADS );

		glTexCoord2f( 0.0, 1.0f );	glVertex2f( 0.0, (GLfloat) height * (GLfloat) scale );
		glTexCoord2f( 1.0f, 1.0f );	glVertex2f( (GLfloat) width * (GLfloat) scale, (GLfloat) height * (GLfloat) scale );
		glTexCoord2f( 1.0f, 0.0 );	glVertex2f( (GLfloat) width * (GLfloat) scale, 0.0 );
		glTexCoord2f( 0.0, 0.0 );	glVertex2f( 0.0, 0.0 );

	glEnd();

	// Disabilito l'utilizzo delle texture
	glDisable( GL_TEXTURE_2D );
}

void
Ocr::DrawArtboard( size_t x, size_t y ) {

	// Scelgo la texture
	glBindTexture( GL_TEXTURE_2D, this->texture_id );

	// Costruisco la texture usando un immagine in scala di grigio
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_LUMINANCE, ARTBOARD_W, ARTBOARD_H,
					0, GL_LUMINANCE, GL_UNSIGNED_BYTE, this->artboard_data );

	// Memorizzo la matrice corrente
	glPushMatrix();

	// Imposto la posizione della texture
	glTranslatef( (GLfloat) x, (GLfloat) y, 0.0 );

	// Imposto il colore
	glColor3ub( 255u, 255u, 255u );

	// Disegno la texture della luminosità del volto
	this->DrawTexture( ARTBOARD_W, ARTBOARD_H, ARTBOARD_SCALE );

	// Imposto lo spessore della linea
	glLineWidth( 2 );

	// Imposto il colore
	glColor3ub( 80u, 200u, 0u );

	// Disegno il contorno
	glBegin( GL_LINE_LOOP );

		glVertex2f(	0.0, 0.0 );
		glVertex2f(	0.0, (GLfloat) ARTBOARD_H * (GLfloat) ARTBOARD_SCALE );
		glVertex2f(	(GLfloat) ARTBOARD_W * (GLfloat) ARTBOARD_SCALE, (GLfloat) ARTBOARD_H * (GLfloat) ARTBOARD_SCALE );
		glVertex2f(	(GLfloat) ARTBOARD_W * (GLfloat) ARTBOARD_SCALE, 0.0 );

	glEnd();

	// Ripristino la matrice precedente
	glPopMatrix();
}

void
Ocr::DrawCharBoundary( size_t x, size_t y ) {

	// Memorizzo la matrice corrente
	glPushMatrix();

	// Imposto la posizione dell'area
	glTranslatef( (GLfloat) x, (GLfloat) y, 0.0 );

	// Ricavo l'area di delimitazione del carattere
	Rectangle &rect = this->char_boundary;

	// Imposto il colore
	glColor4ub( 255u, 0u, 0u, 20u );

	// Disegno la delimitazione del carattere
	glBegin( GL_QUADS );

		glVertex2f(	(GLfloat) rect.left * (GLfloat) ARTBOARD_SCALE,
					(GLfloat) rect.top * (GLfloat) ARTBOARD_SCALE );
		glVertex2f(	(GLfloat) ( rect.right + 1 ) * (GLfloat) ARTBOARD_SCALE,
					(GLfloat) rect.top * (GLfloat) ARTBOARD_SCALE );
		glVertex2f(	(GLfloat) ( rect.right + 1 ) * (GLfloat) ARTBOARD_SCALE,
					(GLfloat) ( rect.bottom + 1 ) * (GLfloat) ARTBOARD_SCALE );
		glVertex2f(	(GLfloat) rect.left * (GLfloat) ARTBOARD_SCALE,
					(GLfloat) ( rect.bottom + 1 ) * (GLfloat) ARTBOARD_SCALE );

	glEnd();

	// Imposto lo spessore della linea
	glLineWidth( 1 );

	// Imposto il colore
	glColor3ub( 225u, 0u, 0u );

	// Disegno la delimitazione del carattere
	glBegin( GL_LINE_LOOP );

		glVertex2f(	(GLfloat) rect.left * (GLfloat) ARTBOARD_SCALE,
					(GLfloat) rect.top * (GLfloat) ARTBOARD_SCALE );
		glVertex2f(	(GLfloat) ( rect.right + 1 ) * (GLfloat) ARTBOARD_SCALE,
					(GLfloat) rect.top * (GLfloat) ARTBOARD_SCALE );
		glVertex2f(	(GLfloat) ( rect.right + 1 ) * (GLfloat) ARTBOARD_SCALE,
					(GLfloat) ( rect.bottom + 1 ) * (GLfloat) ARTBOARD_SCALE );
		glVertex2f(	(GLfloat) rect.left * (GLfloat) ARTBOARD_SCALE,
					(GLfloat) ( rect.bottom + 1 ) * (GLfloat) ARTBOARD_SCALE );

	glEnd();

	// Ripristino la matrice precedente
	glPopMatrix();
}

void
Ocr::DrawCharacter( size_t x, size_t y ) {

	// Scelgo la texture
	glBindTexture( GL_TEXTURE_2D, this->texture_id );

	// Costruisco la texture usando un immagine in scala di grigio
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_LUMINANCE, CHARACTER_W, CHARACTER_H,
					0, GL_LUMINANCE, GL_UNSIGNED_BYTE, this->character_data );

	// Memorizzo la matrice corrente
	glPushMatrix();

	// Imposto la posizione della texture
	glTranslatef( (GLfloat) x, (GLfloat) y, 0.0 );

	// Imposto il colore
	glColor3ub( 255u, 255u, 255u );

	// Disegno la texture della luminosità del volto
	this->DrawTexture( CHARACTER_W, CHARACTER_H, CHARACTER_SCALE );

	// Imposto lo spessore della linea
	glLineWidth( 2 );

	// Imposto il colore
	glColor3ub( 80u, 200u, 0u );

	// Disegno il contorno
	glBegin( GL_LINE_LOOP );

		glVertex2f(	0.0, 0.0 );
		glVertex2f(	0.0, (GLfloat) CHARACTER_H * (GLfloat) CHARACTER_SCALE );
		glVertex2f(	(GLfloat) CHARACTER_W * (GLfloat) CHARACTER_SCALE, (GLfloat) CHARACTER_H * (GLfloat) CHARACTER_SCALE );
		glVertex2f(	(GLfloat) CHARACTER_W * (GLfloat) CHARACTER_SCALE, 0.0 );

	glEnd();

	// Ripristino la matrice precedente
	glPopMatrix();
}

void
Ocr::draw() {

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

	// Controllo che non sia in corso l'addestramento
	if ( this->thread_id == 0 ) { 

		// Delimito l'area del carattere
		this->FindCharacter();

		// Estraggo il carattere dalla tavola da disegno
		this->GetCharacter();

		// Controllo se il carattere è già stato delimitato
		if ( this->character_exist ) {

			// Cerco il carattere con le reti neurali
			this->RecognitionCharacter();
		}
	}

	// Disegno la tavola da disegno
	this->DrawArtboard( 20, 20 );

	// Controllo se il carattere è già stato delimitato
	if ( this->character_exist ) {

		// Disegno l'area di delimitazione del carattere
		this->DrawCharBoundary( 20, 20 );
	}

	// Disegno il carattere
	this->DrawCharacter( 20 + ARTBOARD_W * ARTBOARD_SCALE + 20, 20 );

	// Controllo se il carattere è stato identificato
	if ( this->precision_character > 0.0 ) {

		// Imposto il colore
		glColor3ub( 255u, 0u, 0u );

		// Costruisco il messaggio di successo
		char buffer[50];
		snprintf( buffer, 50, "Carattere: '%c' (%.1f%%)\n", this->recognized_character, this->precision_character );

		// Disegno il messaggio
		gl_draw( buffer, 45, 45 ); 
	}

	// Imposto il colore
	glColor3ub( 255u, 255u, 0u );

	// Disegno alcuni messaggi per l'utente
	gl_draw( "[M] Menu'", 20, 20 + ARTBOARD_H * ARTBOARD_SCALE + 25 );
	gl_draw( "[C] Pulisci", 130, 20 + ARTBOARD_H * ARTBOARD_SCALE + 25 ); 

	// Richiamo la funzione originale
	Fl_Gl_Window::draw();
}

/* INIZIO FUNZIONE PRINCIPALE */

int
main( void ) {

	// Creo la finestra principale
	Ocr ocr_window( 430, 360 );

	//ocr_window.resizable( ocr_window );
	ocr_window.show();

	// Abilito il multithreading con le FLTK
	Fl::lock();

	// Richiamo il loop principale
	return Fl::run();
}

/* FINE FUNZIONE PRINCIPALE */

#endif
