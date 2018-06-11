#ifndef __WLIB_JSON_JSONARRAY_H__
#define __WLIB_JSON_JSONARRAY_H__

#include <wlib/array_list>
#include <wlib/initializer_list>

namespace wlp {

    class json_element;

    template<typename val_t>
    using array_t = array_list<val_t>;

    class json_array :
        public array_t<json_element> {

        typedef array_t<json_element> parent_t;
        enum {
            DEFAULT_SIZE = 15
        };

    public:
        explicit json_array(
            size_type n = DEFAULT_SIZE);

        json_array(json_array &&arr);
        json_array &operator=(json_array &&arr);

        json_array(const json_array &) = delete;
        json_array &operator=(const json_array &) = delete;

        json_array(initializer_list<json_element> l);

        template<typename ...arg_t>
        json_array(arg_t &&...args) : parent_t(sizeof...(arg_t))
        { swallow(prv_push_back(forward<arg_t>(args))...); }

    public:
        template<typename val_t>
        iterator insert(size_type i, val_t &&val)
        { return parent_t::insert(i, json_element(forward<val_t>(val))); }

        template<typename val_t>
        iterator insert(const iterator &it, val_t &&val)
        { return parent_t::insert(it, json_element(forward<val_t>(val))); }

        template<typename val_t>
        void push_back(val_t &&val)
        { parent_t::push_back(json_element(forward<val_t>(val))); }

        template<typename val_t>
        void push_front(val_t &&val)
        { parent_t::push_front(json_element(forward<val_t>(val))); }

        template<typename val_t>
        size_type index_of(val_t &&val) const
        { return parent_t::index_of(json_element(forward<val_t>(val)));  }

        template<typename val_t>
        iterator find(val_t &&val)
        { return parent_t::find(json_element(forward<val_t>(val))); }

        template<typename val_t>
        const_iterator find(val_t &&val) const
        { return parent_t::find(json_element(forward<val_t>(val))); }

    private:
        template<typename val_t>
        bool prv_push_back(val_t &&val) {
            // template pack initializes backwards...
            push_back(forward<val_t>(val));
            return true; // return value needed for swallow
        }
    };

}

#endif
