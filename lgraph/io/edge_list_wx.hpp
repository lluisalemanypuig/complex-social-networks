/*********************************************************************
 * lgraph
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#pragma once

// lgraph includes
#include <lgraph/data_structures/uxgraph.hpp>
#include <lgraph/data_structures/wxgraph.hpp>

namespace lgraph {
namespace io {

namespace edge_list {

// --------------
// -- WEIGHTED --

/**
 * @brief Reads a weighted graph in edge list format.
 * @param filename The name of the file to be read.
 * @param[out] g The graph built with the contents in the file.
 * @return Returns false if the file coule not be opened.
 * Returns true otherwise.
 */
template<class T>
bool read(const std::string& filename, wxgraph<T> *g);
/**
 * @brief Reads a weighted graph in edge list format.
 * @param filename The name of the file to be read.
 * @param[out] g The graph built with the contents in the file.
 * @return Returns false if the file coule not be opened.
 * Returns true otherwise.
 */
template<class T>
bool read(const char *filename, wxgraph<T> *g);

/**
 * @brief Writes a weighted graph in edge list format.
 * @param filename The name of the file where the graph will be
 * written to.
 * @param g The graph to store.
 * @return Returns false if the file could not be opened.
 * Returns true otherwise.
 */
template<class T>
bool write(const std::string& filename, const wxgraph<T> *g);
/**
 * @brief Writes a weighted graph in edge list format.
 * @param filename The name of the file where the graph will be
 * written to.
 * @param g The graph to store.
 * @return Returns false if the file could not be opened.
 * Returns true otherwise.
 */
template<class T>
bool write(const char *filename, const wxgraph<T> *g);

} // -- namespace edge_list
} // -- namespace io
} // -- namespace lgraph

#include <lgraph/io/edge_list_wx.cpp>
