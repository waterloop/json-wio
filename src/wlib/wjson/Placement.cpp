void *operator new(decltype(sizeof(0)), void *ptr) noexcept { return ptr; }
