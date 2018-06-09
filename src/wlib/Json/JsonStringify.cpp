#include <wlib/linked_list>

#include <wlib/Json/JsonStringify.h>
#include <wlib/Json/JsonElement.h>

using namespace wlp;

namespace stringify {
    typedef int (*fstringify_t)(char *, const json_element &);
    
    static int stringify_null(char *ptr, const json_element &) {
        memcpy(ptr, STR_NULL, STR_SIZE_NULL);
        return STR_SIZE_NULL;
    }
    
    static int stringify_true(char *ptr, const json_element &) {
        memcpy(ptr, STR_TRUE, STR_SIZE_TRUE);
        return STR_SIZE_TRUE;
    }
    static int stringify_false(char *ptr, const json_element &) {
        memcpy(ptr, STR_FALSE, STR_SIZE_FALSE);
        return STR_SIZE_FALSE;
    }
    static fstringify_t functions_bool[2] = {stringify_true, stringify_false};
    static int stringify_bool(char *ptr, const json_element &je) 
    { return functions_bool[static_cast<bool>(je.integer())](ptr, je); }

    static int stringify_int(char *ptr, const json_element &je) 
    { return sprintf(ptr, "%lli", je.integer()); }

    static int stringify_float(char *ptr, const json_element &je) 
    { return sprintf(ptr, "%Lf", je.floating()); }

    static int stringify_string(char *ptr, const json_element &je) 
    { return sprintf(ptr, "\"%s\"", je.string().c_str()); }

    static fstringify_t functions[5] = {
        stringify_null, stringify_bool, 
        stringify_int, stringify_float,
        stringify_string
    };
}

enum state {
    PRIMITIVE,
    ARRAY,
    OBJECT  
};

int json::stringify(char *buf, const json_element &je) {
    const json_element *cur;

    int wrt = 0;
    int dif;
    json_type_t cls;

    linked_list<const json_element *> stack;
    stack.push_back(&je);



    while (!stack.empty()) {
        cur = stack.back();
        
        cls = cur->type() >> 4;

        if (cls < CLASS_ARRAY) {
            dif = stringify::functions[cls](buf, je);
            buf += dif;
            wrt += dif;
            stack.pop_back();
        }

        else if (cls == CLASS_ARRAY) {
            *buf = '[';
            ++buf;
            ++wrt;
            
        }

    };

    return wrt;
}
