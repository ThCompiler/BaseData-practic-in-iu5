#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace csv_format 
{
    struct Column_header 
    {
        size_t _id;
        std::string _name;

        Column_header() = default;
        Column_header(std::string name, size_t id);
        Column_header(std::ifstream& in);
        
        friend
            std::ifstream& operator>>(std::ifstream& in, Column_header& header);

        friend
            std::ofstream& operator<<(std::ofstream& out, Column_header& header);

        static void set_separator(char separator = '|');

    private:
        static char _separator;
    };


    typedef std::vector<std::string> Row;


    template<class Column_Header = Column_header>
    class CSV_table 
    {
    public:
        typedef std::unordered_map<std::string, Column_Header> Header;

        CSV_table(char separator = '|');
        CSV_table(std::ifstream& in, char separator = '|');

        Header get_header() const;
        std::string get_key() const;
        std::string get_name() const;
        std::vector<Row> get_table() const;

        void set_header(Header header);

        Row& operator[](size_t id);

        const Row& operator[](size_t id) const;

        template<class Column_Header>
        friend
            std::ifstream& operator>>(std::ifstream& in, CSV_table<Column_Header>& table);

        template<class Column_Header>
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
    inline typename CSV_table<Column_Header>::Header CSV_table<Column_Header>::get_header() const
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
    inline std::vector<Row> CSV_table<Column_Header>::get_table() const
    {
        return _date;
    }

    template<class Column_Header>
    inline void CSV_table<Column_Header>::set_header(CSV_table<Column_Header>::Header header)
    {
        _columns_header = header;
    }

    template<class Column_Header>
    inline Row& CSV_table<Column_Header>::operator[](size_t id)
    {
        if (id >= _date.size())
        {
            throw std::out_of_range("End of table");
        }
        return _date[id];
    }

    template<class Column_Header>
    inline const Row& CSV_table<Column_Header>::operator[](size_t id) const
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

            str += tmp;
        }

        return str;
    }
    

    template<class Column_Header = Column_header>
    std::ifstream& operator>>(std::ifstream& in, CSV_table<Column_Header>& table)
    {
        table._name = CSV_table<>::read_before_separator(in, table._separator);
        table._key = CSV_table<>::read_before_separator(in, table._separator);

        char ch = '\0';
        size_t id = 0;
        Column_Header::set_separator(table._separator);
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
        Row tmp(table._columns_header.size());
        while (in.get(ch)) {
            in.putback(ch);

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

    template<class Column_Header = Column_header>
    std::ofstream& operator<<(std::ofstream& out, const CSV_table<Column_Header>& table)
    {
        out << table._name << table._separator << table._key << "\n";
        
        size_t i = 0;
        std::vector<size_t> columns_id(table._columns_header.size());

        for (auto column : table._columns_header) 
        {
            out << column.second;
            columns_id[i++] = column.second._id;

            if (i != table._columns_header.size())
            {
                out << table._separator;
            }
        }   
        out << "\n";

        for (size_t i = 0; i < table._date.size(); ++i) 
        {
            for (size_t j = 0; j < table[i].size(); ++j) 
            {
                out << table[i][columns_id[j]];
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