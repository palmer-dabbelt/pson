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

#include "parser.h++"
#include "lexer.h++"
#include <iostream>
#include <stack>
using namespace pson;

enum state {
    TOP,
    DONE,
};

static
std::shared_ptr<tree> parse(const std::vector<std::string>::const_iterator start,
                            const std::vector<std::string>::const_iterator stop,
                            bool json_strict);

std::shared_ptr<tree> pson::parse_pson(const std::string& filename)
{
    auto tokens = lexer::lex(filename);
    return parse(tokens.begin(), tokens.end(), false);
}

std::shared_ptr<tree> parse(const std::vector<std::string>::const_iterator start,
                            const std::vector<std::string>::const_iterator stop,
                            bool json_strict)
{
    std::stack<state> state_stack;
    state_stack.push(state::TOP);

    std::shared_ptr<tree> out = nullptr;

    for (auto it = start; it < stop; ++it) {
        auto token = *it;

        switch (state_stack.top()) {
        case state::TOP:
            state_stack.pop();
            state_stack.push(state::DONE);
            out = std::make_shared<tree_element<std::string>>(token);
            break;

        case state::DONE:
            std::cerr << "Extra token after JSON file: " << token << "\n";
            abort();
            break;
        }
    }

    return out;
}
