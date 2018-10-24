#include <wlib/json>
#include <wlib/memory>

#include <Cosa/Trace.hh>
#include <Cosa/UART.hh>

using namespace wlp;

void *operator new(decltype(sizeof(0)), void *ptr) noexcept { return ptr; }

static json_element element;
static char buffer[512];
static constexpr unsigned poolSize = 2048;
static char memory[poolSize];
static int cycle = 0;

void setup() {
    mem::init(memory, poolSize);
    uart.begin(19200);
    trace.begin(&uart);

    json_object payload = {
        "board", "mega2560",
        "month", "jan",
        "day", 25,
        "year", 1998
    };

    element = move(payload);
}

void loop() {
    element["cycle"] = cycle++;
    json::stringify(buffer, element);
    trace << buffer << endl;
    delay(500);
}
