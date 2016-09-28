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

#ifndef LIBPSON__TREE_HXX
#define LIBPSON__TREE_HXX

#include <memory>
#include <vector>

namespace pson {
    /* A JSON file can be represented in memory as a tree.  There's a bunch of
     * sorts of these trees. */
    class tree {
    public:
        /* A non-recursive traversal function. */
        virtual const std::vector<std::shared_ptr<tree>>& children(void) const = 0;
    };

    /* Some tree nodes don't have children, here's a helper to automate that. */
    class tree_node_without_children: public tree {
    private:
        static const std::vector<std::shared_ptr<tree>> _there_are_no_children;

    public:
        virtual const std::vector<std::shared_ptr<tree>>& children(void) const {
            return _there_are_no_children;
        }
    };

    /* A tree node that contains a single element of some templated type. */
    template<typename T>
    class tree_element: public tree_node_without_children {
    private:
        const T _value;

    public:
        tree_element(const T& value)
        : _value(value)
        {}

    public:
        virtual const T& value(void) const { return _value; }
    };

    /* Represents the special "null" JSON type, which isn't the same as NULL or
     * nullptr (C++ types). */
    class tree_null: public tree_node_without_children {
    };

    /* Represents a JSON array, which has a bunch of children. */
    class tree_array: public tree {
    private:
        const std::vector<std::shared_ptr<tree>> _children;

    public:
        tree_array(const decltype(_children)& children)
        : _children(children)
        {}

    public:
        virtual const decltype(_children)& children(void) const { return _children; }
    };
}

#endif
