/*
 * This file is part of pson: Palmer's JSON Parsing Library
 * Copyright (C) 2016 Palmer Dabbelt <palmer@dabelt.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef LIBPSON__PARSER_HXX
#define LIBPSON__PARSER_HXX

#include "tree.h++"
#include <memory>
#include <string>

namespace pson {
    /* A JSON parser. */
    std::shared_ptr<tree> parse_json_file(const std::string& filename);
    std::shared_ptr<tree> parse_json_string(const std::string& data);

    /* A PSON parser.  PSON is a superset of JSON: all JSON files are valid
     * PSON files, but PSON allows trailing commas anywhere. */
    std::shared_ptr<tree> parse_pson_string(const std::string& filename);
    std::shared_ptr<tree> parse_pson_file(const std::string& data);
}

#endif
