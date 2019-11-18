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
#include <sstream>
#include <stack>
using namespace pson;

enum state {
    BODY,
    STRING,
    ESCAPE
};

std::vector<std::string> lexer::lex_file(const std::string& filename)
{
    std::ifstream file(filename);
    file >> std::noskipws;
    return lex_stream(file);
}

std::vector<std::string> lexer::lex_string(const std::string& data)
{
    std::istringstream ss(data);
    ss >> std::noskipws;
    return lex_stream(ss);
}

std::vector<std::string> lexer::lex_stream(std::istream& file)
{
    char c;
    std::string token = "";
    std::vector<std::string> out;

    std::stack<state> state_stack;
    state_stack.push(state::BODY);

    while (file >> c) {
        switch (state_stack.top()) {
        case state::BODY:
            switch (c) {
            case '\\':
                state_stack.push(state::ESCAPE);
                break;

            case '"':
                state_stack.push(state::STRING);
                token = token + c;
                break;

            case '[':
            case ']':
            case '{':
            case '}':
            case ',':
            case ':':
                if (token.size() > 0)
                    out.push_back(token);
                token = c;
                if (token.size() > 0)
                    out.push_back(token);
                token = "";
                break;

            case ' ':
            case '\t':
            case '\n':
                break;

            default:
                token = token + c;
            }
            break;

        case state::STRING:
            switch (c) {
            case '\\':
                state_stack.push(state::ESCAPE);
                break;

            case '"':
                state_stack.pop();
                token = token + c;
                if (token.size() > 0)
                    out.push_back(token);
                token = "";
                break;

            default:
                token = token + c;
            }
            break;

        case state::ESCAPE:
            token = token + c;
            state_stack.pop();
            break;
        }
    }
    if (token.size() > 0)
        out.push_back(token);

    return out;
}
