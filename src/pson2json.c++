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

#include <pson/parser.h++>
#include <pson/emitter.h++>
#include <tclap/CmdLine.h>
#include "version.h"

int main(int argc, const char **argv)
{
    try {
        TCLAP::CmdLine cmd(
            "Converts a PSON file to a JSON file\n",
            ' ',
            PCONFIGURE_VERSION);

        TCLAP::ValueArg<std::string> input("i",
                                           "input",
                                           "A PSON-formatted file",
                                           true,
                                           "",
                                           "in.pson");
        cmd.add(input);

        TCLAP::ValueArg<std::string> output("o",
                                            "output",
                                            "A JSON-formatted file",
                                            true,
                                            "",
                                            "out.json");
        cmd.add(output);

        cmd.parse(argc, argv);

        auto t = pson::parse_pson(input.getValue());
        pson::emit_json(output.getValue(), t);
        return 0;
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: "
                  << e.error()
                  << " for arg "
                  << e.argId()
                  << std::endl;
        return 2;
    }

    return 0;
}
