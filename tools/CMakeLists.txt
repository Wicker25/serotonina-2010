# CMake's configuration

# Vincolo sulle versioni minori di CMake
cmake_minimum_required( VERSION 2.6 )

# Aggiungo una direttiva al compilatore
add_definitions( -Wall -ansi -pedantic -pedantic-errors )

# Imposto le dipendenze necessarie per gli esempi e gli strumenti
find_package( Serotonina )
find_package( FLTK )
find_package( OpenGL )
find_package( Threads )

# Imposto le versioni degli strumenti
set( SEROTONINA_PLOT_VERSION_MAJOR 1 )
set( SEROTONINA_PLOT_VERSION_MINOR 0 )
set( SEROTONINA_PLOT_VERSION_PATCH 0 )

set( SEROTONINA_GYM_VERSION_MAJOR 1 )
set( SEROTONINA_GYM_VERSION_MINOR 0 )
set( SEROTONINA_GYM_VERSION_PATCH 1 )

set( SEROTONINA_OCR_VERSION_MAJOR 1 )
set( SEROTONINA_OCR_VERSION_MINOR 0 )
set( SEROTONINA_OCR_VERSION_PATCH 1 )

# Percorso degli headers
include_directories(

	${CMAKE_SOURCE_DIR}/include
	${CMAKE_BINARY_DIR}/include
	${SEROTONINA_INCLUDE_DIR}
	${FLTK_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	${OpenCV_INCLUDE_DIRS}
)

# Imposto i file di configurazione generati automaticamente
configure_file(

	"${CMAKE_SOURCE_DIR}/include/version.hpp.in"
	"${CMAKE_BINARY_DIR}/include/version.hpp"
)

# Imposto la cartella delle librerie generate
set( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib )

# Controllo se è stata trovata la libreria Serotonina
IF ( SEROTONINA_FOUND )

	# Controllo se il sistema operativo è UNIX
	IF ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )

		# Controllo se sono state trovate le FLTK
		IF ( FLTK_FOUND )

			# Libreria per i grafici di Serotonina
			add_library( serotonina_plot STATIC ${CMAKE_SOURCE_DIR}/src/plot.cpp )

			target_link_libraries( serotonina_plot ${FLTK_LIBRARIES} )
			target_link_libraries( serotonina_plot ${OPENGL_LIBRARIES} )

			# Strumento Gym per l'addestramento delle reti neurali
			add_executable( st_gym ${CMAKE_SOURCE_DIR}/src/gym.cpp )

			target_link_libraries( st_gym ${SEROTONINA_LIBRARIES} )
			target_link_libraries( st_gym serotonina_plot )
			target_link_libraries( st_gym ${CMAKE_THREAD_LIBS_INIT} )
			target_link_libraries( st_gym ${FLTK_LIBRARIES} )
			target_link_libraries( st_gym ${OPENGL_LIBRARIES} )

			# Strumento OCR per il riconoscimento di caratteri manoscritti
			add_executable( st_ocr ${CMAKE_SOURCE_DIR}/src/ocr.cpp )

			target_link_libraries( st_ocr ${SEROTONINA_LIBRARIES} )
			target_link_libraries( st_ocr ${CMAKE_THREAD_LIBS_INIT} )
			target_link_libraries( st_ocr ${FLTK_LIBRARIES} )
			target_link_libraries( st_ocr ${OPENGL_LIBRARIES} )

		ELSE ( FLTK_FOUND )

			MESSAGE( STATUS "WARNING: Could NOT find dependencies of tools." )

		ENDIF ( FLTK_FOUND )

	ENDIF ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )

ELSE ( SEROTONINA_FOUND )

	# Log di lavoro
	MESSAGE( FATAL_ERROR "-- Could NOT find Serotonina!" )

ENDIF ( SEROTONINA_FOUND )

