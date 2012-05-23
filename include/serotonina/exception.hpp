/* 
    Title --- exception.hpp

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
    along with Serotonina. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SEROTONINA_EXCEPTION_HPP_
#define _SEROTONINA_EXCEPTION_HPP_

#include <iostream>
#include <stdexcept>

namespace serotonina { // Namespace di Serotonina

/*!
 * @class Exception
 * \if english
 *   @brief Exception's class
 * \endif
 * \if italian
 *   @brief Classe di un eccezione
 * \endif
 */

class Exception : public std::runtime_error {

	/* ECCEZIONE */

public:

	/**
	 * \if english
	 *   @brief Constructor method
	 *   @param[in] msg Error message
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore
	 *   @param[in] msg Messaggio di errore
	 * \endif
	 */

	// Metodo costruttore
	Exception( const std::string &msg = "" ) : std::runtime_error( msg ) { }

	/**
	 * \if english
	 *   @brief Constructor method
	 *   @param[in] msg0 First part of error message
	 *   @param[in] msg1 Second part of error message
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore
	 *   @param[in] msg0 Prima parte del messaggio di errore
	 *   @param[in] msg1 Seconda parte del messaggio di errore
	 * \endif
	 */
	// Metodo costruttore
	Exception(	const std::string &msg0,
				const std::string &msg1 ) : std::runtime_error( msg0 + msg1 ) { }

	/**
	 * \if english
	 *   @brief Constructor method
	 *   @param[in] msg0 First part of error message
	 *   @param[in] msg1 Second part of error message
 	 *   @param[in] msg2 Third part of error message
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore
	 *   @param[in] msg0 Prima parte del messaggio di errore
	 *   @param[in] msg1 Seconda parte del messaggio di errore
	 *   @param[in] msg2 Terza parte del messaggio di errore
	 * \endif
	 */

	// Metodo costruttore
	Exception(	const std::string &msg0,
				const std::string &msg1,
				const std::string &msg2 ) : std::runtime_error( msg0 + msg1 + msg2 ) { }

	/**
	 * \if english
	 *   @brief Constructor method
	 *   @param[in] msg0 First part of error message
	 *   @param[in] msg1 Second part of error message
 	 *   @param[in] msg2 Third part of error message
	 *   @param[in] msg3 Fourth part of error message
	 * \endif
	 * \if italian
	 *   @brief Metodo costruttore
	 *   @param[in] msg0 Prima parte del messaggio di errore
	 *   @param[in] msg1 Seconda parte del messaggio di errore
	 *   @param[in] msg2 Terza parte del messaggio di errore
	 *   @param[in] msg3 Quarta parte del messaggio di errore
	 * \endif
	 */

	// Metodo costruttore
	Exception(	const std::string &msg0,
				const std::string &msg1,
				const std::string &msg2,
				const std::string &msg3 ) : std::runtime_error( msg0 + msg1 + msg2 + msg3 ) { }

};

} // Chiudo il namespace di Serotonina

#endif
