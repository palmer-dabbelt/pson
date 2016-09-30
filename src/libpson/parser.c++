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
#include <map>
#include <stack>
using namespace pson;

enum state {
    TOP,
    DONE,
    ARRAY,
    EAT_COMMAS,
    OBJECT_KEY,
    OBJECT_VALUE,
};

static
std::shared_ptr<tree> parse(const std::vector<std::string>::const_iterator start,
                            const std::vector<std::string>::const_iterator stop,
                            bool json_strict);

static inline std::string to_string(const enum state& s);

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

    size_t child_opens = 0;
    std::vector<std::string>::const_iterator child_start;
    std::vector<std::shared_ptr<tree>> child_elements;
    std::vector<std::shared_ptr<tree_pair_t>> child_pairs;
    std::shared_ptr<tree> child_key;

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
                child_elements = {};
                child_start = it + 1;
                child_opens = 1;
            } else if (token == "{") {
                state_stack.push(state::OBJECT_KEY);
                child_pairs = {};
                child_start = it + 1;
                child_opens = 1;
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
                child_opens++;
            } else if (token == "}" || token == "]") {
                child_opens--;
            }

            if (child_opens == 1 && token == ",") {
                auto element = parse(child_start, it, json_strict);
                if (element == nullptr) {
                    std::cerr << "Unable to parse array element\n";
                    abort();
                }

                child_elements.push_back(element);
                state_stack.push(state::EAT_COMMAS);
            }

            if (child_opens == 0) {
                if (child_start <= it-1) {
                    auto element = parse(child_start, it, json_strict);
                    if (element == nullptr) {
                        std::cerr << "Unable to parse last array element\n";
                        abort();
                    }
                    child_elements.push_back(element);
                }
                
                if (token != "]") {
                    std::cerr << "Arrays must end with ]\n";
                    abort();
                }

                out = std::make_shared<tree_array>(child_elements);
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

        case state::OBJECT_KEY:
            if (token == "{" || token == "[")
                child_opens++;
            if (token == "}" || token == "]")
                child_opens--;

            if (child_opens == 1 && token == ":") {
                child_key = parse(child_start, it, json_strict);
                if (child_key == nullptr) {
                    std::cerr << "Unable to parse object key\n";
                    abort();
                }
                child_start = it + 1;

                state_stack.push(state::OBJECT_VALUE);
            }

            if (child_opens == 0) {
                out = std::make_shared<tree_object>(child_pairs);
                state_stack.push(state::DONE);
            }
            break;


        case state::OBJECT_VALUE:
            if (token == "{" || token == "[")
                child_opens++;
            if (token == "}" || token == "]")
                child_opens--;

            if (child_opens == 1 && token == ",") {
                auto child_value = parse(child_start, it, json_strict);
                if (child_value == nullptr) {
                    std::cerr << "Unable to parse object value\n";
                    abort();
                }
                if (child_key == nullptr) {
                    std::cerr << "Object value without key\n";
                    abort();
                }

                child_pairs.push_back(make_tree_pair(child_key, child_value));
                child_key = nullptr;
                state_stack.pop();
                state_stack.push(state::EAT_COMMAS);
            }

            if (child_opens == 0) {
                auto child_value = parse(child_start, it, json_strict);
                if (child_value == nullptr) {
                    std::cerr << "Unable to parse object value\n";
                    abort();
                }
                if (child_key == nullptr) {
                    std::cerr << "Object value without key\n";
                    abort();
                }

                child_pairs.push_back(make_tree_pair(child_key, child_value));
                child_key = nullptr;
                state_stack.pop();
                state_stack.push(state::EAT_COMMAS);

                out = std::make_shared<tree_object>(child_pairs);
                state_stack.push(state::DONE);
            }
            break;
        }
    }

    if (out == nullptr) {
        std::cerr << "Unable to parse" << std::endl;
        abort();
    }
    return out;
}

std::string to_string(const enum state& s)
{
    switch (s) {
    case state::TOP: return "TOP";
    case state::DONE: return "DONE";
    case state::ARRAY: return "ARRAY";
    case state::EAT_COMMAS: return "EAT_COMMAS";
    case state::OBJECT_KEY: return "OBJECT_KEY";
    case state::OBJECT_VALUE: return "OBJECT_VALUE";
    }

    abort();
    return "";
}
