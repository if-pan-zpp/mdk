#include "utils/Text.hpp"
#include "files/pdb/ActualFields.hpp"
#include <cassert>
using namespace mdk;
using namespace std;

void Integer::read(const string &s) {
    *v = stoi((string)view(s, i, j));
}

void Integer::write(string &s) const {
    auto sv = view(s, i, j);
    snprintf((char*)sv.data(), sv.size(), "%d", *v);
}

void Real::read(const string &s) {
    *v = stod((string)view(s, i, j)) * scalar;
}

void Real::write(string &s) const {
    auto sv = view(s, i, j);
    snprintf((char*)sv.data(), sv.size(), "%*.*f", n, m, *v / scalar);
}

void String::read(const string &s) {
    if (mode & Exact) *v = view(s, i, j);
    else *v = trim(view(s, i, j));
}

void String::write(string &s) const {
    auto sv = view(s, i, j);

    const char *fmt = (mode & Right) ? "% *s" : "%-*s";
    int N = v->size();
    if (n >= 0) N = min(n, N);

    snprintf((char*)sv.data(), sv.size(), fmt, N, v->data());

    if (mode & Trim) *v = trim(*v);
}

void Char::read(const string &s) {
    *v = view(s, i);
}

void Char::write(string &s) const {
    s[i-1] = *v;
}

void SymOp::read(const string &s) {
    *v = trim(view(s, i, j));
}

void SymOp::write(string &s) const {
    auto sv = view(s, i, j);
    snprintf((char*)sv.data(), sv.size(), "%*s", (int)v->size(), v->data());
}

void Literal::read(const string &s) {
    assert(view(s, i, j) == lit);
}

void Literal::write(string &s) const {
    auto sv = view(s, i, j);
    snprintf((char*)sv.data(), sv.size(), "%*s", (int)lit.size(), lit.data());
}
