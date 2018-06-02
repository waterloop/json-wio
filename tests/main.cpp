#include <Cosa/Trace.hh>
#include <Cosa/UART.hh>

void setup() {
    uart.begin(9600);
    trace.begin(&uart);
}

void loop() {}

