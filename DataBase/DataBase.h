#pragma once
#include <iomanip>
#include <variant>
#include <algorithm>
#include "CSV.h"
#include "Parse.h"
#include "Time.h"


namespace DB
{
	struct Column : public csv_format::Column_header
	{
		size_t _out_lenght;
        parsing_type _type;
        
		Column();
		Column(std::string name, size_t id, parsing_type type, size_t out_lenght);
		Column(std::ifstream& in);

		friend
			std::ifstream& operator>>(std::ifstream& in, Column& header);

		friend
			std::ofstream& operator<<(std::ofstream& out, const Column& header);
	};

//=================================================================================================================

	class Element
	{
	public:
		typedef std::variant<Time, uint8_t, uint16_t, int, long, long long,
			unsigned int, unsigned long, unsigned long long, float, double, long double, std::string> types;

		Element();

		template<class T>
		Element(parsing_type type, T value);

		template<class T>
		void set(parsing_type type, T value);

        template<class T>
        void operator=(T value);

		const void* get() const;

		parsing_type get_type() const;

	    std::string as_string() const;

		bool operator< (const Element& el) const;
		bool operator<=(const Element& el) const;
		bool operator> (const Element& el) const;
		bool operator>=(const Element& el) const;
		bool operator==(const Element& el) const;
		bool operator!=(const Element& el) const;

        template<class T>
        bool operator==(const T& val) const;

	private:
		types _value;
		parsing_type _type;
	};

//=================================================================================================================

	class DataBase : public csv_format::CSV_table<Element, Column>
	{
	public:
		DataBase(char separator = '|');
		DataBase(std::ifstream& in, char separator = '|');

		void sort(std::string column = "", void (*sort)(std::vector<Row>::iterator first, std::vector<Row>::iterator last,
			bool (*cmp)(const Row& row1, const Row& row2)) = std::sort);

		friend
			std::ostream& operator<<(std::ostream& out, const DataBase& bd);

		friend
			std::ofstream& operator<<(std::ofstream& out, const DataBase& bd);

		friend
			std::ifstream& operator>>(std::ifstream& in, DataBase& bd);


	private:
		void print_divide_line(std::ostream& out) const;

		void print_column(std::ostream& out) const;

		void print_table(std::ostream& out) const;

		static size_t _sort_id;

		static bool comparator(const Row& row1, const Row& row2);
	};

//=================================================================================================================

    template<class T>
    inline Element::Element(DB::parsing_type type, T value)
        : Element()
    {
        set(type, value);
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class T>
    inline void Element::set(DB::parsing_type type, T value)
    {
        bool bad = false;
        switch (type)
        {
        case DB::parsing_type::UInt8:
            if (typeid(value) != typeid(uint8_t)) bad = true;
            break;
        case DB::parsing_type::UInt16:
            if (typeid(value) != typeid(uint16_t)) bad = true;
            break;
        case DB::parsing_type::Int32:
            if (typeid(value) != typeid(int)) bad = true;
            break;
        case DB::parsing_type::Long:
            if (typeid(value) != typeid(long)) bad = true;
            break;
        case DB::parsing_type::LLong:
            if (typeid(value) != typeid(long long)) bad = true;
            break;
        case DB::parsing_type::UInt:
            if (typeid(value) != typeid(unsigned int)) bad = true;
            break;
        case DB::parsing_type::ULong:
            if (typeid(value) != typeid(unsigned long)) bad = true;
            break;
        case DB::parsing_type::ULLong:
            if (typeid(value) != typeid(unsigned long long)) bad = true;
            break;
        case DB::parsing_type::Float:
            if (typeid(value) != typeid(float)) bad = true;
            break;
        case DB::parsing_type::Double:
            if (typeid(value) != typeid(double)) bad = true;
            break;
        case DB::parsing_type::LDouble:
            if (typeid(value) != typeid(long double)) bad = true;
            break;
        case DB::parsing_type::String:
            if (typeid(value) != typeid(std::string)) bad = true;
            break;
        case DB::parsing_type::Date:
            if (typeid(value) != typeid(Time)) bad = true;
            break;
        default:
            _type = parsing_type::NoType;
            _value = 0;
            return;
        }

        if (bad)
        {
            throw std::invalid_argument("Type of argument not equal geted type");
        }

        _type = type;
        _value = value;
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class T>
    inline void Element::operator=(T value)
    {
        set(_type, value);
    }

//-----------------------------------------------------------------------------------------------------------------

    template<class T>
    inline bool Element::operator==(const T& val) const
    {
        types tmp = val;
        return _value == tmp;
    }

//-----------------------------------------------------------------------------------------------------------------

    template<>
    inline void Element::set(DB::parsing_type type, std::string value)
    {
        _type = type;
        if (type == DB::parsing_type::UInt8)
        {
            uint8_t a = 0;
            a << value;
            _value = a;
            return;
        }
        if (type == DB::parsing_type::UInt16)
        {
            uint16_t a = 0;
            a << value;
            _value = a;
            return;
        }
        if (type == DB::parsing_type::Int32)
        {
            int a = 0;
            a << value;
            _value = a;
            return;
        }
        if (type == DB::parsing_type::Long)
        {
            long a = 0;
            a << value;
            _value = a;
            return;
        }
        if (type == DB::parsing_type::LLong)
        {
            long long a = 0;
            a << value;
            _value = a;
            return;
        }
        if (type == DB::parsing_type::UInt)
        {
            unsigned int a = 0;
            a << value;
            _value = a;
            return;
        }
        if (type == DB::parsing_type::ULong)
        {
            unsigned long a = 0;
            a << value;
            _value = a;
            return;
        }
        if (type == DB::parsing_type::ULLong)
        {
            unsigned long long a = 0;
            a << value;
            _value = a;
            return;
        }
        if (type == DB::parsing_type::Float)
        {
            float a = 0;
            a << value;
            _value = a;
            return;
        }
        if (type == DB::parsing_type::Double)
        {
            double a = 0;
            a << value;
            _value = a;
            return;
        }
        if (type == DB::parsing_type::LDouble)
        {
            long double a = 0;
            a << value;
            _value = a;
            return;
        }
        if (type == DB::parsing_type::String)
        {
            _value = value;
            return;
        }
        if (type == DB::parsing_type::Date)
        {
            DB::Time a = 0;
            a << value;
            _value = a;
            return;
        }

        _type = DB::parsing_type::NoType;
        _value = 0;
    }
}