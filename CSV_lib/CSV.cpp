#include "CSV.h"

namespace csv_format
{
    char Column_header::_separator = '|';

//-----------------------------------------------------------------------------------------------------------------

    Column_header::Column_header()
        :  _id(0)
        , _name("")
        , _type(parsing_type::NoType) {}

//-----------------------------------------------------------------------------------------------------------------

    Column_header::Column_header(std::string name, size_t id, parsing_type tmp_type)
        : _id(id)
        , _name(name)
        , _type(tmp_type) {}

//-----------------------------------------------------------------------------------------------------------------

    Column_header::Column_header(std::ifstream& in)
        : Column_header() 
    {
        in >> *this;
        _id = 0;
    }

//-----------------------------------------------------------------------------------------------------------------

    void Column_header::set_separator(char separator)
    {
        _separator = separator;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::ifstream& operator>>(std::ifstream& in, Column_header& header)
    {
        header._name = CSV_table<TestElement, Column_header>::read_before_separator(in, Column_header::_separator);
        if (header._name.size() == 0)
        {
            throw std::runtime_error("Can't read header");
        }
        return in;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::ofstream& operator<<(std::ofstream& out, Column_header& header)
    {
        out << header._name;
        return out;
    }

//=================================================================================================================

    TestElement::TestElement()
        : _value(0)
        , _type(parsing_type::NoType) {}

    TestElement::TestElement(parsing_type type, std::string value)
        : _value(value)
        , _type(type) {}

    void TestElement::set(parsing_type type, std::string value)
    {
        _type = type;
        _value = value;
    }

//-----------------------------------------------------------------------------------------------------------------

    const void* TestElement::get() const
    {
        if (_type == parsing_type::String)
        {
            return &std::get<std::string>(_value);
        }
        return nullptr;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string TestElement::as_string() const
    {
        if (_type == parsing_type::String)
        {
            return std::get<std::string>(_value);
        }
        return "";
    }

//-----------------------------------------------------------------------------------------------------------------

    bool TestElement::operator<(const TestElement& el) const
    {
        return _value < el._value;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool TestElement::operator<=(const TestElement& el) const
    {
        return (*this < el) && (*this == el);
    }

//-----------------------------------------------------------------------------------------------------------------

    bool TestElement::operator>(const TestElement& el) const
    {
        return !(*this <= el);
    }

//-----------------------------------------------------------------------------------------------------------------

    bool TestElement::operator>=(const TestElement& el) const
    {
        return !(*this < el);
    }

//-----------------------------------------------------------------------------------------------------------------

    bool TestElement::operator==(const TestElement& el) const
    {
        return (_value == el._value);
    }

//-----------------------------------------------------------------------------------------------------------------

    bool TestElement::operator!=(const TestElement& el) const
    {
        return !(*this == el);
    }
}