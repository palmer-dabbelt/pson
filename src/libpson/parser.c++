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
    ARRAY,
    EAT_COMMAS,
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

    std::vector<std::shared_ptr<tree>> array_elements;
    size_t array_opens;

    std::vector<std::string>::const_iterator child_start;

    for (auto it = start; it < stop; ++it) {
        auto token = *it;

        switch (state_stack.top()) {
        case state::TOP:
            if (token[0] == '"') {
                if (token[token.size() - 1] != '"') {
                    std::cerr << "Malformed string: no trailing \"\n";
                    abort();
                }
                auto stripped = token.substr(1, token.size() - 2);
                out = std::make_shared<tree_element<std::string>>(stripped);
                state_stack.push(state::DONE);
            } else if (token == "null") {
                out = std::make_shared<tree_null>();
                state_stack.push(state::DONE);
            } else if (token == "[") {
                state_stack.push(state::ARRAY);
                array_elements = {};
                child_start = it + 1;
                array_opens = 1;
            } else {
                std::cerr << "Unparsable token " << token << "\n";
                abort();
            }
            break;

        case state::DONE:
            if ((json_strict == false) && (token == ",")) {
                /* We explicitly allow extra trailing commas when not parsing
                 * JSON in strict mode. */
            } else {
                std::cerr << "Extra token after JSON file: " << token << "\n";
                abort();
            }
            break;

        case state::ARRAY:
            if (token == "{" || token == "[") {
                array_opens++;
            } else if (token == "}" || token == "]") {
                array_opens--;
            }

            if (array_opens == 1 && token == ",") {
                auto element = parse(child_start, it, json_strict);
                if (element == nullptr) {
                    std::cerr << "Unable to parse array element\n";
                    abort();
                }

                array_elements.push_back(element);
                state_stack.push(state::EAT_COMMAS);
            }

            if (array_opens == 0) {
                if (child_start <= it-1) {
                    auto element = parse(child_start, it, json_strict);
                    if (element == nullptr) {
                        std::cerr << "Unable to parse last array element\n";
                        abort();
                    }
                    array_elements.push_back(element);
                }
                
                if (token != "]") {
                    std::cerr << "Arrays must end with ]\n";
                    abort();
                }

                out = std::make_shared<tree_array>(array_elements);
                state_stack.push(state::DONE);
            }
            break;

        case state::EAT_COMMAS:
            child_start = it;
            /* This is funky, but essentially here we want to go ahead and
             * re-parse the closing brace.  Since we know EAT_COMMAS can't
             * enter an opening brace there's no reason to bother doing
             * anything like counting the opening braces. */
            if (token == "]" || token == "}")
                it--;
            if (token != ",")
                state_stack.pop();
            break;
        }
    }

    return out;
}
