#include "files/pdb/Records.hpp"
using namespace mdk;
using namespace std;

Record::Record() {
    tag = END;
    end = End();
}

Record::~Record() {
    switch (tag) {
        case ATOM: { atom.~Atom(); break; }
        case SSBOND: { ssbond.~SSBond(); break; }
        case CRYST1: { cryst1.~Cryst1(); break; }
        case END: { end.~End(); break; }
    };
}

Record::Record(Record const& other) {
    tag = other.tag;
    switch (tag) {
        case ATOM: { atom = other.atom; break; }
        case SSBOND: { ssbond = other.ssbond; break; }
        case CRYST1: { cryst1 = other.cryst1; break; }
        case END: { end = other.end; break; }
    };
}

Record::Record(Record&& other) noexcept {
    tag = other.tag;
    switch (tag) {
        case ATOM: { atom = std::move(other.atom); break; }
        case SSBOND: { ssbond = std::move(other.ssbond); break; }
        case CRYST1: { cryst1 = std::move(other.cryst1); break; }
        case END: { end = other.end; break; }
    };
}

Record& Record::operator=(Record const& other) {
    tag = other.tag;
    switch (tag) {
        case ATOM: { atom = other.atom; break; }
        case SSBOND: { ssbond = other.ssbond; break; }
        case CRYST1: { cryst1 = other.cryst1; break; }
        case END: { end = other.end; break; }
    };
    return *this;
}