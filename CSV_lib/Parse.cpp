#include "Parse.h"

namespace DB {
    void operator<<(parsing_type& type, const std::string& str)
    {
        if (str == "UInt8") 
        {
            type = parsing_type::UInt8;
            return;
        }
        if (str == "UInt16") 
        {
            type = parsing_type::UInt16;
            return;
        }
        if (str == "Int32") 
        {
            type = parsing_type::Int32;
            return;
        }
        if (str == "Long")
        {
            type = parsing_type::Long;
            return;
        }
        if (str == "LLong")
        {
            type = parsing_type::LLong;
            return;
        }
        if (str == "UInt") 
        {
            type = parsing_type::UInt;
            return;
        }
        if (str == "ULong") 
        {
            type = parsing_type::ULong;
            return;
        }
        if (str == "ULLong")
        {
            type = parsing_type::ULLong;
            return;
        }
        if (str == "FLoat") 
        {
            type = parsing_type::Float;
            return;
        }
        if (str == "Double")
        {
            type = parsing_type::Double;
            return;
        }
        if (str == "LDouble")
        {
            type = parsing_type::LDouble;
            return;
        }
        if (str == "String") 
        {
            type = parsing_type::String;
            return;
        }
        if (str == "Date") 
        {
            type = parsing_type::Date;
            return;
        }
        type = parsing_type::NoType;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator<<(uint8_t& a, const std::string& str)
    {
        char* end = NULL;
        a = (uint8_t)strtoul(str.c_str(), &end, 10);

        if (*end != '\0')
        {
            a = 0;
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator<<(uint16_t& a, const std::string& str)
    {
        char* end = NULL;
        a = (uint16_t)strtoul(str.c_str(), &end, 10);

        if (*end != '\0')
        {
            a = 0;
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator<<(int& a, const std::string& str)
    {
        char* end = NULL;
        a = strtol(str.c_str(), &end, 10);

        if (*end != '\0')
        {
            a = 0;
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator<<(long& a, const std::string& str)
    {
        char* end = NULL;
        a = strtol(str.c_str(), &end, 10);

        if (*end != '\0')
        {
            a = 0;
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator<<(long long& a, const std::string& str)
    {
        char* end = NULL;
        a = strtoll(str.c_str(), &end, 10);

        if (*end != '\0')
        {
            a = 0;
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator<<(unsigned int& a, const std::string& str)
    {
        char* end = NULL;
        a = strtoul(str.c_str(), &end, 10);

        if (*end != '\0')
        {
            a = 0;
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator<<(unsigned long& a, const std::string& str)
    {
        char* end = NULL;
        a = strtoul(str.c_str(), &end, 10);

        if (*end != '\0')
        {
            a = 0;
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator<<(unsigned long long& a, const std::string& str)
    {
        char* end = NULL;
        a = strtoull(str.c_str(), &end, 10);

        if (*end != '\0')
        {
            a = 0;
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator<<(double& a, const std::string& str)
    {
        char* end = NULL;
        a = strtod(str.c_str(), &end);

        if (*end != '\0')
        {
            a = 0;
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator<<(long double& a, const std::string& str)
    {
        char* end = NULL;
        a = strtold(str.c_str(), &end);

        if (*end != '\0')
        {
            a = 0;
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator<<(float& a, const std::string& str)
    {
        char* end = NULL;
        a = strtof(str.c_str(), &end);

        if (*end != '\0')
        {
            a = 0;
            return false;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator>>(const uint8_t& a, std::string& str)
    {
        char res[256];
        if (sprintf_s(res, "%hhu", a) < 0)
        {
            str = "";
            return false;
        }
        str = res;
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator>>(const uint16_t& a, std::string& str)
    {
        char res[256];
        if (sprintf_s(res, "%u", a) < 0)
        {
            str = "";
            return false;
        }
        str = res;
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator>>(const int& a, std::string& str)
    {
        char res[256];
        if(sprintf_s(res, "%d", a) < 0)
        {
            str = "";
            return false;
        }
        str = res;
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator>>(const long& a, std::string& str)
    {
        char res[256];
        if (sprintf_s(res, "%ld", a) < 0)
        {
            str = "";
            return false;
        }
        str = res;
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator>>(const long long& a, std::string& str)
    {
        char res[256];
        if (sprintf_s(res, "%lld", a) < 0)
        {
            str = "";
            return false;
        }
        str = res;
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator>>(const unsigned int& a, std::string& str)
    {
        char res[256];
        if (sprintf_s(res, "%u", a) < 0)
        {
            str = "";
            return false;
        }
        str = res;
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator>>(const unsigned long& a, std::string& str)
    {
        char res[256];
        if (sprintf_s(res, "%lu", a) < 0)
        {
            str = "";
            return false;
        }
        str = res;
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator>>(const unsigned long long& a, std::string& str)
    {
        char res[256];
        if (sprintf_s(res, "%llu", a) < 0)
        {
            str = "";
            return false;
        }
        str = res;
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator>>(const double& a, std::string& str)
    {
        char res[256];
        if (sprintf_s(res, "%f", a) < 0)
        {
            str = "";
            return false;
        }
        str = res;
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator>>(const long double& a, std::string& str)
    {
        char res[256];
        if (sprintf_s(res, "%Lf", a) < 0)
        {
            str = "";
            return false;
        }
        str = res;
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    bool operator>>(const float& a, std::string& str)
    {
        char res[256];
        if (sprintf_s(res, "%f", a) < 0)
        {
            str = "";
            return false;
        }
        str = res;
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string to_string(const uint8_t& a)
    {
        std::string str;
        if (!(a >> str))
        {
            throw std::runtime_error("Can't convert to string");
        }
        return str;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string to_string(const uint16_t& a)
    {
        std::string str;
        if (!(a >> str))
        {
            throw std::runtime_error("Can't convert to string");
        }
        return str;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string to_string(const int& a)
    {
        std::string str;
        if (!(a >> str))
        {
            throw std::runtime_error("Can't convert to string");
        }
        return str;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string to_string(const long& a)
    {
        std::string str;
        if (!(a >> str))
        {
            throw std::runtime_error("Can't convert to string");
        }
        return str;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string to_string(const long long& a)
    {
        std::string str;
        if (!(a >> str))
        {
            throw std::runtime_error("Can't convert to string");
        }
        return str;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string to_string(const unsigned int& a)
    {
        std::string str;
        if (!(a >> str))
        {
            throw std::runtime_error("Can't convert to string");
        }
        return str;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string to_string(const unsigned long& a)
    {
        std::string str;
        if (!(a >> str))
        {
            throw std::runtime_error("Can't convert to string");
        }
        return str;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string to_string(const unsigned long long& a)
    {
        std::string str;
        if (!(a >> str))
        {
            throw std::runtime_error("Can't convert to string");
        }
        return str;
    }

    //-----------------------------------------------------------------------------------------------------------------

    std::string to_string(const double& a)
    {
        std::string str;
        if (!(a >> str))
        {
            throw std::runtime_error("Can't convert to string");
        }
        return str;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string to_string(const long double& a)
    {
        std::string str;
        if (!(a >> str))
        {
            throw std::runtime_error("Can't convert to string");
        }
        return str;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::string to_string(const float& a)
    {
        std::string str;
        if (!(a >> str))
        {
            throw std::runtime_error("Can't convert to string");
        }
        return str;
    }

//-----------------------------------------------------------------------------------------------------------------

    std::ostream& operator<<(std::ostream& out, const parsing_type& type)
    {
        switch (type)
        {
        case parsing_type::NoType:
            out << "NoType";
            break;
        case parsing_type::UInt8:
            out << "UInt8";
            break;
        case parsing_type::UInt16:
            out << "UInt16";
            break;
        case parsing_type::Int32:
            out << "Int32";
            break;
        case parsing_type::Long:
            out << "Long";
            break;
        case parsing_type::LLong:
            out << "LLong";
            break;
        case parsing_type::UInt:
            out << "UInt";
            break;
        case parsing_type::ULong:
            out << "ULong";
            break;
        case parsing_type::ULLong:
            out << "ULLong";
            break;
        case parsing_type::Float:
            out << "Float";
            break;
        case parsing_type::Double:
            out << "Double";
            break;
        case parsing_type::LDouble:
            out << "LDouble";
            break;
        case parsing_type::String:
            out << "String";
            break;
        case parsing_type::Date:
            out << "Int32";
            break;
        default:
            throw std::runtime_error("Unknow parsable type");
            break;
        }
        return out;
    }
}