#include "utils.h"

using namespace nana;

///
std::string sformat(const std::string & fmt_str, ...) {
    va_list args_initial;
    va_start(args_initial, fmt_str);
    size_t length = vsnprintf(NULL, 0, fmt_str.c_str(), args_initial);
    va_end(args_initial);

    std::vector<char> buffer(length + 1);
    va_start(args_initial, fmt_str);
    vsnprintf( & buffer[0], length + 1, fmt_str.c_str(), args_initial);
    va_end(args_initial);

    return & buffer[0];
}


// преобразование UTF8 to local charset in Windows
std::string to_local(std::string str) {
    return nana::charset(str, nana::unicode::utf8);
}
