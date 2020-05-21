#include "DataBase.h"

namespace DB
{
    Column::Column()
        : Column_header()
        , _out_lenght(0)
        , _type(parsing_type::NoType) {}

//-----------------------------------------------------------------------------------------------------------------

    Column::Column(std::string name, size_t id, parsing_type type, size_t out_lenght)
        : Column_header(name, id)
        , _out_lenght(out_lenght)
        , _type(type) {}

//-----------------------------------------------------------------------------------------------------------------

    Column::Column(std::ifstream& in)
    {
        _type = parsing_type::NoType;
        _out_lenght = 0;
        in >> *this;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::ifstream& operator>>(std::ifstream& in, Column& header)
    {
        char* err = NULL;

        header._name = csv_format::CSV_table<>::read_before_separator(in, Column::_separator);
        header._type << csv_format::CSV_table<>::read_before_separator(in, Column::_separator);
        header._out_lenght = (size_t)strtoul(csv_format::CSV_table<>::
                                        read_before_separator(in, Column::_separator).c_str(), &err, 10);

        if (header._name.size() == 0 || *err != '\0') 
        {
            throw std::runtime_error("Can't read header");
        }

        return in;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::ofstream& operator<<(std::ofstream& out, const Column& header)
    {
        out << header._name << Column::_separator << header._type << Column::_separator << header._out_lenght;
        return out;
    }

//=================================================================================================================

    Element::Element()
        : _value(0)
        , _type(parsing_type::NoType) {}

//-----------------------------------------------------------------------------------------------------------------

    const void* Element::get() const
    {
        switch (_type)
        {
        case DB::parsing_type::UInt8:
            return &std::get<uint8_t>(_value);
        case DB::parsing_type::UInt16:
            return &std::get<uint16_t>(_value);
        case DB::parsing_type::Int32:
            return &std::get<int>(_value);
        case DB::parsing_type::Long:
            return &std::get<long>(_value);
        case DB::parsing_type::LLong:
            return &std::get<long long>(_value);
        case DB::parsing_type::UInt:
            return &std::get<unsigned int>(_value);
        case DB::parsing_type::ULong:
            return &std::get<unsigned long>(_value);
        case DB::parsing_type::ULLong:
            return &std::get<unsigned long long>(_value);
        case DB::parsing_type::Float:
            return &std::get<float>(_value);
        case DB::parsing_type::Double:
            return &std::get<double>(_value);
        case DB::parsing_type::LDouble:
            return &std::get<long double>(_value);
        case DB::parsing_type::String:
            return &std::get<std::string>(_value);
        case DB::parsing_type::Date:
            return &std::get<Time>(_value);
        default:
            break;
        }
        return nullptr;
    }

//-----------------------------------------------------------------------------------------------------------------

    DB::parsing_type Element::get_type() const
    {
        return _type;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string Element::as_string() const
    {
        std::string ans = "";
        switch (_type)
        {
        case DB::parsing_type::UInt8:
            return DB::to_string(std::get<uint8_t>(_value));
        case DB::parsing_type::UInt16:
            return DB::to_string(std::get<uint16_t>(_value));
        case DB::parsing_type::Int32:
            return DB::to_string(std::get<int>(_value));
        case DB::parsing_type::Long:
            return DB::to_string(std::get<long>(_value));
        case DB::parsing_type::LLong:
            return DB::to_string(std::get<long long>(_value));
        case DB::parsing_type::UInt:
            return DB::to_string(std::get<unsigned int>(_value));
        case DB::parsing_type::ULong:
            return DB::to_string(std::get<unsigned long>(_value));
        case DB::parsing_type::ULLong:
            return DB::to_string(std::get<unsigned long long>(_value));
        case DB::parsing_type::Float:
            return DB::to_string(std::get<float>(_value));
        case DB::parsing_type::Double:
            return DB::to_string(std::get<double>(_value));
        case DB::parsing_type::LDouble:
            return DB::to_string(std::get<long double>(_value));
        case DB::parsing_type::String:
            return std::get<std::string>(_value);
        case DB::parsing_type::Date:
            return DB::to_string(std::get<DB::Time>(_value));
        default:
            break;
        }
        return "";
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Element::operator<(const Element& el) const
    {
        if (_type != el._type)
        {
            throw std::invalid_argument("Different type of element");
        }
        return _value < el._value;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Element::operator<=(const Element& el) const
    {
        return (*this < el) && (*this == el);
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Element::operator>(const Element& el) const
    {
        return !(*this <= el);
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Element::operator>=(const Element& el) const
    {
        return !(*this < el);
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Element::operator==(const Element& el) const
    {
        return (_type == el._type && _value == el._value);
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Element::operator!=(const Element& el) const
    {
        return !(*this == el);
    }

//=================================================================================================================

    DataBase::DataBase(char separator)
        : csv_format::CSV_table<Element, Column>(separator) {}

//-----------------------------------------------------------------------------------------------------------------

    DataBase::DataBase(std::ifstream& in, char separator)
        : csv_format::CSV_table<Element, Column>(in, separator) {}

//-----------------------------------------------------------------------------------------------------------------

    void DataBase::sort(std::string column, void(*sort)(std::vector<Row>::iterator first, std::vector<Row>::iterator last,
        bool(*cmp)(const Row& row1, const Row& row2)))
    {
        if (column == "") 
        {
            _sort_id = 0;
        } 
        else
        {
            if (_columns_header.find(column) == _columns_header.end())
            {
                throw std::invalid_argument("This name of column not exist");
            }
            _sort_id = _columns_header[column]._id;
        }

        sort(_date.begin(), _date.end(), DataBase::comparator);
    }

//-----------------------------------------------------------------------------------------------------------------

    void DataBase::print_divide_line(std::ostream& out) const
    {
        out << std::setfill('-');
        for (auto column : _columns_header)
        {
            out << "+" << std::setw(column.second._out_lenght) << "-";
        }
        out << "+\n";
        out << std::setfill(' ');
    }

//-----------------------------------------------------------------------------------------------------------------

    void DataBase::print_column(std::ostream& out) const
    {
        for (auto column : _columns_header)
        {
            out << "|" << std::setw(column.second._out_lenght) << column.second._name;
        }
        out << "|\n";
    }

//-----------------------------------------------------------------------------------------------------------------

    void DataBase::print_table(std::ostream& out) const
    {
        print_divide_line(out);
        for (size_t i = 0; i < _date.size(); ++i)
        {
            for (auto column : _columns_header)
            {
                out << "|" << std::setw(column.second._out_lenght) << _date[i][column.second._id].as_string();
            }
            out << "|\n";
            print_divide_line(out);
        }
    }

//-----------------------------------------------------------------------------------------------------------------

    bool DataBase::comparator(const Row& row1, const Row& row2)
    {
        return row1[_sort_id] < row2[_sort_id];
    }

//-----------------------------------------------------------------------------------------------------------------

    std::ostream& operator<<(std::ostream& out, const DataBase& bd)
    {
        out << "Name of Table: " << bd.get_name() << "\n";
        bd.print_divide_line(out);
        bd.print_column(out);
        bd.print_table(out);
        return out;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::ofstream& operator<<(std::ofstream& out, const DataBase& bd)
    {
        bd.write_to_file(out);
        return out;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::ifstream& operator>>(std::ifstream& in, DataBase& bd)
    {
        bd.read_from_file(in);
        return in;
    }

//-----------------------------------------------------------------------------------------------------------------

    size_t DataBase::_sort_id = 0;
}