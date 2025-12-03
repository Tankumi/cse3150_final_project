#include "Announcement.h"
#include <sstream>


Announcement Announcement::prepend(uint32_t asn) const {
    Announcement copy = *this;
    copy.ASPath.insert(copy.ASPath.begin(), asn);
    return copy;
}

std::string Announcement::path_string() const {
    std::ostringstream out;
    out << "\"(";
    for (size_t i = 0; i < ASPath.size(); i++) {
        out << ASPath[i];
        if (i + 1 < ASPath.size())
            out << ", ";
    }
    out << ")\"";
    return out.str();
}