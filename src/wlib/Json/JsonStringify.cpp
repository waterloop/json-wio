#include <stdio.h>

#include <wlib/linked_list>

#include <wlib/Json/JsonStringify.h>
#include <wlib/Json/JsonElement.h>

using namespace wlp;

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

namespace count {
    typedef int (*fcount_t)(const json_element *);
    static int count_null(const json_element *)
    { return STR_SIZE_NULL; }
    static int count_bool(const json_element *je)
    { return static_cast<bool>(je->integer()) ? STR_SIZE_TRUE : STR_SIZE_FALSE; }
    static int count_int(const json_element *je)
    { return snprintf(nullptr, 0, WLIB_JSON_INTFMT, je->integer()); }
    static int count_float(const json_element *je)
    { return snprintf(nullptr, 0, WLIB_JSON_FLOATFMT, je->floating()); }
    static int count_string(const json_element *je)
    { return je->string().length() + 2; }
    static fcount_t functions[5] = {
        count_null, count_bool,
        count_int, count_float,
        count_string
    };
}

int json::buff_size(const json_element &je) {
    // essentially pre-stringifies the JSON
    const json_element *cur;
    int wrt = 0;
    json_type_t cls;
    linked_list<const json_element *> stack;
    stack.push_back(&je);
    linked_list<json_array::const_iterator> array_its;
    linked_list<const json_array *> array_stack;
    linked_list<json_object::const_iterator> object_its;
    linked_list<const json_object *> object_stack;
    while (!stack.empty()) {
        cur = stack.back();
        cls = cur->type() >> 4;
        if (cls < CLASS_ARRAY) {
            wrt += count::functions[cls](cur);
            stack.pop_back();
        } else if (cls == CLASS_ARRAY) {
            if (cur->array().size() == 0) {
                wrt += 2;
                stack.pop_back();
            } else if (array_stack.empty() || &cur->array() != array_stack.back())  {
                ++wrt;
                array_stack.push_back(&cur->array());
                json_array::const_iterator begin = cur->array().begin();
                array_its.push_back(begin);
                stack.push_back(&*begin);
                ++array_its.back();
            } else if (!array_stack.empty() && &cur->array() == array_stack.back()) {
                ++wrt;
                if (array_its.back() == cur->array().end()) {
                    array_stack.pop_back();
                    array_its.pop_back();
                    stack.pop_back();
                } else {
                    stack.push_back(&*array_its.back());
                    ++array_its.back();
                }
            }

        } else if (cls == CLASS_OBJECT) {
            if (cur->object().size() == 0) {
                wrt += 2;
                stack.pop_back();
            }
            else if (object_stack.empty() || &cur->object() != object_stack.back()) {
                ++wrt;
                object_stack.push_back(&cur->object());
                json_object::const_iterator begin = cur->object().begin();
                object_its.push_back(begin);
                const json_element &key = begin.key();
                if (!key.is_string()) { return -1; }
                wrt += key.string().length() + 3;
                stack.push_back(&*begin);
                ++object_its.back();
            }
            else if (!object_stack.empty() && &cur->object() == object_stack.back()) {
                ++wrt;
                if (object_its.back() == cur->object().end()) {
                    object_stack.pop_back();
                    object_its.pop_back();
                    stack.pop_back();
                }
                else {
                    const json_element &key = object_its.back().key();
                    if (!key.is_string()) { return -1; }
                    wrt += key.string().length() + 3;
                    stack.push_back(&*object_its.back());
                    ++object_its.back();
                }
            }
        }

    };
    return wrt;
}

int json::stringify(char *buf, const json_element &je) {
    const json_element *cur;

    int wrt = 0;
    int dif;
    json_type_t cls;

    linked_list<const json_element *> stack;
    stack.push_back(&je);

    linked_list<json_array::const_iterator> array_its;
    linked_list<const json_array *> array_stack;

    linked_list<json_object::const_iterator> object_its;
    linked_list<const json_object *> object_stack;

    while (!stack.empty()) {
        cur = stack.back();

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
            else if (array_stack.empty() || &cur->array() != array_stack.back())  {
                *buf = '[';
                ++buf;
                ++wrt;

                // hold the reference to the latest array being written
                array_stack.push_back(&cur->array());
                // keep a stack of iterators for each array too
                json_array::const_iterator begin = cur->array().begin();
                array_its.push_back(begin);

                // load the first element on the stack
                stack.push_back(&*begin);
                // increment the iterator to next
                ++array_its.back();
            }

            // move to next element
            else if (!array_stack.empty() && &cur->array() == array_stack.back()) {
                // write comma
                *buf = ',';
                ++buf;
                ++wrt;

                // end of array
                if (array_its.back() == cur->array().end()) {
                    // erase comma and write end bracket
                    *(buf - 1) = ']';

                    // pop array off array stack and stack
                    array_stack.pop_back();
                    array_its.pop_back();
                    stack.pop_back();
                }

                // load next element
                else {
                    stack.push_back(&*array_its.back());
                    ++array_its.back();
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
            else if (object_stack.empty() || &cur->object() != object_stack.back()) {
                *buf = '{';
                ++buf;
                ++wrt;

                // hold reference to latest object
                object_stack.push_back(&cur->object());
                // keep stack of iterators
                json_object::const_iterator begin = cur->object().begin();
                object_its.push_back(begin);

                // return error if key is not string
                const json_element &key = begin.key();
                if (!key.is_string()) { return -1; }

                // write key
                dif = sprintf(buf, "\"%s\":", key.string().c_str());
                buf += dif;
                wrt += dif;

                // load first element
                stack.push_back(&*begin);
                // increment next iterator
                ++object_its.back();
            }

            // write next key and load object
            else if (!object_stack.empty() && &cur->object() == object_stack.back()) {
                // write comma
                *buf = ',';
                ++buf;
                ++wrt;

                // end of object
                if (object_its.back() == cur->object().end()) {
                    // erase comma and write closing bracket
                    *(buf - 1) = '}';

                    // pop object off stacks
                    object_stack.pop_back();
                    object_its.pop_back();
                    stack.pop_back();
                }

                // write next key and load element
                else {
                    const json_element &key = object_its.back().key();
                    if (!key.is_string()) { return -1; }

                    dif = sprintf(buf, "\"%s\":", key.string().c_str());
                    buf += dif;
                    wrt += dif;

                    stack.push_back(&*object_its.back());
                    ++object_its.back();
                }
            }
        }

    };

    *buf = '\0';
    return wrt;
}
