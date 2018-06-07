#ifndef __WLIB_JSON_JSONOBJECT_H__
#define __WLIB_JSON_JSONOBJECT_H__

#include <wlib/hash_map>

#include <wlib/Json/JsonType.h>
#include <wlib/Json/JsonUtil.h>

namespace wlp {

    class json_element;

    class json_object :
        public hash_map<
            json_element,
            json_element,
            json_element_hash,
            json_element_equals> {

        enum {
            JSON_OBJECT_DEFAULT_SIZE = 12,
            JSON_OBJECT_DEFAULT_LOAD = 75
        };

    public:
        explicit json_object(
            size_type n = JSON_OBJECT_DEFAULT_SIZE,
            percent_type load = JSON_OBJECT_DEFAULT_LOAD);

        json_object(json_object &&obj);
        json_object &operator=(json_object &&obj);

        json_object(const json_object &) = delete;
        json_object &operator=(const json_object &) = delete;

    public:
        template<typename key_t, typename val_t>
        pair<iterator, bool> insert(key_t &&key, val_t &&val) {
            return hash_map::insert(
                json_element(forward<key_t>(key)),
                json_element(forward<val_t>(val))
            );
        }

        template<typename key_t, typename val_t>
        pair<iterator, bool> insert_or_assign(key_t &&key, val_t &&val) {
            return hash_map::insert_or_assign(
                json_element(forward<key_t>(key)),
                json_element(forward<val_t>(val))
            );
        }

        template<typename key_t>
        bool erase(key_t &&key)
        { return hash_map::erase(json_element(forward<key_t>(key))); }

        template<typename key_t>
        json_element &at(key_t &&key)
        { return hash_map::at(json_element(forward<key_t>(key))); }

        template<typename key_t>
        const json_element &at(key_t &&key) const
        { return hash_map::at(json_element(forward<key_t>(key))); }

        template<typename key_t>
        bool contains(key_t &&key) const
        { return hash_map::contains(json_element(forward<key_t>(key))); }

        template<typename key_t>
        iterator find(key_t &&key)
        { return hash_map::find(json_element(forward<key_t>(key))); }

        template<typename key_t>
        const_iterator find(key_t &&key) const
        { return hash_map::find(json_element(forward<key_t>(key))); }

        template<typename key_t>
        json_element &operator[](key_t &&key)
        { return hash_map::operator[](json_element(forward<key_t>(key))); }
    };

}

#endif
