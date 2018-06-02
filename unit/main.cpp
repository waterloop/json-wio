#include <iostream>
#include <wlib/Json/JsonElement.h>

#define println(str) std::cout << (str) << std::endl

#define decl_je(name, type, val) json_element name(static_cast<type>(val))

using namespace wlp;

int main(int argc, char *argv[]) {
    decl_je(null_e, nullptr_t, nullptr);
    decl_je(bool_e, bool, true);
    decl_je(char_e, char, 'g');
    
    decl_je(s_char_e, signed char, -10);
    decl_je(s_short_e, short, -11);
    decl_je(s_int_e, int, -12);
    decl_je(s_long_e, long, -13);
    decl_je(s_lli_e, long long, -14);

    decl_je(u_char_e, unsigned char, 10);
    decl_je(u_short_e, unsigned short, 11);
    decl_je(u_int_e, unsigned int, 12);
    decl_je(u_long_e, unsigned long, 13);
    decl_je(u_lli_e, unsigned long long, 14);

    dynamic_string d_str("war of the worlds");
    static_string<128> s_str("your empire needs you");

    json_element c_str_e("hello my name is jeff");
    json_element d_str_e(d_str);
    json_element s_str_e(s_str);

    

}
