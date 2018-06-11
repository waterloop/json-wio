#include <Cosa/Trace.hh>
#include <Cosa/UART.hh>

#include <wlib/Json/JsonElement.h>

static wlp::json_element element;

void setup() {
    uart.begin(9600);
    trace.begin(&uart);

    wlp::json_object obj = {
        "first", 1,
        "second", 2
    };
    element = move(obj);
}

void loop() {}
