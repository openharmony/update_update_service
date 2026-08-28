#include "parse_content_len.h"
#include <cstdint>
#include <cassert>
#include <iostream>
int main() {
    int64_t v = 7;
    assert(!ParseContentLen("", v) && v == 7);
    assert(!ParseContentLen("abc", v));
    assert(!ParseContentLen("9999999999999999999", v));
    assert(ParseContentLen("0", v) && v == 0);
    assert(ParseContentLen("1024", v) && v == 1024);
    std::cout << "ok" << std::endl;
    return 0;
}
