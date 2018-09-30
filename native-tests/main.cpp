#include <wlib/json>
#include <wlib/memory>
#include <iostream>

static constexpr unsigned s_poolSize = 16400 * 4;
static char s_memory[s_poolSize];

using namespace wlp;

static void doSomething(int b) {
}

int main(void) {
    if (!mem::init(s_memory, s_poolSize)) {
        std::cout << "Failed to init memory" << std::endl;
    }

    json_object payload = {
        "board", "mega2560",
        "month", "jan",
        "day", 25,
        "year", 1998
    };

    json_element element = move(payload);

    char buffer[512];
    json::stringify(buffer, element);

    std::cout << buffer << std::endl;
}
