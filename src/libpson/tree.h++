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
        /* Provides some internal debugging information about a tree instance. */
        virtual const std::string debug(void) const = 0;
    };

    /* A tree node that contains a single element of some templated type. */
    template<typename T>
    class tree_element: public tree {
    private:
        const T _value;

    public:
        tree_element(const T& value)
        : _value(value)
        {}

    public:
        virtual const T& value(void) const { return _value; }
        virtual const std::string debug(void) const { return "tree_element"; }
    };

    /* Represents the special "null" JSON type, which isn't the same as NULL or
     * nullptr (C++ types). */
    class tree_null: public tree {
        virtual const std::string debug(void) const { return "tree_null"; }
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
        const std::vector<std::shared_ptr<tree>>& children(void) const { return _children; }
        virtual const std::string debug(void) const { return "tree_array"; }
    };
    static inline
    auto begin(const tree_array& a) -> decltype(begin(a.children()))
    { return begin(a.children()); }
    static inline
    auto end(const tree_array& a) -> decltype(end(a.children()))
    { return end(a.children()); }

    /* Objects are an ordered set of key/value pairs. */
    class tree_pair_t: public tree {
    public:
        virtual const std::shared_ptr<tree>& key(void) const = 0;
        virtual const std::shared_ptr<tree>& value(void) const = 0;
        virtual const std::string debug(void) const { return "tree_pair_t"; }
    };

    template <typename K, typename V>
    class tree_pair: public tree_pair_t {
    private:
        const K _key;
        const V _value;

    public:
        tree_pair(const K& key, const V& value)
        : _key(key),
          _value(value)
        {}

    public:
        virtual const std::shared_ptr<tree>& key(void) const { return _key; }
        virtual const std::shared_ptr<tree>& value(void) const { return _value; }
        virtual const std::string debug(void) const
        { return std::string("tree_pair<") + typeid(K).name() + ", " + typeid(V).name() + ">"; }
    };
    template <typename K, typename V>
    std::shared_ptr<tree_pair<K, V>> make_tree_pair(const K& key, const V& value)
    { return std::make_shared<tree_pair<K, V>>(key, value); }

    /* Represents a JSON object, which are just a bunch of pairs. */
    class tree_object: public tree {
    private:
        template <typename T>
        static std::vector<std::shared_ptr<tree_pair_t>> vcast(const std::vector<T>& children)
        {
            auto out = std::vector<std::shared_ptr<tree_pair_t>>();
            for (const auto& c: children)
                out.push_back(c);
            return out;
        }

    private:
        const std::vector<std::shared_ptr<tree_pair_t>> _children;

    public:
        template<typename T>
        tree_object(const std::vector<T>& children)
        : _children(vcast(children))
        {}

    public:
        const decltype(_children)& children(void) const { return _children; }
        virtual const std::string debug(void) const { return "tree_object"; }
    };
    static inline
    auto begin(const tree_object& a) -> decltype(begin(a.children()))
    { return begin(a.children()); }
    static inline
    auto end(const tree_object& a) -> decltype(end(a.children()))
    { return end(a.children()); }
}

#endif
