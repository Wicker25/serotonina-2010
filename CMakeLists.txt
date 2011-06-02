# CMake's configuration

# Nome del progetto e linguaggio utilizzato
project( SEROTONINA )

# Vincolo sulle versioni minori di CMake
cmake_minimum_required( VERSION 2.6 )

# Imposto le versioni degli strumenti
set( SEROTONINA_VERSION_MAJOR 1 )
set( SEROTONINA_VERSION_MINOR 0 )
set( SEROTONINA_VERSION_PATCH 2 )

set( SEROTONINA_PLOT_VERSION_MAJOR 1 )
set( SEROTONINA_PLOT_VERSION_MINOR 0 )
set( SEROTONINA_PLOT_VERSION_PATCH 0 )

set( SEROTONINA_GYM_VERSION_MAJOR 1 )
set( SEROTONINA_GYM_VERSION_MINOR 0 )
set( SEROTONINA_GYM_VERSION_PATCH 0 )

set( SEROTONINA_OCR_VERSION_MAJOR 1 )
set( SEROTONINA_OCR_VERSION_MINOR 0 )
set( SEROTONINA_OCR_VERSION_PATCH 0 )

set( SEROTONINA_FACE_RECOGNITION_VERSION_MAJOR 1 )
set( SEROTONINA_FACE_RECOGNITION_VERSION_MINOR 0 )
set( SEROTONINA_FACE_RECOGNITION_VERSION_PATCH 0 )

# Aggiungo una direttiva al compilatore
add_definitions( -Wall -ansi )
# -pedantic -pedantic-errors

# Imposto le dipendenze necessarie
find_package( FLTK REQUIRED )
find_package( OpenGL REQUIRED )
find_package( OpenCV REQUIRED )

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
set( SEROTONINA_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src )
set( SEROTONINA_BINARY_DIR ${CMAKE_BINARY_DIR}/bin )

# Libreria Serotonina
add_library( serotonina SHARED ${SEROTONINA_SOURCE_DIR}/network.cpp ${SEROTONINA_SOURCE_DIR}/network-train.cpp )
target_link_libraries( serotonina m )

# Libreria per i grafici di Serotonina
add_library( serotonina_plot STATIC ${SEROTONINA_SOURCE_DIR}/extra/plot.cpp )

# Strumento Gym per l'addestramento delle reti neurali
add_executable( st_gym ${SEROTONINA_SOURCE_DIR}/extra/gym.cpp )

add_dependencies( st_gym serotonina )
add_dependencies( st_gym serotonina_plot )

target_link_libraries( st_gym serotonina )
target_link_libraries( st_gym serotonina_plot )
target_link_libraries( st_gym pthread )
target_link_libraries( st_gym ${FLTK_LIBRARIES} )
target_link_libraries( st_gym ${OPENGL_LIBRARIES} )

# Strumento OCR per il riconoscimento di caratteri manoscritti
add_executable( st_ocr ${SEROTONINA_SOURCE_DIR}/extra/ocr.cpp )

add_dependencies( st_ocr serotonina )

target_link_libraries( st_ocr serotonina )
target_link_libraries( st_ocr pthread )
target_link_libraries( st_ocr ${FLTK_LIBRARIES} )
target_link_libraries( st_ocr ${OPENGL_LIBRARIES} )

# Strumento Face recognition per il riconoscimento di un volto attraverso la webcam
add_executable( st_face_recognition ${SEROTONINA_SOURCE_DIR}/extra/face-recognition.cpp )

add_dependencies( st_face_recognition serotonina )

target_link_libraries( st_face_recognition serotonina )
target_link_libraries( st_face_recognition ${FLTK_LIBRARIES} )
target_link_libraries( st_face_recognition ${OPENGL_LIBRARIES} )
target_link_libraries( st_face_recognition ${OpenCV_LIBS} )

# Installatore della libreria
install_targets( /lib serotonina )

# Installatore degli headers
install( DIRECTORY ${CMAKE_SOURCE_DIR}/include/serotonina
		 DESTINATION ${CMAKE_INSTALL_PREFIX}/include
		 FILES_MATCHING
		 PATTERN "*"
		 PATTERN "*.in" EXCLUDE )

# Installatore delle informazioni per pkg-config
install( FILES ${CMAKE_SOURCE_DIR}/pkgconfig/serotonina.pc
		 DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig )

# Memorizzo le cartelle di destinazione
file( WRITE "${CMAKE_SOURCE_DIR}/install_directory.txt"

	"${CMAKE_INSTALL_PREFIX}/include/serotonina\n"
)

# Aggiungo il "make uninstall"
add_custom_target( uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake )

