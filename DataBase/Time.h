#pragma once
#include <chrono>
#include <string>
#include <iostream>
#include <vector>
#include "Parse.h"


namespace DB 
{
    class Time : public Parseable 
    {
    public:
        typedef long Years;
        typedef uint8_t Months;
        typedef long long Days;
      
        Time(Days day = 10, Months month = 9, Years year = 2008,  char separator = '/');

        Time(const std::string& str);

        Days days_from_begin_year();

        Days get_days() const;
        Months get_months() const;
        Years get_years() const;

        virtual bool operator<<(const std::string& str) override;

        virtual bool operator>>(std::string& str) const override;

        bool operator<(const Time& time) const;
        bool operator<=(const Time& time) const;
        bool operator>(const Time& time) const;
        bool operator>=(const Time& time) const;
        bool operator==(const Time& time) const;
        bool operator!=(const Time& time) const;

        Time operator+(Days day) const;
        Time& operator+=(Days day);

        Time operator-(Days day) const;
        Time& operator-=(Days day);

        Days operator-(const Time& time) const;

        friend
            std::istream& operator>>(std::istream& in, Time& time);

        friend
            std::ostream& operator<<(std::ostream& out, const Time& time);

        static bool is_leap_year(Years year);

        static Days day_in_month(Months month, Years year);
    private:
        Days _day;
        Years _year;
        Months _month;

        char _separator;

        static const std::vector<Days> _months;
    };

    std::string to_string(const Time& time);
}