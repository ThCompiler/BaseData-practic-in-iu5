#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <variant>


namespace csv_format
{
    enum class parsing_type {
        String,
        NoType
    };

//================================================================================================================= 

    struct Column_header
    {
        size_t _id;
        std::string _name;

        Column_header();
        Column_header(std::string name, size_t id, parsing_type tmp_type = parsing_type::NoType);
        Column_header(std::ifstream& in);

        friend
            std::ifstream& operator>>(std::ifstream& in, Column_header& header);

        friend
            std::ofstream& operator<<(std::ofstream& out, Column_header& header);

        static void set_separator(char separator = '|');

        parsing_type _type;

    protected:
        static char _separator;
    };

//================================================================================================================= 

    class TestElement
    {
    public:
        typedef std::variant<bool, std::string> types;

        TestElement();

        TestElement(parsing_type type, std::string value);

        void set(parsing_type type, std::string value);

        const void* get() const;

        std::string as_string() const;

        bool operator< (const TestElement& el) const;
        bool operator<=(const TestElement& el) const;
        bool operator> (const TestElement& el) const;
        bool operator>=(const TestElement& el) const;
        bool operator==(const TestElement& el) const;
        bool operator!=(const TestElement& el) const;

    private:
        types _value;
        parsing_type _type;
    };

//=================================================================================================================

    template<class Elements = TestElement, class Column_Header = Column_header>
    class CSV_table
    {
    public:
        typedef std::unordered_map<std::string, Column_Header> Header;
        typedef std::vector<Elements> Row;

        CSV_table(char separator = '|');
        CSV_table(std::ifstream& in, char separator = '|');

        virtual Header get_header() const;
        virtual std::string get_key() const;
        virtual std::string get_name() const;
        virtual const std::vector<Row>& get_table() const;

        void push_row(const Row& row);
        virtual void set_header(Header& header);

        virtual void delete_row(size_t id);
        virtual void delete_column(std::string name);

        virtual Row& operator[](size_t id);

        virtual const Row& operator[](size_t id) const;

        virtual Elements& operator()(std::string attribute, size_t i);

        virtual const Elements& operator()(std::string attribute, size_t i) const;

        size_t height() const;

        size_t width() const;

        template<class Elements, class Column_Header>
        friend
            std::ifstream& operator>>(std::ifstream& in, CSV_table<Elements, Column_Header>& table);

        template<class Elements, class Column_Header>
        friend
            std::ofstream& operator<<(std::ofstream& out, const CSV_table<Elements, Column_Header>& table);

        static std::string read_before_separator(std::ifstream& in, char separator = '|');

    protected:
        Header _columns_header;
        std::vector<Row> _date;

        virtual void write_to_file(std::ofstream& out) const;

        virtual void read_from_file(std::ifstream& in);

        virtual void reparsing();

    private:
        std::string _name;
        std::string _key;

        char _separator;
    };

//=================================================================================================================

    template<class Elements, class Column_Header>
    inline CSV_table<Elements, Column_Header>::CSV_table(char separator)
        : _separator(separator) {}

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline CSV_table<Elements, Column_Header>::CSV_table(std::ifstream& in, char separator)
        : _separator(separator)
    {
        in >> *this;
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline typename CSV_table<Elements, Column_Header>::Header CSV_table<Elements, Column_Header>::get_header() const
    {
        return _columns_header;
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline std::string CSV_table<Elements, Column_Header>::get_key() const
    {
        return _key;
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline std::string CSV_table<Elements, Column_Header>::get_name() const
    {
        return _name;
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline const std::vector<typename CSV_table<Elements, Column_Header>::Row>& CSV_table<Elements, Column_Header>::get_table() const
    {
        return _date;
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline void CSV_table<Elements, Column_Header>::push_row(const Row& row)
    {
        _date.push_back(row);
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline void CSV_table<Elements, Column_Header>::set_header(CSV_table<Elements, Column_Header>::Header& header)
    {
        _columns_header = header;
        reparsing();
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline void CSV_table<Elements, Column_Header>::delete_row(size_t id)
    {
        if (id >= _date.size())
        {
            return;
        }

        _date.erase(_date.begin() + (const int)id);
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline void CSV_table<Elements, Column_Header>::delete_column(std::string name)
    {
        if (_columns_header.find(name) == _columns_header.end())
        {
            return;
        }

        size_t id = _columns_header[name]._id;
        for (size_t i = 0; i < _date.size(); ++i)
        {
            _date[i].erase(_date[i].begin() + (const int)id);
        }
        _columns_header.erase(name);
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline typename CSV_table<Elements, Column_Header>::Row& CSV_table<Elements, Column_Header>::operator[](size_t id)
    {
        if (id >= _date.size())
        {
            throw std::out_of_range("End of table");
        }
        return _date[id];
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline const typename CSV_table<Elements, Column_Header>::Row& CSV_table<Elements, Column_Header>::operator[](size_t id) const
    {
        if (id >= _date.size())
        {
            throw std::out_of_range("End of table");
        }
        return _date[id];
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline Elements& CSV_table<Elements, Column_Header>::operator()(std::string attribute, size_t i)
    {
        if (i >= _date.size() || _columns_header.find(attribute) == _columns_header.end())
        {
            throw std::out_of_range("End of table");
        }
        return _date[i][_columns_header[attribute]._id];
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline const Elements& CSV_table<Elements, Column_Header>::operator()(std::string attribute, size_t i) const
    {
        Header tmp = _columns_header;
        if (i >= _date.size() || _columns_header.find(attribute) == _columns_header.end())
        {
            throw std::out_of_range("End of table");
        }
        return _date[i][tmp[attribute]._id];
    }


//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline size_t CSV_table<Elements, Column_Header>::height() const
    {
        return _date.size();
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline size_t CSV_table<Elements, Column_Header>::width() const
    {
        return _columns_header.size();
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline std::string CSV_table<Elements, Column_Header>::read_before_separator(std::ifstream& in, char separator)
    {
        char tmp = '\0';
        bool open = false;
        std::string str = "";
        while (in.get(tmp))
        {
            if (tmp == '\"' && str.size() == 0)
            {
                str += tmp;
                open = true;
                continue;
            }

            if (tmp == separator || tmp == '\n')
            {
                if ((isblank(tmp) || tmp == '\n') && str.size() == 0)
                {
                    continue;
                }

                if (!open)
                {
                    if (tmp == '\n') {
                        in.putback(tmp);
                    }
                    return str;
                }

                for (long i = (long)str.size() - 1; i >= 0; --i)
                {
                    if (!isblank(str[i]) && str[i] != '\"')
                    {
                        break;
                    }
                    if (str[i] == '\"') {
                        for (long j = i + 1; j < (long)str.size(); str.pop_back(), ++j) {}

                        if (tmp == '\n')
                        {
                            in.putback(tmp);
                        }
                        return str;
                    }
                }
            }

            str += tmp;
        }

        return str;
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline void CSV_table<Elements, Column_Header>::write_to_file(std::ofstream& out) const
    {
        out << _name << _separator << _key << "\n";

        size_t k = 0;
        std::vector<size_t> columns_id(_columns_header.size());

        for (auto column : _columns_header)
        {
            out << column.second;
            columns_id[k++] = column.second._id;

            if (k != _columns_header.size())
            {
                out << _separator;
            }
        }
        out << "\n";

        for (size_t i = 0; i < _date.size(); ++i)
        {
            for (size_t j = 0; j < _date[i].size(); ++j)
            {
                out << _date[i][columns_id[j]].as_string();
                if (j + 1 != columns_id.size())
                {
                    out << _separator;
                }
            }
            out << "\n";
        }
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline void CSV_table<Elements, Column_Header>::read_from_file(std::ifstream& in)
    {
        _name = CSV_table<>::read_before_separator(in, _separator);
        _key = CSV_table<>::read_before_separator(in, _separator);

        char ch = '\0';
        size_t id = 0;
        Column_Header::set_separator(_separator);
        while (ch != '\n')
        {
            Column_Header tmp(in);
            tmp._id = id++;

            _columns_header[tmp._name] = tmp;

            in.get(ch);
            if (ch != '\n')
            {
                in.putback(ch);
            }
        }
        typename CSV_table<Elements, Column_Header>::Row tmp(_columns_header.size());
        while (in.get(ch)) {
            in.putback(ch);

            for (size_t i = 0; i < tmp.size(); ++i) {
                std::string tmp_str = CSV_table<>::read_before_separator(in, _separator);

                if (tmp_str.size() == 0)
                {
                    return;
                }

                for (auto column : _columns_header)
                {
                    if (column.second._id == i)
                    {
                        tmp[i].set(column.second._type, tmp_str);
                        break;
                    }
                }
            }

            _date.push_back(tmp);
        }

    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements, class Column_Header>
    inline void CSV_table<Elements, Column_Header>::reparsing()
    {
        for (auto column : _columns_header) {
            for (size_t i = 0; i < _date.size(); ++i)
            {
                _date[i][column.second._id].set(column.second._type, _date[i][column.second._id].as_string());
            }
        }
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements = TestElement, class Column_Header = Column_header>
    std::ifstream& operator>>(std::ifstream& in, CSV_table<Elements, Column_Header>& table)
    {
        table.read_from_file(in);
        return in;
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class Elements = TestElement, class Column_Header = Column_header>
    std::ofstream& operator<<(std::ofstream& out, const CSV_table<Elements, Column_Header>& table)
    {
        table.write_to_file(out);
        return out;
    }
}