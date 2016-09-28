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

#include "lexer.h++"
#include <fstream>
using namespace pson;

enum state {
    BODY,
};

std::vector<std::string> lexer::lex(const std::string& filename)
{
    std::ifstream file(filename);
    file >> std::noskipws;

    char c;
    std::string token = "";
    std::vector<std::string> out;

    while (file >> c) {
        switch (c) {
        default:
            token = token + c;
        }
    }
    if (token.size() > 0)
        out.push_back(token);

    return out;
}
