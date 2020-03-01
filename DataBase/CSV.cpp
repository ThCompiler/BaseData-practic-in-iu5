#include "CSV.h"

namespace csv_format
{
    char Column_header::_separator = '|';

    Column_header::Column_header(std::string name, size_t id)
        : _name(name)
        , _id(id) {}

    Column_header::Column_header(std::ifstream& in)
    {
        in >> *this;
        _id = 0;
    }

    void Column_header::set_separator(char separator)
    {
        _separator = separator;
    }

    std::ifstream& operator>>(std::ifstream& in, Column_header& header)
    {
        header._name = CSV_table<Column_header>::read_before_separator(in, Column_header::_separator);
        if (header._name.size() == 0)
        {
            throw;
        }
        return in;
    }

    std::ofstream& operator<<(std::ofstream& out, Column_header& header)
    {
        out << header._name;
        return out;
    }
}