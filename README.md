# wlib-json
JSON Library for WLib. Parse and stringify implemented without recursion, and supports type conversions.

### Install

```bash
wio pac add wlib-json
```

### Usage
```c++
using namespace wlp;

constexpr auto json_str = "{\"mem_size\":64,\"region_name\":\"devmem1\"}";
json_element element = json::parse(json_str);
if (element.is_object()) {
    printf("region_name: %s\n", element["region_name"].as<char *>());
    printf("mem_size:    %i\n", element["mem_size"].as<int>());
    
    json_array flags = {0x20, 0x32, 0x50};
    element.object().insert("flags", move(flags));
    
    static char buf[128];
    json::stringify(buf, element);
}
```
