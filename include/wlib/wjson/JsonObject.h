#ifndef __WLIB_JSON_JSONOBJECT_H__
#define __WLIB_JSON_JSONOBJECT_H__

#include <wlib/hash_map>

#include <wlib/wjson/JsonType.h>
#include <wlib/wjson/JsonUtil.h>

namespace wlp {

    class json_element;

    template<typename key_t, typename val_t, typename hash_t, typename equal_t>
    using object_t = hash_map<key_t, val_t, hash_t, equal_t>;

    class json_object :
        public object_t<
            json_element,
            json_element,
            json_element_hash,
            json_element_equals> {

        typedef object_t<json_element, json_element, 
            json_element_hash, json_element_equals> parent_t;
        enum {
            DEFAULT_SIZE = 12,
            DEFAULT_LOAD = 80
        };

    public:
        explicit json_object(size_type n = DEFAULT_SIZE, percent_type load = DEFAULT_LOAD);

        json_object(json_object &&obj);
        json_object &operator=(json_object &&obj);

        json_object(const json_object &) = delete;
        json_object &operator=(const json_object &) = delete;

        template<typename ...arg_t>
        json_object(arg_t &&...args) : parent_t(sizeof...(arg_t) / 2, DEFAULT_LOAD) 
        { prv_insert(forward<arg_t>(args)...); }

    public:
        template<typename key_t, typename val_t>
        pair<iterator, bool> insert(key_t &&key, val_t &&val) {
            return parent_t::insert(
                json_element(forward<key_t>(key)),
                json_element(forward<val_t>(val))
            );
        }

        template<typename key_t, typename val_t>
        pair<iterator, bool> insert_or_assign(key_t &&key, val_t &&val) {
            return parent_t::insert_or_assign(
                json_element(forward<key_t>(key)),
                json_element(forward<val_t>(val))
            );
        }

        template<typename key_t>
        bool erase(key_t &&key)
        { return parent_t::erase(json_element(forward<key_t>(key))); }

        template<typename key_t>
        json_element &at(key_t &&key)
        { return parent_t::at(json_element(forward<key_t>(key))); }

        template<typename key_t>
        const json_element &at(key_t &&key) const
        { return parent_t::at(json_element(forward<key_t>(key))); }

        template<typename key_t>
        bool contains(key_t &&key) const
        { return parent_t::contains(json_element(forward<key_t>(key))); }

        template<typename key_t>
        iterator find(key_t &&key)
        { return parent_t::find(json_element(forward<key_t>(key))); }

        template<typename key_t>
        const_iterator find(key_t &&key) const
        { return parent_t::find(json_element(forward<key_t>(key))); }

        template<typename key_t>
        json_element &operator[](key_t &&key)
        { return parent_t::operator[](json_element(forward<key_t>(key))); }

    private:
        template<typename ...arg_t>
        void prv_insert() {}
        template<typename key_t, typename val_t, typename ...arg_t>
        void prv_insert(key_t &&key, val_t &&val, arg_t &&...args) {
            insert(forward<key_t>(key), forward<val_t>(val));
            prv_insert(forward<arg_t>(args)...);
        }
    };

}

#endif
