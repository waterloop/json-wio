#include <Cosa/Trace.hh>
#include <Cosa/UART.hh>

#include <wlib/Json/JsonElement.h>

static json_element element(25);

void setup() {
    uart.begin(9600);
    trace.begin(&uart);
}

void loop() {}

