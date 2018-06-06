#ifndef __WLIB_JSON_JSONCOMPARE_H__
#define __WLIB_JSON_JSONCOMPARE_H__

namespace wlp {

    class json_element;

    extern bool operator==(const json_element &je1, const json_element &je2);
    extern bool operator< (const json_element &je1, const json_element &je2);

    inline bool operator!=(const json_element &je1, const json_element &je2) {
        return !(je1 == je2);
    }
    inline bool operator> (const json_element &je1, const json_element &je2) {
        return je2 < je1;
    }
    inline bool operator<=(const json_element &je1, const json_element &je2) {
        return !(je1 > je2);
    }
    inline bool operator>=(const json_element &je1, const json_element &je2) {
        return !(je1 < je2);
    }

}

#endif

