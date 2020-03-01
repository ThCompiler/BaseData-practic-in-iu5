#include "CSV.h"

namespace csv_format
{
    char Column_header::_separator = '|';

    Column_header::Column_header(std::string name, DB::parsing_type type, size_t id)
        : _name(name)
        , _type(type)
        , _id(id) {}

    inline Column_header::Column_header(std::ifstream& in)
    {
        in >> *this;
        _id = 0;
    }

    inline std::istream& Column_header::set_separator(std::istream& in, char separator)
    {
        _separator = separator;
        return in;
    }

    inline std::ostream& Column_header::set_separator(std::ostream& out, char separator)
    {
        _separator = separator;
        return out;
    }

    std::ofstream& operator<<(std::ofstream& out, Column_header& header)
    {
        out << header._name << header._separator << header._type;
        return out;
    }
}