#include <wlib/linked_list>

#include <wlib/wjson/JsonParse.h>
#include <wlib/wjson/JsonElement.h>

using namespace wlp;

/***************************************************************************
 * Data types
 **************************************************************************/
typedef signed char state_t;
enum invalid : state_t { INVALID = -1 };

template<char lo, char hi>
static bool between(char i)
{ return lo <= i && i <= hi; }


/***************************************************************************
 * Function prototypes
 **************************************************************************/
static bool parse_element(const char *str, json_element *ret);
static size_type skip_ahead(const char *str, size_type index);

static bool is_number_start(char ch);
static bool is_number_exit(char ch);
static bool is_digit(char ch);
static bool is_exponent(char ch);
static bool number_scan(const char *str, size_type index, size_type *end, bool *flt);

static bool is_true_start(char ch);
static bool is_false_start(char ch);
static bool is_null_start(char ch);
static bool true_scan(const char *str, size_type index, size_type *end);
static bool false_scan(const char *str, size_type index, size_type *end);
static bool null_scan(const char *str, size_type index, size_type *end);

static bool is_string_start(char ch);
static bool is_hexadecimal(char ch);
static bool is_valid_escape(char ch);
static bool string_scan(const char *str, size_type index, size_type *end);

static bool is_array_start(char ch);
static bool is_array_end(char ch);
static bool is_object_start(char ch);
static bool is_object_end(char ch);

/***************************************************************************
 * Public functions
 **************************************************************************/
json_element json::parse(const char *str) {
    json_element el;
    parse_element(str, &el);
    return el;
}

/***************************************************************************
 * Function definitions
 **************************************************************************/
static bool parse_element(const char *str, json_element *ret) {
    size_type index = 0;

    json_element *cur;
    size_type end;
    size_type len;
    bool flt;
    char ch;

    linked_list<void *> ref_stack;

    ref_stack.push_back(ret);

    while (!ref_stack.empty()) {

        index = skip_ahead(str, index);
        ch = str[index];
        cur = static_cast<json_element *>(ref_stack.back());

        // comma
        if (ch == ',') {

            // active object
            if (cur->is_object()) {
                // read next key
                ++index;
                index = skip_ahead(str, index);
                if (!is_string_start(str[index]))
                { return false; }
                if (!string_scan(str, index, &end)) 
                { return false; }
                len = end - index;
                dynamic_string string(str + index + 1, len - 2);
                index = end;
                
                auto itp = cur->object().insert(move(string), nullptr);
                if (!itp.second())
                { return false; }
                index = skip_ahead(str, index);
                if (str[index] != ':') 
                { return false; }
                ++index;
                ref_stack.push_back(&*itp.first());
            }

            // active array
            else if (cur->is_array()) {
                // load next element
                cur->array().push_back(nullptr);
                ref_stack.push_back(&cur->array().back());
                ++index;
            }

            // invalid
            else { return false; }
        }

        // number
        else if (is_number_start(ch)) {
            if (!number_scan(str, index, &end, &flt))
            { return false; }

            if (flt)
            { *cur = WLIB_JSON_STRTOFLOAT(str + index, nullptr); }
            else
            { *cur = WLIB_JSON_STRTOINT(str + index, nullptr); }
            index = end - 1;
            ref_stack.pop_back();
        }

        // strings
        else if (is_string_start(ch)) {
            if (!string_scan(str, index, &end))
            { return false; }

            len = end - index;
            dynamic_string string(str + index + 1, len - 2);
            *cur = move(string);
            index = end;
            ref_stack.pop_back();
        }

        // arrays
        else if (is_array_start(ch)) {
            // initiate new array
            json_array arr;
            *cur = move(arr);

            // index points to '['
            end = skip_ahead(str, index + 1);

            // empty array
            if (is_array_end(str[end])) {
                // end points to ']'
                index = end + 1;
                ref_stack.pop_back();
            }

            // initiate next element
            else {
                cur->array().push_back(nullptr);
                ref_stack.push_back(&cur->array().back());
                ++index;
            }
        }

        // object
        else if (is_object_start(ch)) {
            // initiate new object
            json_object obj;
            *cur = move(obj);

            end = skip_ahead(str, index + 1);
            if (is_object_end(str[end])) {
                index = end + 1;
                ref_stack.pop_back();
            }

            else {
                // key must be string
                if (!is_string_start(str[end]))
                { return false; }

                index = end;
                if (!string_scan(str, index, &end))
                { return false; }

                // create string key
                len = end - index;
                dynamic_string string(str + index + 1, len - 2);
                index = end;

                // insert
                auto itp = cur->object().insert(move(string), nullptr);
                if (!itp.second())
                { return false; }

                // handle colon
                index = skip_ahead(str, index);
                if (str[index] != ':')
                { return false; }
                ++index;

                ref_stack.push_back(&*itp.first());
            }
        }

        // true
        else if (is_true_start(ch)) {
            if (!true_scan(str, index, &end))
            { return false; }

            *cur = true;
            index = end;
            ref_stack.pop_back();
        }

        // false
        else if (is_false_start(ch)) {
            if (!false_scan(str, index, &end))
            { return false; }

            *cur = false;
            index = end;
            ref_stack.pop_back();
        }

        // null
        else if (is_null_start(ch)) {
            if (!null_scan(str, index, &end))
            { return false; }

            *cur = nullptr;
            index = end;
            ref_stack.pop_back();
        }

        // end of object
        else if (is_object_end(ch)) {
            if (ref_stack.empty() || !static_cast<json_element *>(ref_stack.back())->is_object())
            { return false; }
            // pop off object
            ref_stack.pop_back();
            ++index;
        }

        // end of array
        else if (is_array_end(ch)) {
            if (ref_stack.empty() || !static_cast<json_element *>(ref_stack.back())->is_array())
            { return false; }
            // pop off the array
            ref_stack.pop_back();
            ++index;
        }

        // invalid
        else { return false; }
    }

    return true;
}

static size_type skip_ahead(const char *str, size_type index) {
    char ch;
    char nt;
    while ((ch = str[index]) != '\0') {
        if ('\t' <= ch && ch <= ' ')
        { ++index; }

        // comments
        else if (ch == '/') {
            nt = str[index + 1];

            // block comment
            if (nt == '*') {
                ++index;
                do {
                    ++index;
                    if (str[index] == '\0') { return index; }
                } while (str[index] != '*' || str[index + 1] != '/');
                index += 2;

            // line comment
            } else if (nt == '/') {
                do {
                    ++index;
                    if (str[index] == '\0') { return index; }
                } while (str[index] != '\n');
            }

            // not a comment
            else { return index; }

        }

        // not a space or comment
        else { return index; }
    }

    return index;
}

static bool is_array_start(char ch) { return ch == '['; }
static bool is_array_end(char ch) { return ch == ']'; }
static bool is_object_start(char ch) { return ch == '{'; }
static bool is_object_end(char ch) { return ch == '}'; }

static bool is_digit(char ch)
{ return '0' <= ch && ch <= '9'; }
static bool is_number_start(char ch)
{ return ch == '-' || is_digit(ch); }
static bool is_number_exit(char ch)
{ return ch == '\0' || ch == ']' || ch == '}' ||
    ch == ',' || ch == ' ' || ch == '/'; }
static bool is_exponent(char ch)
{ return ch == 'e' || ch == 'E'; }
namespace number_sm {
    enum number_state : state_t {
        N_ST_START = 0,
        N_ST_NEGATIVE = 1,
        N_ST_DIGIT = 2,
        N_ST_PERIOD = 3,
        N_ST_DECIMAL = 4,
        N_ST_EXPONENT = 5,
        N_ST_SIGN = 6,
        N_ST_VALUE = 7,
        N_ST_STATES = 8
    };
    typedef state_t (*fstate_t)(char ch);
    static state_t n_st_start(char ch) {
        if (ch == '-') { return N_ST_NEGATIVE; }
        else if (is_digit(ch)) { return N_ST_DIGIT; }
        return INVALID;
    }
    static state_t n_st_negative(char ch) {
        if (is_digit(ch)) { return N_ST_DIGIT; }
        return INVALID;
    }
    static state_t n_st_digit(char ch) {
        if (is_digit(ch)) { return N_ST_DIGIT; }
        if (ch == '.') { return N_ST_PERIOD; }
        if (is_exponent(ch)) { return N_ST_EXPONENT; }
        if (is_number_exit(ch)) { return N_ST_STATES; }
        return INVALID;
    }
    static state_t n_st_period(char ch) {
        if (is_digit(ch)) { return N_ST_DECIMAL; }
        return INVALID;
    }
    static state_t n_st_decimal(char ch) {
        if (is_digit(ch)) { return N_ST_DECIMAL; }
        if (is_exponent(ch)) { return N_ST_EXPONENT; }
        if (is_number_exit(ch)) { return N_ST_STATES; }
        return INVALID;
    }
    static state_t n_st_exponent(char ch) {
        if (ch == '+' || ch == '-') { return N_ST_SIGN; }
        return INVALID;
    }
    static state_t n_st_sign(char ch) {
        if (is_digit(ch)) { return N_ST_VALUE; }
        return INVALID;
    }
    static state_t n_st_value(char ch) {
        if (is_digit(ch)) { return N_ST_VALUE; }
        if (is_number_exit(ch)) { return N_ST_STATES; }
        return INVALID;
    }
    static fstate_t transition[N_ST_STATES] = {
        n_st_start, n_st_negative, n_st_digit,
        n_st_period, n_st_decimal, n_st_exponent,
        n_st_sign, n_st_value
    };
}
static bool number_scan(const char *str, size_type index, size_type *end, bool *flt) {
    state_t state = number_sm::N_ST_START;
    *flt = false;
    do {
        state = number_sm::transition[state](str[index]);
        ++index;
        if (state == number_sm::N_ST_PERIOD ||
            state == number_sm::N_ST_EXPONENT)
        { *flt = true; }
        if (state == number_sm::N_ST_STATES) {
            *end = index;
            return true;
        }
    } while (state != INVALID);
    return false;
}

static bool is_true_start(char ch) { return ch == 't'; }
static bool is_false_start(char ch) { return ch == 'f'; }
static bool is_null_start(char ch) { return ch == 'n'; }
static bool true_scan(const char *str, size_type index, size_type *end) {
    static char str_true[] = STR_TRUE;
    for (size_type i = 0; i < STR_SIZE_TRUE; ++i, ++index) {
        if (str[index] != str_true[i]) { return false; }
    }
    *end = index;
    return true;
}
static bool false_scan(const char *str, size_type index, size_type *end) {
    static char str_false[] = STR_FALSE;
    for (size_type i = 0; i < STR_SIZE_FALSE; ++i, ++index) {
        if (str[index] != str_false[i]) { return false; }
    }
    *end = index;
    return true;
}
static bool null_scan(const char *str, size_type index, size_type *end) {
    static char str_null[] = STR_NULL;
    for (size_type i = 0; i < STR_SIZE_NULL; ++i, ++index) {
        if (str[index] != str_null[i]) { return false; }
    }
    *end = index;
    return true;
}

static bool is_string_start(char ch) { return ch == '\"'; }
static bool is_hexadecimal(char ch)
{ return is_digit(ch) || ('a' <= ch && ch <= 'f') || ('A' <= ch && ch <= 'F'); }
static bool is_valid_escape(char ch) {
    return ch == '\"' || ch == '\\' || ch == '/' ||
        ch == 'b' || ch == 'f' || ch == 'n' ||
        ch == 'r' || ch == 't';
}
namespace string_sm {
    typedef state_t (*fstate_t)(char ch);
    enum string_state : state_t {
        S_ST_START = 0,
        S_ST_CHAR = 1,
        S_ST_ESCAPE = 2,
        S_ST_HEX_0 = 3,
        S_ST_HEX_1 = 4,
        S_ST_HEX_2 = 5,
        S_ST_HEX_3 = 6,
        S_ST_STATES = 7
    };
    static state_t n_st_start(char ch) {
        if (is_string_start(ch)) { return S_ST_CHAR; }
        return INVALID;
    }
    static state_t n_st_char(char ch) {
        if (ch == '\\') { return S_ST_ESCAPE; }
        if (is_string_start(ch)) { return S_ST_STATES; }
        if (32 <= ch && ch <= 126) { return S_ST_CHAR; }
        return INVALID;
    }
    static state_t n_st_escape(char ch) {
        if (is_valid_escape(ch)) { return S_ST_CHAR; }
        if (ch == 'u') { return S_ST_HEX_0; }
        return INVALID;
    }
    static state_t n_st_hex_0(char ch) {
        if (is_hexadecimal(ch)) { return S_ST_HEX_1; }
        return INVALID;
    }
    static state_t n_st_hex_1(char ch) {
        if (is_hexadecimal(ch)) { return S_ST_HEX_2; }
        return INVALID;
    }
    static state_t n_st_hex_2(char ch) {
        if (is_hexadecimal(ch)) { return S_ST_HEX_3; }
        return INVALID;
    }
    static state_t n_st_hex_3(char ch) {
        if (is_hexadecimal(ch)) { return S_ST_CHAR; }
        return INVALID;
    }
    static fstate_t transition[S_ST_STATES] = {
        n_st_start, n_st_char, n_st_escape,
        n_st_hex_0, n_st_hex_1, n_st_hex_2, n_st_hex_3
    };
}
static bool string_scan(const char *str, size_type index, size_type *end) {
    state_t state = string_sm::S_ST_START;
    do {
        state = string_sm::transition[state](str[index]);
        ++index;
        if (state == string_sm::S_ST_STATES) {
            *end = index;
            return true;
        }
    } while (state != INVALID);
    return false;
}
