#include "Parse.h"

bool DB::operator<<(parsing_type& type, const std::string str)
{
    if (str == "Int32") {
        type = parsing_type::Int32;
        return 0;
    }
    if (str == "NoType") {
        type = parsing_type::NoType;
        return 0;
    }
    if (str == "Double") {
        type = parsing_type::Double;
        return 0;
    }
    if (str == "String") {
        type = parsing_type::String;
        return 0;
    }
    if (str == "Date") {
        type = parsing_type::Date;
        return 0;
    }
    return 1;
}

std::ostream& DB::operator<<(std::ostream& out, const parsing_type& type)
{
    switch (type)
    {
    case parsing_type::Int32:
        out << "Int32";
        break;
    case parsing_type::NoType:
        out << "NoType";
        break;
    case parsing_type::Double:
        out << "Double";
        break;
    case parsing_type::String:
        out << "String";
        break;
    case parsing_type::Date:
        out << "Int32";
        break;
    default:
        break;
    }
    return out;
}