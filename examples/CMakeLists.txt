# CMake's configuration

# Vincolo sulle versioni minori di CMake
cmake_minimum_required( VERSION 2.6 )

# Aggiungo una direttiva al compilatore
add_definitions( -Wall -ansi -pedantic -pedantic-errors )

# Imposto le dipendenze necessarie per gli esempi e gli strumenti
find_package( Serotonina )

# Controllo se è stata trovata la libreria di Serotonina
IF ( SEROTONINA_FOUND )

	# Compilo tutti gli esempi
	add_executable( create_network ${CMAKE_SOURCE_DIR}/create_network.cpp )
	target_link_libraries( create_network ${SEROTONINA_LIBRARIES} )

	add_executable( train_network ${CMAKE_SOURCE_DIR}/train_network.cpp )
	target_link_libraries( train_network ${SEROTONINA_LIBRARIES} )

	add_executable( load_from_file ${CMAKE_SOURCE_DIR}/load_from_file.cpp )
	target_link_libraries( load_from_file ${SEROTONINA_LIBRARIES} )

	add_executable( create_train_algorithm ${CMAKE_SOURCE_DIR}/create_train_algorithm.cpp )
	target_link_libraries( create_train_algorithm ${SEROTONINA_LIBRARIES} )

	add_executable( and ${CMAKE_SOURCE_DIR}/and.cpp )
	target_link_libraries( and ${SEROTONINA_LIBRARIES} )

	add_executable( xor ${CMAKE_SOURCE_DIR}/xor.cpp )
	target_link_libraries( xor ${SEROTONINA_LIBRARIES} )

	add_executable( coder ${CMAKE_SOURCE_DIR}/coder.cpp )
	target_link_libraries( coder ${SEROTONINA_LIBRARIES} )

	add_executable( face ${CMAKE_SOURCE_DIR}/face.cpp )
	target_link_libraries( face ${SEROTONINA_LIBRARIES} )

ELSE ( SEROTONINA_FOUND )

	# Log di lavoro
	MESSAGE( FATAL_ERROR "-- Could NOT find Serotonina!" )

ENDIF ( SEROTONINA_FOUND )

