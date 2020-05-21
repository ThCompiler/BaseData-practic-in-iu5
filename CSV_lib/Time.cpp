#include "Time.h"

namespace DB 
{
    Time::Time(Days day, Months month, Years year, char separator)
        : _year(year)
        , _month(month)
        , _day(day)
        , _separator(separator) {}

//-----------------------------------------------------------------------------------------------------------------

    Time::Time(const std::string& str)
        : Time() {
        *this << str;
    }

//-----------------------------------------------------------------------------------------------------------------

    Time::Days Time::days_from_begin_year()
    {
        Days ans = 0;
        for (Months i = 0; i < _month; ++i)
        {
            ans += day_in_month(i, _year);
        }
        ans += _day;
        return ans;
    }

//-----------------------------------------------------------------------------------------------------------------

    Time::Days Time::get_days() const
    {
        return _day;
    }

//-----------------------------------------------------------------------------------------------------------------

    Time::Months Time::get_months() const
    {
        return _month;
    }

//-----------------------------------------------------------------------------------------------------------------

    Time::Years Time::get_years() const
    {
        return _year;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Time::operator<<(const std::string& str)
    {
        std::string tmp = " ";
        tmp[0] = _separator;
        std::string format = "%lld" + tmp + "%hhu" + tmp + "%ld";
        if (sscanf_s(str.c_str(), format.c_str(), &_day, &_month, &_year) != 3) {
            _day = 10; 
            _month = 9;
            _year = 2008;
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Time::operator>>(std::string& str) const
    {
        str = "";
        str += to_string(_day) + _separator + to_string(_month) + _separator + to_string(_year);
        if (str.size() <= 2) {
            str = "";
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Time::operator<(const Time& time) const
    {
        if (_year < time._year)
        {
            return true;
        }
        if (_year == time._year)
        {
            if (_month < time._month)
            {
                return true;
            }
            if (_month == time._month)
            {
                if (_day < time._day)
                {
                    return true;
                }
            }
        }
        return false;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Time::operator<=(const Time& time) const
    {
        return (*this < time) && (*this == time);
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Time::operator>(const Time& time) const
    {
        return !(*this <= time);
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Time::operator>=(const Time& time) const
    {
        return !(*this < time);
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Time::operator==(const Time& time) const
    {
        return _year == time._year && _month == time._month && _day == time._day;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Time::operator!=(const Time& time) const
    {
        return !(*this == time);
    }

//-----------------------------------------------------------------------------------------------------------------

    Time Time::operator+(Days day) const
    {
        Time ans = *this;
        ans._day += day;
        while (ans._day > day_in_month(ans._month, ans._year)) 
        {
            ans._day -= day_in_month(ans._month, ans._year);
            ans._month++;
            ans._year += ans._month / 12;
            ans._month %= 12;
        }

        return ans;
    }

//-----------------------------------------------------------------------------------------------------------------

    Time& Time::operator+=(Days day)
    {
        *this = *this + day;
        return *this;
    }

//-----------------------------------------------------------------------------------------------------------------

    Time Time::operator-(Days day) const
    {
        Time ans = *this;
        ans._day -= day;
        while (ans._day <= 0)
        {
            ans._day += day_in_month(ans._month, ans._year);
            ans._month--;
            ans._year += ans._month / 12;
            ans._month %= 12;
        }

        return ans;
    }

//-----------------------------------------------------------------------------------------------------------------

    Time& Time::operator-=(Days day)
    {
        *this = *this - day;
        return *this;
    }

//-----------------------------------------------------------------------------------------------------------------

    Time::Days Time::operator-(const Time& time) const
    {
        Time ans = *this;
        Time tmp = time;

        if (ans < tmp)
        {
            std::swap(ans, tmp);
        }

        ans._day -= tmp._day;
        if (ans._day < 0)
        {
            if (ans._month == 0) 
            {
                ans._year--;
                ans._month = 11;
                ans._day += _months[ans._month];
            }
            else
            {
                ans._month--;
                ans._day += day_in_month(ans._month, ans._year);
            }
        }

        ans._month -= tmp._month;
        if (ans._month < 0)
        {
            ans._year--;
            ans._month += 12;
        }
        ans._year -= tmp._year;

        Days sum = ans.days_from_begin_year();
        for (Years i = __min(_year, time._year); i < __min(_year, time._year) + ans._year; ++i) {
            sum += is_leap_year(i) ? 366 : 365;
        }

        return sum;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool Time::is_leap_year(Years year)
    {
        return (year % 4 && !(year % 100));
    }

//-----------------------------------------------------------------------------------------------------------------

    Time::Days Time::day_in_month(Months month, Years year)
    {
        if (month == 1 && !is_leap_year(year)) {
            return 29;
        }
        return _months[month];
    }

//-----------------------------------------------------------------------------------------------------------------

    std::istream& operator>>(std::istream& in, Time& time)
    {
        char c = '\0';
        std::string tmp = "";
        while (c != time._separator)
        {
            if (c < '0' || c > '9')
            {
                return in;
            }
            tmp += c;
        }
        time._day << tmp;

        tmp = "";
        c = '\0';
        while (c != time._separator)
        {
            if (c < '0' || c > '9')
            {
                return in;
            }
            tmp += c;
        }
        time._month << tmp;

        tmp = "";
        c = '\0';
        while (true)
        {
            if (c < '0' || c > '9')
            {
                in.putback(c);
                break;
            }
            tmp += c;
        }
        time._year << tmp;
        return in;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::ostream& operator<<(std::ostream& out, const Time& time)
    {
        out << time._day << time._separator << time._month << time._separator << time._year;
        return out;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string to_string(const Time& time)
    {
        std::string str;
        if (!(time >> str))
        {
            throw std::runtime_error("Can't convert to string");
        }
        return str;
    }

//-----------------------------------------------------------------------------------------------------------------

    const std::vector<Time::Days> Time::_months = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
}
