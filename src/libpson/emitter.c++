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

#include "emitter.h++"
#include <simple_match/simple_match.hpp>
#include <fstream>
#include <iostream>
using namespace pson;
using namespace simple_match;
using namespace simple_match::placeholders;

/* emit() doesn't leave any trailing whitspace or commas, that's for whatever
 * is the level above to create. */
static void emit(std::ofstream& out, size_t depth, const std::shared_ptr<tree>& root);
static void indent(std::ofstream& out, size_t depth);

void pson::emit_json(const std::string& filename, const std::shared_ptr<tree>& root)
{
    std::ofstream file(filename);
    emit(file, 0, root);
    file << "\n";
}

void emit(std::ofstream& out, size_t depth, const std::shared_ptr<tree>& root)
{
    match (root,
        some<tree_element<std::string>>(), [&](auto e) {
            indent(out, depth);
            out << "\"" << e.value() << "\"";
        },
        some<null_element>(), [&](auto e __attribute__((unused))) {
            indent(out, depth);
            out << "null";
        },
        none(), [&](){
            std::cerr << "Unmatched type in emit()\n";
            if (root == nullptr) {
                std::cerr << "  Cannot emit nullptr\n";
            }
            std::cerr << std::endl;
            abort();
        }
    );
}

void indent(std::ofstream& out, size_t depth)
{
    for (size_t i = 0; i < depth; ++i)
        out << "  ";
}
