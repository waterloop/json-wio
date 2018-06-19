#include <stdio.h>

#include <wlib/linked_list>

#include <wlib/wjson/JsonStringify.h>
#include <wlib/wjson/JsonElement.h>

#define const_void_cast(ptr) const_cast<void *>(static_cast<const void *>(ptr))

using namespace wlp;

struct json_iterator {
    union {
        json_object::const_iterator obj;
        json_array::const_iterator arr;
    };

    json_iterator();
    explicit json_iterator(const json_object::const_iterator &it);
    explicit json_iterator(const json_array::const_iterator &it);
    json_iterator &operator=(json_iterator &&it) noexcept;
};

json_iterator::json_iterator() {}
json_iterator::json_iterator(const json_object::const_iterator &it) : obj(it) {}
json_iterator::json_iterator(const json_array::const_iterator &it) : arr(it) {}
json_iterator &json_iterator::operator=(json_iterator &&it) noexcept {
    memcpy(this, &it, sizeof(json_iterator));
    return *this;
}

namespace stringify {
    typedef int (*fstringify_t)(char *, const json_element *);

    static int stringify_null(char *ptr, const json_element *) {
        memcpy(ptr, STR_NULL, STR_SIZE_NULL);
        return STR_SIZE_NULL;
    }

    static int stringify_true(char *ptr, const json_element *) {
        memcpy(ptr, STR_TRUE, STR_SIZE_TRUE);
        return STR_SIZE_TRUE;
    }
    static int stringify_false(char *ptr, const json_element *) {
        memcpy(ptr, STR_FALSE, STR_SIZE_FALSE);
        return STR_SIZE_FALSE;
    }
    static fstringify_t functions_bool[2] = {stringify_false, stringify_true};
    static int stringify_bool(char *ptr, const json_element *je)
    { return functions_bool[static_cast<bool>(je->integer())](ptr, je); }

    static int stringify_int(char *ptr, const json_element *je)
    { return sprintf(ptr, WLIB_JSON_INTFMT, je->integer()); }

    static int stringify_float(char *ptr, const json_element *je)
    { return sprintf(ptr, WLIB_JSON_FLOATFMT, je->floating()); }

    static int stringify_string(char *ptr, const json_element *je)
    { return sprintf(ptr, "\"%s\"", je->string().c_str()); }

    static fstringify_t functions[5] = {
        stringify_null, stringify_bool,
        stringify_int, stringify_float,
        stringify_string
    };
}

int json::stringify(char *buf, const json_element &je) {
    const json_element *cur;

    int wrt = 0;
    int dif;
    json_type_t cls;

    linked_list<void *> stack;
    stack.push_back(const_void_cast(&je));

    linked_list<json_iterator> array_its;
    linked_list<void *> array_stack;

    linked_list<json_iterator> object_its;
    linked_list<void *> object_stack;

    while (!stack.empty()) {
        cur = static_cast<const json_element *>(stack.back());

        cls = cur->type() >> 4;

        // primitive type
        if (cls < CLASS_ARRAY) {
            dif = stringify::functions[cls](buf, cur);
            buf += dif;
            wrt += dif;
            stack.pop_back();
        }

        // arrays
        else if (cls == CLASS_ARRAY) {
            // empty array
            if (cur->array().size() == 0) {
                *buf = '[';
                ++buf;
                *buf = ']';
                ++buf;

                wrt += 2;
                stack.pop_back();
            }

            // initiate another array
            else if (array_stack.empty() || &cur->array() != static_cast<const json_array *>(array_stack.back()))  {
                *buf = '[';
                ++buf;
                ++wrt;

                // hold the reference to the latest array being written
                array_stack.push_back(const_void_cast(&cur->array()));
                // keep a stack of iterators for each array too
                json_array::const_iterator begin = cur->array().begin();
                array_its.push_back(json_iterator(begin));

                // load the first element on the stack
                stack.push_back(const_void_cast(&*begin));
                // increment the iterator to next
                ++array_its.back().arr;
            }

            // move to next element
            else if (!array_stack.empty() && &cur->array() == static_cast<const json_array *>(array_stack.back())) {
                // write comma
                *buf = ',';
                ++buf;
                ++wrt;

                // end of array
                if (array_its.back().arr == cur->array().end()) {
                    // erase comma and write end bracket
                    *(buf - 1) = ']';

                    // pop array off array stack and stack
                    array_stack.pop_back();
                    array_its.pop_back();
                    stack.pop_back();
                }

                // load next element
                else {
                    stack.push_back(const_void_cast(&*array_its.back().arr));
                    ++array_its.back().arr;
                }
            }

        }

        // objects
        else if (cls == CLASS_OBJECT) {
            // empty object
            if (cur->object().size() == 0) {
                *buf = '{';
                ++buf;
                *buf = '}';
                ++buf;

                wrt += 2;
                stack.pop_back();
            }

            // initiate object
            else if (object_stack.empty() || &cur->object() != static_cast<const json_object *>(object_stack.back())) {
                *buf = '{';
                ++buf;
                ++wrt;

                // hold reference to latest object
                object_stack.push_back(const_void_cast(&cur->object()));
                // keep stack of iterators
                json_object::const_iterator begin = cur->object().begin();
                object_its.push_back(json_iterator(begin));

                // return error if key is not string
                const json_element &key = begin.key();
                if (!key.is_string()) { return -1; }

                // write key
                dif = sprintf(buf, "\"%s\":", key.string().c_str());
                buf += dif;
                wrt += dif;

                // load first element
                stack.push_back(const_void_cast(&*begin));
                // increment next iterator
                ++object_its.back().obj;
            }

            // write next key and load object
            else if (!object_stack.empty() && &cur->object() == static_cast<const json_object *>(object_stack.back())) {
                // write comma
                *buf = ',';
                ++buf;
                ++wrt;

                // end of object
                if (object_its.back().obj == cur->object().end()) {
                    // erase comma and write closing bracket
                    *(buf - 1) = '}';

                    // pop object off stacks
                    object_stack.pop_back();
                    object_its.pop_back();
                    stack.pop_back();
                }

                // write next key and load element
                else {
                    const json_element &key = object_its.back().obj.key();
                    if (!key.is_string()) { return -1; }

                    dif = sprintf(buf, "\"%s\":", key.string().c_str());
                    buf += dif;
                    wrt += dif;

                    stack.push_back(const_void_cast(&*object_its.back().obj));
                    ++object_its.back().obj;
                }
            }
        }

    };

    *buf = '\0';
    return wrt;
}
