#ifndef __WLIB_JSON_JSONARRAY_H__
#define __WLIB_JSON_JSONARRAY_H__

#include <wlib/array_list>
#include <wlib/initializer_list>

namespace wlp {

    class json_element;

    class json_array :
        public array_list<json_element> {

        enum {
            JSON_ARRAY_DEFAULT_SIZE = 15
        };

    public:
        explicit json_array(
            size_type n = JSON_ARRAY_DEFAULT_SIZE);

        json_array(json_array &&arr);
        json_array &operator=(json_array &&arr);

        json_array(const json_array &) = delete;
        json_array &operator=(const json_array &) = delete;

        json_array(initializer_list<json_element> l);

        template<typename ...args>
        json_array(args &&...values) : array_list(sizeof...(args)) 
        { swallow(prv_push_back(values)...); }

        template<typename val_t>
        iterator insert(size_type i, val_t &&val)
        { return array_list::insert(i, json_element(forward<val_t>(val))); }

        template<typename val_t>
        iterator insert(const iterator &it, val_t &&val)
        { return array_list::insert(it, json_element(forward<val_t>(val))); }

        template<typename val_t>
        void push_back(val_t &&val)
        { array_list::push_back(json_element(forward<val_t>(val))); }

        template<typename val_t>
        void push_front(val_t &&val)
        { array_list::push_front(json_element(forward<val_t>(val))); }

        template<typename val_t>
        size_type index_of(val_t &&val) const
        { return array_list::index_of(json_element(forward<val_t>(val)));  }

        template<typename val_t>
        iterator find(val_t &&val)
        { return array_list::find(json_element(forward<val_t>(val))); }

        template<typename val_t>
        const_iterator find(val_t &&val) const
        { return array_list::find(json_element(forward<val_t>(val))); }

    private:
        template<typename val_t>
        bool prv_push_back(val_t &&val) {
            push_back(forward<val_t>(val)); 
            return true; 
        }
    };

}

#endif
