#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Parse.h"

namespace csv_format 
{
    struct Column_header 
    {
        size_t _id;
        std::string _name;
        DB::parsing_type _type;

        Column_header(std::string name, DB::parsing_type type, size_t id);
        Column_header(std::ifstream& in);
        
        friend
            std::ifstream& operator>>(std::ifstream& in, Column_header& header);

        friend
            std::ofstream& operator<<(std::ofstream& out, Column_header& header);

        static std::istream& set_separator(std::istream& in, char separator = '|');

        static std::ostream& set_separator(std::ostream& out, char separator = '|');

    private:
        static char _separator;
    };

    std::ifstream& operator>>(std::ifstream& in, Column_header& header);

    std::ofstream& operator<<(std::ofstream& out, Column_header& header);

    template<class Column_Header = Column_header>
    class CSV_table 
    {
    public:
        typedef std::unordered_map<std::string, Column_Header> Header;
        typedef std::vector<std::string> Row;

        CSV_table(char separator = '|');
        CSV_table(std::ifstream& in, char separator = '|');

        Header get_header() const;
        std::string get_key() const;
        std::string get_name() const;
        std::vector<Row> get_table() const;

        void set_header(Header header);

        Row& operator[](size_t id);

        friend
            std::ifstream& operator>>(std::ifstream& in, CSV_table<Column_Header>& table);

        friend
            std::ofstream& operator<<(std::ofstream& out, const CSV_table<Column_Header>& table);

        static std::string read_before_separator(std::ifstream& in, char separator = '|');
        
    private:
        std::string _key;
        std::string _name;
        std::vector<Row> _date;
        Header _columns_header;

        char _separator;
    };

    template<class Column_Header>
    inline CSV_table<Column_Header>::CSV_table(char separator)
    {
        _separator = separator;
    }

    template<class Column_Header>
    inline CSV_table<Column_Header>::CSV_table(std::ifstream& in, char separator)
    {
        _separator = separator;
        in >> *this;
    }

    template<class Column_Header>
    inline std::unordered_map<std::string, Column_Header> CSV_table<Column_Header>::get_header() const
    {
        return _columns_header;
    }

    template<class Column_Header>
    inline std::string CSV_table<Column_Header>::get_key() const
    {
        return _key;
    }

    template<class Column_Header>
    inline std::string CSV_table<Column_Header>::get_name() const
    {
        return _name;
    }

    template<class Column_Header>
    inline std::vector<typename CSV_table<Column_Header>::Row> CSV_table<Column_Header>::get_table() const
    {
        return _date;
    }

    template<class Column_Header>
    inline void CSV_table<Column_Header>::set_header(CSV_table<Column_Header>::Header header)
    {
        _columns_header = header;
    }

    template<class Column_Header>
    inline CSV_table<>::Row& CSV_table<Column_Header>::operator[](size_t id)
    {
        if (id >= _date.size())
        {
            throw std::out_of_range("End of table");
        }
        return _date[id];
    }

    template<class Column_Header>
    inline std::string CSV_table<Column_Header>::read_before_separator(std::ifstream& in, char separator)
    {
        char tmp = '\0';
        bool open = false;
        std::string str = "";
        while (in.get(tmp)) 
        {
            if (tmp == '\"' && str.size() == 0) 
            {
                open = true;
                continue;
            }

            if (tmp == separator || tmp == '\n') 
            {
                if (!open) 
                {
                    if (tmp == '\n') {
                        in.putback(tmp);
                    }
                    return str;
                }

                for (int i = str.size() - 1; i >= 0; --i) 
                {
                    if (!isblank(str[i]) && str[i] != '\"') 
                    {
                        break;
                    }
                    if (str[i] != '\"') {
                        for (size_t j = i; j < str.size(); str.pop_back(), ++j) {}

                        if (tmp == '\n') 
                        {
                            in.putback(tmp);
                        }
                        return str;
                    }
                }
            }

            if ((isblank(tmp) || tmp == '\n') && str.size() == 0) 
            {
                continue;
            }

            str += tmp;
        }

        return str;
    }

    template<class Column_Header>
    std::ifstream& operator>>(std::ifstream& in, Column_header& header)
    {
        header._name = CSV_table<Column_Header>::read_before_separator(in, Column_header::_separator);
        if ((header._type << CSV_table<Column_Header>::read_before_separator(in, Column_header::_separator))
            || header._name.size() == 0)
        {
            throw;
        }
        return in;
    }

    template<class Column_Header>
    std::ifstream& operator>>(std::ifstream& in, CSV_table<Column_Header>& table)
    {
        table._name = CSV_table<>::read_before_separator(in, table._separator);
        table._key = CSV_table<>::read_before_separator(in, table._separator);

        char ch = '\0';
        size_t id = 0;
        in >> Column_Header::set_separator(table._separator);
        while (ch != '\n') 
        {
            Column_Header tmp(in);
            tmp._id = id++;

            table._columns_header[tmp._name] = tmp;

            in.get(ch);
            if (ch != '\n') 
            {
                in.putback(ch);
            }
        }

        while (in.get(ch)) {
            in.putback(ch);
            CSV_table<>::Row tmp(table._columns_header.size());

            for (size_t i = 0; i < tmp.size(); ++i) {
                std::string tmp_str = CSV_table<>::read_before_separator(in, table._separator);

                if (tmp_str.size() == 0) 
                {
                    return in;
                }

                tmp[i] = tmp_str;
            }

            table._date.push_back(tmp);
        }

        return in;
    }

    template<class Column_Header>
    std::ofstream& operator<<(std::ofstream& out, const CSV_table<Column_Header>& table)
    {
        out << table._name << table._separator << table._key << "\n";
        
        size_t i = 0;
        std::vector<size_t> columns_id(table._columns_header.size());

        for (auto column : table._columns_header) 
        {
            out << column;
            columns_id[i] = column._id;

            if (i + 1 != table._columns_header.size())
            {
                out << table._separator;
            }
        }   
        out << "\n";

        for (size_t i = 0; i < table._date.size(); ++i) 
        {
            for (size_t j = 0; j < table._date[i].size(); ++j) 
            {
                out << table[columns_id[i]][j];
                if (j + 1 != columns_id.size())
                {
                    out << table._separator;
                }
            }
            out << "\n";
        }

        return out;
    }
}