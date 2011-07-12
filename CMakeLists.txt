# CMake's configuration

# Nome del progetto e linguaggio utilizzato
project( Serotonina )

# Vincolo sulle versioni minori di CMake
cmake_minimum_required( VERSION 2.6 )

# Imposto la versione della libreria
set( SEROTONINA_VERSION_MAJOR 1 )
set( SEROTONINA_VERSION_MINOR 1 )
set( SEROTONINA_VERSION_PATCH 6 )

# Aggiungo una direttiva al compilatore
add_definitions( -Wall -ansi -pedantic -pedantic-errors )

# Percorso degli headers
include_directories(

	${CMAKE_SOURCE_DIR}/include
	${CMAKE_BINARY_DIR}/include
)

# Imposto i file di configurazione generati automaticamente
configure_file(

	"${CMAKE_SOURCE_DIR}/include/serotonina/version.hpp.in"
	"${CMAKE_BINARY_DIR}/include/serotonina/version.hpp"
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
add_library( serotonina SHARED	${SEROTONINA_SOURCE_DIR}/network.cpp
								${SEROTONINA_SOURCE_DIR}/trainer.cpp
								${SEROTONINA_SOURCE_DIR}/mod/batch.cpp
								${SEROTONINA_SOURCE_DIR}/mod/rprop.cpp
								${SEROTONINA_SOURCE_DIR}/mod/rpropp.cpp
								${SEROTONINA_SOURCE_DIR}/mod/rpropm.cpp
								${SEROTONINA_SOURCE_DIR}/mod/irpropp.cpp
								${SEROTONINA_SOURCE_DIR}/mod/irpropm.cpp )

target_link_libraries( serotonina m )

# Cerco doxygen nel sistema
find_package( Doxygen )

# Controllo se doxygen è installato sul sistema
if ( DOXYGEN_FOUND )

	# Aggiungo i comandi per la creazione della documentazione
	add_custom_target( doc_en COMMAND doxygen doxy_en.conf )
	add_custom_target( doc_it COMMAND doxygen doxy_it.conf )

endif ( DOXYGEN_FOUND )

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

# Installatore della documentazione
install( DIRECTORY ${CMAKE_SOURCE_DIR}/doc/
		 DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/serotonina
		 FILES_MATCHING
		 PATTERN "*" )

# Installatore delle informazioni per pkg-config
install( FILES ${CMAKE_SOURCE_DIR}/pkgconfig/serotonina.pc
		 DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig )

# Installatore delle informazioni per CMake
install( FILES ${CMAKE_SOURCE_DIR}/cmake/FindSerotonina.cmake
		 DESTINATION ${CMAKE_ROOT}/Modules/ )

# Memorizzo le cartelle di destinazione
file( WRITE "${CMAKE_SOURCE_DIR}/install_directory.txt"

	"${CMAKE_INSTALL_PREFIX}/include/serotonina\n"
	"${CMAKE_INSTALL_PREFIX}/share/doc/serotonina\n"
)

# Aggiungo il "make uninstall"
add_custom_target( uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake )

