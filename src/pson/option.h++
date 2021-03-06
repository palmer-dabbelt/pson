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

#ifndef LIBPSON__OPTION_HXX
#define LIBPSON__OPTION_HXX

#include <iostream>

namespace pson {
    /* */
    template<typename T> class option {
    private:
        bool _valid;
        T _du;

    public:
        option(void)
        : _valid(false),
          _du()
        {}

        option(const T& data)
        : _valid(true),
          _du(data)
        {}

    public:
        bool valid(void) const { return _valid; }
        const T& data(void) const {
            if (_valid == true)
                return _du;

            std::cerr << "accessing invalid option\n";
            abort();
        }
    };
}

#endif
