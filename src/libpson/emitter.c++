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
            out << "\"" << e.value() << "\"";
        },
        some<tree_null>(), [&](auto e __attribute__((unused))) {
            out << "null";
        },
        some<tree_array>(), [&](auto e) {
            out << "[\n";

            auto it = begin(e);
            auto e_end = end(e);
            while (it < e_end) {
                indent(out, depth + 1);
                emit(out, depth + 1, *it);
                ++it;
                if (it < e_end)
                    out << ",\n";
            }

            out << "\n";
            indent(out, depth);
            out << "]";
        },
        some<tree_object>(), [&](auto e) {
            out << "{\n";

            auto it = begin(e);
            auto e_end = end(e);
            while (it < e_end) {
                indent(out, depth + 1);
                emit(out, depth + 1, (*it)->key());
                out << ": ";
                emit(out, depth + 1, (*it)->value());
                ++it;
                if (it < e_end)
                    out << ",\n";
            }

            out << "\n";
            indent(out, depth);
            out << "}";
        },
        none(), [&](){
            std::cerr << "Unmatched type in emit()" << std::endl;
            if (root == nullptr) {
                std::cerr << "  Cannot emit nullptr" << std::endl;
            } else {
                std::cerr << "  root->debug(): " << root->debug() << std::endl;
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
