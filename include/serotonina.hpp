/* 
    Title --- serotonina.hpp

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

#ifndef _SEROTONINA_HPP_
#define _SEROTONINA_HPP_

/*!
 * @namespace Serotonina
 * \if english
 *
 *   \mainpage Serotonina v1.1.6 - Reference Manual
 *
 *   \section description Description
 *
 *   Serotonina is a free library for creation and management of artificial neural networks. It is a object-oriented library, it is written in c++ language and it was designed for easy and efficient management of any feedforward neural network, with several methods of supervisioned training implemented as modules.
 *
 *   \section features Features
 *
 *   The library is released under terms of the <a href="http://en.wikipedia.org/wiki/GNU_Lesser_General_Public_License" target="_blank">GNU LGPL v3</a> and it has no dependencies, but some of his demos uses <a href="http://www.fltk.org/" target="_blank">FLTK</a> and <a href="http://opencv.willowgarage.com/" target="_blank">OpenCV</a> libraries.
 *
 *   The goals of the library are:
 *
 *       - <b>speed</b>, to manage efficiently and quickly any type of artificial neural network;
 *       - <b>modularity</b>, to allow users to extend the library's functions with custom modules;
 *       - <b>portability</b>, especially on <i>GNU/Linux</i> and <i>MS Windows</i> platforms;
 *       - <b>elegance of the code</b>, in a clear and essential way, but first of all efficiently.
 *
 *   The current technical features are:
 *
 *       - Multilayer perceptron network (<a href="http://en.wikipedia.org/wiki/Multilayer_perceptron" target="_blank">MPL</a>)
 *       - Sigmoid transfer function
 *       - Several supervised learning algorithms (<i>Backpropagation</i>, <i>Rprop</i>, <i>IRprop</i>, etc)
 *       - Support for custom training algorithms
 *       - Support for reading training set from a file
 *       - Support for loading and saving trained network
 *       - English and Italian documentation, with several examples
 *
 *   \section documentation Documentation
 *
 *   The library began as a school-leaving project and it is related to a thesis:
 *
 *       - <a href="http://www.hackyourmind.org/downloads/documenti/Le%20reti%20neurali%20artificiali%20-%20tesi%20di%20maturita.pdf" target="_blank">Le reti neurali artificiali - tesi di maturita.pdf</a>
 *
 *   The reference manual is available here:
 *
 *       - <a href="http://www.serotonina.hackyourmind.org/doc/en/">Reference manual (english)</a>
 *       - <a href="http://www.serotonina.hackyourmind.org/doc/it/">Manuale di riferimento (italiano)</a>
 *
 *   \section download Download
 *
 *   The source of the latest version of Serotonina is available on <a href="https://github.com/Wicker25/Serotonina-Neural-Network-Library" target="_blank">github.com</a> at:
 *
 *       - <a href="https://github.com/Wicker25/Serotonina-Neural-Network-Library" target="_blank">https://github.com/Wicker25/Serotonina-Neural-Network-Library</a>
 * 
 *   \section links Links
 *
 *      - <a href="https://github.com/Wicker25/Serotonina-Neural-Network-Library" target="_blank">Serotonina on Github.com</a>
 *      - <a href="https://www.ohloh.net/p/serotonina_neural_network_library" target="_blank">Serotonina on Ohloh.net</a>
 *
 * \endif
 * \if italian
 *
 *   \mainpage Serotonina v1.1.6 - Manuale di riferimento
 *
 *   \section description Descrizione
 *
 *   Serotonina è una libreria libera per la creazione e la gestione di reti neurali artificiali. E’ una libreria orientata agli oggetti, scritta in linguaggio C++ ed è stata pensata per poter gestire con semplicità ed efficienza qualsiasi tipo di rete neurale feedforward, attraverso vari metodi di apprendimento supervisionato implementati sotto forma di moduli.
 *
 *   \section features Caratteristiche
 *
 *   La libreria è rilasciata sotto licenza <a href="http://it.wikipedia.org/wiki/GNU_Lesser_General_Public_License" target="_blank">GNU LGPL v3</a> e non necessita di dipendenze particolari, tuttavia alcuni dei suoi demo si appoggiano alle librerie <a href="http://www.fltk.org/" target="_blank">FLTK</a> e <a href="http://opencv.willowgarage.com/" target="_blank">OpenCV</a>.
 *
 *   I tratti distintivi, nonché gli obiettivi che essa si pone di raggiungere, sono:
 *
 *       - la <b>velocità</b>, per poter gestire con efficienza e rapidità qualsiasi tipo di rete neurale artificiale;
 *       - la <b>modularità</b>, per consentire agli utilizzatori di estendere le funzionalità della libreria attraverso la creazione di moduli personalizzati;
 *       - la <b>portabilità</b>, principalmente sui sistemi <i>GNU/Linux</i> e <i>MS Windows</i>;
 *       - l’<b>eleganza del codice</b>, secondo le regole della chiarezza e dell’essenzialità, ponendo sempre al primo posto l’efficienza.
 *
 *   Le caratteristiche tecniche attualmente implementate sono le seguenti:
 *
 *       - Reti di percettroni multistrato (<a href="http://en.wikipedia.org/wiki/Multilayer_perceptron" target="_blank">MPL</a>)
 *       - Funzione di trasferimento sigmoidale
 *       - Diversi algoritmi di apprendimento supervisionato (<i>Backpropagation</i>, <i>Rprop</i>, <i>IRprop</i>, etc)
 *       - Possibilità di creare nuovi algoritmi di addestramento personalizzati
 *       - Lettura dell’insieme di addestramento da un file
 *       - Possibilità di salvare e caricare le reti neurali addestrate
 *       - Documentazione in italiano e in inglese, correlata da esempi
 *
 *   \section documentation Documentazione
 *
 *   La libreria nasce come un progetto di maturità ed è correlata da una tesi:
 *
 *       - <a href="http://www.hackyourmind.org/downloads/documenti/Le%20reti%20neurali%20artificiali%20-%20tesi%20di%20maturita.pdf" target="_blank">Le reti neurali artificiali - tesi di maturita.pdf</a>
 *
 *   Il manuale di riferimento, invece, è disponibile (in lingua italiana e inglese) ai seguenti indirizzi:
 *
 *       - <a href="http://www.serotonina.hackyourmind.org/doc/it/">Manuale di riferimento (italiano)</a>
 *       - <a href="http://www.serotonina.hackyourmind.org/doc/en/">Reference manual (english)</a>
 *
 *   \section download Download
 *
 *   I sorgenti dell’ultima versione di Serotonina sono disponibili su <a href="https://github.com/Wicker25/Serotonina-Neural-Network-Library" target="_blank">github.com</a> al seguente indirizzo:
 *
 *       - <a href="https://github.com/Wicker25/Serotonina-Neural-Network-Library" target="_blank">https://github.com/Wicker25/Serotonina-Neural-Network-Library</a>
 *
 *   \section links Links
 *
 *      - <a href="https://github.com/Wicker25/Serotonina-Neural-Network-Library" target="_blank">Serotonina su Github.com</a>
 *      - <a href="https://www.ohloh.net/p/serotonina_neural_network_library" target="_blank">Serotonina su Ohloh.net</a>
 *
 * \endif
 */

/*!
 * @namespace Serotonina
 * \if english
 *   @brief Main namespace of the library.
 * \endif
 * \if italian
 *   @brief Namespace principale della libreria
 * \endif
 */

// Includo tutti gli headers della libreria

#include <serotonina/version.hpp>
#include <serotonina/types.hpp>
#include <serotonina/macros.hpp>
#include <serotonina/exception.hpp>
#include <serotonina/string.hpp>

#include <serotonina/neuron.hpp>
#include <serotonina/synapse.hpp>
#include <serotonina/layer.hpp>

#include <serotonina/network.hpp>
#include <serotonina/network-inl.hpp>

#include <serotonina/trainer.hpp>
#include <serotonina/trainer-inl.hpp>

#include <serotonina/train-algorithm.hpp>

// Includo i moduli per l'addestramento

#include <serotonina/mod/batch.hpp>
#include <serotonina/mod/rprop.hpp>
#include <serotonina/mod/rpropp.hpp>
#include <serotonina/mod/rpropm.hpp>
#include <serotonina/mod/irpropp.hpp>
#include <serotonina/mod/irpropm.hpp>

#endif
