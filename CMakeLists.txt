# CMake's configuration

# Nome del progetto e linguaggio utilizzato
project( Serotonina )

# Vincolo sulle versioni minori di CMake
cmake_minimum_required( VERSION 2.6 )

# Imposto le versioni degli strumenti
set( Serotonina_VERSION_MAJOR 1 )
set( Serotonina_VERSION_MINOR 1 )
set( Serotonina_VERSION_PATCH 1 )

set( Serotonina_PLOT_VERSION_MAJOR 1 )
set( Serotonina_PLOT_VERSION_MINOR 0 )
set( Serotonina_PLOT_VERSION_PATCH 0 )

set( Serotonina_GYM_VERSION_MAJOR 1 )
set( Serotonina_GYM_VERSION_MINOR 0 )
set( Serotonina_GYM_VERSION_PATCH 1 )

set( Serotonina_OCR_VERSION_MAJOR 1 )
set( Serotonina_OCR_VERSION_MINOR 0 )
set( Serotonina_OCR_VERSION_PATCH 1 )

set( Serotonina_FACE_RECOGNITION_VERSION_MAJOR 1 )
set( Serotonina_FACE_RECOGNITION_VERSION_MINOR 0 )
set( Serotonina_FACE_RECOGNITION_VERSION_PATCH 1 )

# Aggiungo una direttiva al compilatore
add_definitions( -Wall -ansi -pedantic -pedantic-errors )

# Imposto le dipendenze necessarie
find_package( FLTK )
find_package( OpenGL )
find_package( OpenCV )
#find_package( OpenCV COMPONENTS highgui )

# Percorso degli headers
include_directories(

	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_BINARY_DIR}/include"
)

# Imposto i file di configurazione generati automaticamente
configure_file(

	"${CMAKE_SOURCE_DIR}/include/serotonina/version.hpp.in"
	"${CMAKE_BINARY_DIR}/include/serotonina/version.hpp"
)

configure_file(

	"${CMAKE_SOURCE_DIR}/include/extra/version.hpp.in"
	"${CMAKE_BINARY_DIR}/include/extra/version.hpp"
)

# Imposto lo script per la disinstallazione
configure_file(

	"${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY
)

# Imposto la cartella delle librerie generate
set( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib )
# Imposto la cartella dei binari generati
#set( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin )

# Modifico i percorsi dei sorgenti e dei binari
set( Serotonina_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src )
set( Serotonina_BINARY_DIR ${CMAKE_BINARY_DIR}/bin )

# Libreria Serotonina
add_library( serotonina SHARED	${Serotonina_SOURCE_DIR}/network.cpp
								${Serotonina_SOURCE_DIR}/trainer.cpp
								${Serotonina_SOURCE_DIR}/mod/batch.cpp
								${Serotonina_SOURCE_DIR}/mod/rprop.cpp
								${Serotonina_SOURCE_DIR}/mod/rpropp.cpp
								${Serotonina_SOURCE_DIR}/mod/rpropm.cpp
								${Serotonina_SOURCE_DIR}/mod/irpropp.cpp
								${Serotonina_SOURCE_DIR}/mod/irpropm.cpp )

target_link_libraries( serotonina m )

# Controllo se sono state trovate le FLTK
IF ( FLTK_FOUND )

	# Libreria per i grafici di Serotonina
	add_library( serotonina_plot STATIC ${Serotonina_SOURCE_DIR}/extra/plot.cpp )

	# Strumento Gym per l'addestramento delle reti neurali
	add_executable( st_gym ${Serotonina_SOURCE_DIR}/extra/gym.cpp )

	add_dependencies( st_gym serotonina )
	add_dependencies( st_gym serotonina_plot )

	target_link_libraries( st_gym serotonina )
	target_link_libraries( st_gym serotonina_plot )
	target_link_libraries( st_gym pthread )
	target_link_libraries( st_gym ${FLTK_LIBRARIES} )
	target_link_libraries( st_gym ${OPENGL_LIBRARIES} )

	# Strumento OCR per il riconoscimento di caratteri manoscritti
	add_executable( st_ocr ${Serotonina_SOURCE_DIR}/extra/ocr.cpp )

	add_dependencies( st_ocr serotonina )

	target_link_libraries( st_ocr serotonina )
	target_link_libraries( st_ocr pthread )
	target_link_libraries( st_ocr ${FLTK_LIBRARIES} )
	target_link_libraries( st_ocr ${OPENGL_LIBRARIES} )

	# Controllo se sono state trovate le OpenCV
	IF ( OpenCV_FOUND )

		# Strumento Face recognition per il riconoscimento di un volto attraverso la webcam
		add_executable( st_face_recognition ${Serotonina_SOURCE_DIR}/extra/face-recognition.cpp )

		add_dependencies( st_face_recognition serotonina )

		target_link_libraries( st_face_recognition serotonina )
		target_link_libraries( st_face_recognition ${FLTK_LIBRARIES} )
		target_link_libraries( st_face_recognition ${OPENGL_LIBRARIES} )
		target_link_libraries( st_face_recognition ${OpenCV_LIBS} )

	ELSE ( OpenCV_FOUND )

		# Log di lavoro
		MESSAGE( STATUS "WARNING: could NOT find dependencies of Face Recognition tool." )

	ENDIF ( OpenCV_FOUND )

ELSE ( FLTK_FOUND )

	MESSAGE( STATUS "WARNING: Could NOT find dependencies of tools." )

ENDIF ( FLTK_FOUND )


# Installatore della libreria
install_targets( /lib serotonina )

# Installatore degli headers
install( FILES ${CMAKE_SOURCE_DIR}/include/serotonina.hpp
		 DESTINATION ${CMAKE_INSTALL_PREFIX}/include )

install( DIRECTORY ${CMAKE_SOURCE_DIR}/include/serotonina
		 DESTINATION ${CMAKE_INSTALL_PREFIX}/include
		 FILES_MATCHING
		 PATTERN "*"
		 PATTERN "*.in" EXCLUDE )

# Installatore delle informazioni per pkg-config
install( FILES ${CMAKE_SOURCE_DIR}/pkgconfig/serotonina.pc
		 DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig )

# Installatore delle informazioni per CMake
install( FILES ${CMAKE_SOURCE_DIR}/cmake/FindSerotonina.cmake
		 DESTINATION ${CMAKE_ROOT}/Modules/ )

# Memorizzo le cartelle di destinazione
file( WRITE "${CMAKE_SOURCE_DIR}/install_directory.txt"

	"${CMAKE_INSTALL_PREFIX}/include/serotonina\n"
)

# Aggiungo il "make uninstall"
add_custom_target( uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake )

