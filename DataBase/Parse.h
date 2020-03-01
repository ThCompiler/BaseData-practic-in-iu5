#pragma once
#include <string>
#include <ostream>

namespace DB {
    enum class parsing_type {
        NoType,
        Int32,
        Double,
        String,
        Date
    };

    bool operator<<(parsing_type& type, const std::string str);

    std::ostream& operator<<(std::ostream& out, const parsing_type& type);
}