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
        _type = DB::parsing_type::NoType;
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

    std::ifstream& operator>>(std::ifstream& in, Column_header& header)
    {
        header._name = CSV_table<Column_header>::read_before_separator(in, Column_header::_separator);
        if ((header._type << CSV_table<Column_header>::read_before_separator(in, Column_header::_separator))
            || header._name.size() == 0)
        {
            throw;
        }
        return in;
    }

    std::ofstream& operator<<(std::ofstream& out, Column_header& header)
    {
        out << header._name << header._separator << header._type;
        return out;
    }
}