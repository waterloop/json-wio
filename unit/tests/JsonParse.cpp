#include <gtest/gtest.h>

#include <wlib/wjson/JsonParse.h>
#include <wlib/wjson/JsonElement.h>

using namespace wlp;

TEST(json_parse, parse_null) {
    constexpr auto str = "null";
    json_element el = json::parse(str);
    ASSERT_EQ(TYPE_NULL, el.type());
}

TEST(json_parse, parse_true) {
    constexpr auto str = "true";
    json_element el = json::parse(str);
    ASSERT_EQ(TYPE_BOOL, el.type());
    ASSERT_EQ(true, el);
}

TEST(json_parse, parse_false) {
    constexpr auto str = "false";
    json_element el = json::parse(str);
    ASSERT_EQ(TYPE_BOOL, el.type());
    ASSERT_TRUE(false == el);
}

TEST(json_parse, parse_int) {
    constexpr auto str = "-445566778899";
    json_element el = json::parse(str);
    ASSERT_EQ(CLASS_INT, el.type() >> 4);
    ASSERT_EQ(TYPE_SIGNED_LONG_LONG, el.type());
    ASSERT_EQ(-445566778899L, el);
}

TEST(json_parse, parse_float) {
    constexpr auto str1 = "     -1234.5678e-0010   ";
    constexpr auto str2 = "/*block comment*/ 54321.23E+5   ";
    constexpr auto str3 = "//line comment\n 0.004";
    constexpr auto str4 = "5e+5";
    json_element el;

    el = json::parse(str1);
    ASSERT_EQ(TYPE_LONG_DOUBLE, el.type());
    ASSERT_DOUBLE_EQ(-1234.5678e-10, el.floating());

    el = json::parse(str2);
    ASSERT_EQ(TYPE_LONG_DOUBLE, el.type());
    ASSERT_DOUBLE_EQ(54321.23E+5, el.floating());

    el = json::parse(str3);
    ASSERT_EQ(TYPE_LONG_DOUBLE, el.type());
    ASSERT_DOUBLE_EQ(0.004, el.floating());
    
    el = json::parse(str4);
    ASSERT_EQ(TYPE_LONG_DOUBLE, el.type());
    ASSERT_DOUBLE_EQ(5e+5, el.floating());
}

TEST(json_parse, parse_string) {
    constexpr auto str = "\"START     \\r \\/   abcdefghijklmnop1235*&#@73;;\\\"sd\\\"  \\t \\n \\uff5a \\u33df END\"";
    constexpr auto expected = "START     \\r \\/   abcdefghijklmnop1235*&#@73;;\\\"sd\\\"  \\t \\n \\uff5a \\u33df END";
    json_element el = json::parse(str);
    ASSERT_STREQ(expected, el.string().c_str());
}

TEST(json_parse, empty_array) {
    constexpr auto str1 = "   [   ] ";
    constexpr auto str2 = "   []  ";
    json_element el;
    el = json::parse(str1);

    ASSERT_EQ(TYPE_JSON_ARRAY, el.type());
    ASSERT_EQ(0, el.array().size());

    el = json::parse(str2);
    ASSERT_EQ(TYPE_JSON_ARRAY, el.type());
    ASSERT_EQ(0, el.array().size());
}

TEST(json_parse, singleton_array) {
    constexpr auto str = "    [  -12345  ]   ";
    json_element el = json::parse(str);
    ASSERT_EQ(TYPE_JSON_ARRAY, el.type());
    ASSERT_EQ(1, el.size());
    ASSERT_EQ(-12345, el[0]);
}

TEST(json_parse, primitive_array) {
    constexpr auto str = "[1, 2  , 3,true,5,6,\"hello\",\"bye\"   ]";
    json_element el = json::parse(str);
    ASSERT_EQ(TYPE_JSON_ARRAY, el.type());
    ASSERT_EQ(8, el.array().size());
    ASSERT_EQ(1, el[0]);
    ASSERT_EQ(2, el[1]);
    ASSERT_EQ(3, el[2]);
    ASSERT_EQ(true, el[3]);
    ASSERT_EQ(5, el[4]);
    ASSERT_EQ(6, el[5]);
    ASSERT_EQ("hello", el[6]);
    ASSERT_EQ("bye", el[7]);
}

TEST(json_parse, array_in_array) {
    constexpr auto str = "[\"START\",[1, 2], [3, 4],[5,6],[7,8], [ [10, 20], [20, 30], [30,40] ], \"END\" ]";
    json_element el = json::parse(str);
    ASSERT_EQ(TYPE_JSON_ARRAY, el.type());
    ASSERT_EQ(7, el.size());
    auto it = el.array().begin();
    
    ASSERT_EQ(TYPE_JSON_STRING, it->type());
    ASSERT_EQ("START", *it);
    ++it;

    ASSERT_EQ(TYPE_JSON_ARRAY, it->type());
    ASSERT_EQ(2, it->size());
    ASSERT_EQ(1, (*it)[0]);
    ASSERT_EQ(2, (*it)[1]);

    ++it;
    ASSERT_EQ(TYPE_JSON_ARRAY, it->type());
    ASSERT_EQ(2, it->size());
    ASSERT_EQ(3, (*it)[0]);
    ASSERT_EQ(4, (*it)[1]);

    ++it;
    ASSERT_EQ(TYPE_JSON_ARRAY, it->type());
    ASSERT_EQ(2, it->size());
    ASSERT_EQ(5, (*it)[0]);
    ASSERT_EQ(6, (*it)[1]);

    ++it;
    ASSERT_EQ(TYPE_JSON_ARRAY, it->type());
    ASSERT_EQ(2, it->size());
    ASSERT_EQ(7, (*it)[0]);
    ASSERT_EQ(8, (*it)[1]);

    ++it;
    ASSERT_EQ(TYPE_JSON_ARRAY, it->type());
    ASSERT_EQ(3, it->size());
    
    auto sub = it->array().begin();
    ASSERT_EQ(TYPE_JSON_ARRAY, sub->type());
    ASSERT_EQ(2, sub->size());
    ASSERT_EQ(10, (*sub)[0]);
    ASSERT_EQ(20, (*sub)[1]);

    ++sub;
    ASSERT_EQ(TYPE_JSON_ARRAY, sub->type());
    ASSERT_EQ(2, sub->size());
    ASSERT_EQ(20, (*sub)[0]);
    ASSERT_EQ(30, (*sub)[1]);

    ++sub;
    ASSERT_EQ(TYPE_JSON_ARRAY, sub->type());
    ASSERT_EQ(2, sub->size());
    ASSERT_EQ(30, (*sub)[0]);
    ASSERT_EQ(40, (*sub)[1]);

    ++it;
    ASSERT_EQ(TYPE_JSON_STRING, it->type());
    ASSERT_EQ("END", *it);
}

TEST(json_parse, empty_object) {
    constexpr auto str1 = "{}";
    constexpr auto str2 = "  {  }  ";
    json_element el;
    el = json::parse(str1);
    ASSERT_EQ(TYPE_JSON_OBJECT, el.type());
    ASSERT_EQ(0, el.size());

    el = json::parse(str2);
    ASSERT_EQ(TYPE_JSON_OBJECT, el.type());
    ASSERT_EQ(0, el.size());
}

TEST(json_parse, singleton_object) {
    constexpr auto str1 = "{\"key\":-12345}";
    constexpr auto str2 = "   {   \"key\"   :      \"value\"  }";
    json_element el = json::parse(str1);

    ASSERT_EQ(TYPE_JSON_OBJECT, el.type());
    ASSERT_EQ(1, el.size());
    ASSERT_EQ(-12345, el["key"]);

    el = json::parse(str2);
    ASSERT_EQ(TYPE_JSON_OBJECT, el.type());
    ASSERT_EQ(1, el.size());
    ASSERT_EQ("value", el["key"]);
}

TEST(json_parse, primitive_object) {
    constexpr auto str = "{\"first\":1,\"second\":2,\"third\":3,\"fourth\":\"fifth\",\"sixth\":\"seventh\"}";
    json_element el = json::parse(str);

    ASSERT_EQ(TYPE_JSON_OBJECT, el.type());
    ASSERT_EQ(5, el.size());
    ASSERT_EQ(1, el["first"]);
    ASSERT_EQ(2, el["second"]);
    ASSERT_EQ(3, el["third"]);
    ASSERT_EQ("fifth", el["fourth"]);
    ASSERT_EQ("seventh", el["sixth"]);
}

TEST(json_parse, nested_object) {
    constexpr auto str = 
        "{\"hello\":{\"first\":\"last\",\"second\":\"third\",\"ninth\":[1,2,3]},"
        "\"bye\":{\"first\":\"never\",\"second\":\"always\",\"ninth\":[5,4,3]}}";
    json_element el = json::parse(str);
    ASSERT_EQ(TYPE_JSON_OBJECT, el.type());
    ASSERT_EQ(2, el.size());

    json_element &hello = el["hello"];
    json_element &bye = el["bye"];

    ASSERT_EQ(TYPE_JSON_OBJECT, hello.type());
    ASSERT_EQ(TYPE_JSON_OBJECT, bye.type());

    ASSERT_EQ("last", hello["first"]);
    ASSERT_EQ("third", hello["second"]);
    ASSERT_EQ("never", bye["first"]);
    ASSERT_EQ("always", bye["second"]);

    json_element &hello_arr = hello["ninth"];
    json_element &bye_arr = bye["ninth"];

    ASSERT_EQ(TYPE_JSON_ARRAY, hello_arr.type());
    ASSERT_EQ(TYPE_JSON_ARRAY, bye_arr.type());
    ASSERT_EQ(3, hello_arr.size());
    ASSERT_EQ(3, bye_arr.size());

    ASSERT_EQ(1, hello_arr[0]);
    ASSERT_EQ(2, hello_arr[1]);
    ASSERT_EQ(3, hello_arr[2]);
    ASSERT_EQ(5, bye_arr[0]);
    ASSERT_EQ(4, bye_arr[1]);
    ASSERT_EQ(3, bye_arr[2]);
}

TEST(json_parse, full_stack) {
    constexpr auto str =
    "[{\"counts\":[[\"one\",\"three\",\"five\"],[1,2,3,4,5],[1,2,3,4,5],"
    "[1,2,3,{\"colors\":[\"red\",\"green\",\"blue\"],\"desc\":{\"birthda"
    "te\":{\"month\":\"jan\",\"day\":25,\"year\":1998},\"lastname\":\"ni"
    "u\",\"firstname\":\"jeff\"},\"pairs\":[[false,\"zero\",0],[true,\"o"
    "ne\",1],[\"two\",2],[\"three\",3]],\"random\":[\"bye\",\"hello\",4,"
    "3,2,1]}]],\"objarr\":[{\"fire\":\"hot\"},{\"ice\":\"cold\"},{\"wate"
    "r\":\"lukewarm\"}]}]";
    json_element el = json::parse(str);
    ASSERT_EQ(1, el.size());
    ASSERT_TRUE(el.is_array());

    json_element &el0 = el[0];
    ASSERT_TRUE(el0.is_object());
    json_element &el1 = el0["counts"];
    ASSERT_TRUE(el1.is_array());
    json_element &el2 = el1[0];
    ASSERT_TRUE(el2.is_array());
    ASSERT_EQ(3, el2.size());
    ASSERT_EQ("one", el2[0]);
    ASSERT_EQ("five", el2[2]);
}
