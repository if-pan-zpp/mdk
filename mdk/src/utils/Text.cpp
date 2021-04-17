#include "Text.hpp"
#include <algorithm>
using namespace mdk;
using namespace std;

string_view mdk::view(string const& s, int i, int j) {
    if (i > s.size() || j > s.size() || i > j)
        throw runtime_error("invalid view requested");

    return string_view(s.data() + i - 1, j - i + 1);
}

char mdk::view(string const& s, int i) {
    if (i > s.size())
        throw runtime_error("invalid view requested");

    return s[i - 1];
}

string_view mdk::ltrim(string_view s) {
    auto beg = s.begin();
    auto end = find_if(s.begin(), s.end(), [](unsigned char c) -> auto {
        return !std::isspace(c);
    });

    return string_view(beg, end - beg);
}

string_view mdk::rtrim(string_view s) {
    auto beg = find_if(s.rbegin(), s.rend(), [](unsigned char c) -> auto {
        return !std::isspace(c);
    }).base();
    auto end = s.end();

    return string_view(beg, end - beg);
}

string_view mdk::trim(string_view s) {
    return rtrim(ltrim(s));
}