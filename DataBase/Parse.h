#pragma once
#include <string>
#include <ostream>
#include <variant>


namespace DB 
{
    enum class parsing_type 
    {
        NoType,
        UInt8,
        UInt16,
        Int32,
        Long,
        LLong,
        UInt,
        ULong,
        ULLong,
        Float,
        Double,
        LDouble,
        String,
        Date
    };

//=================================================================================================================   

    class Parseable 
    {
    public:
        virtual bool operator<<(const std::string& str) = 0;

        virtual bool operator>>(std::string& str) const = 0;
    };

//=================================================================================================================

    void operator<<(parsing_type& type, const std::string& str);

    bool operator<<(uint8_t& a,             const std::string& str);
    bool operator<<(uint16_t& a,            const std::string& str);
    bool operator<<(int& a,                 const std::string& str);
    bool operator<<(long& a,                const std::string& str);
    bool operator<<(long long& a,           const std::string& str);
    bool operator<<(unsigned int& a,        const std::string& str);
    bool operator<<(unsigned long& a,       const std::string& str);
    bool operator<<(unsigned long long& a,  const std::string& str);
    bool operator<<(double& a,              const std::string& str);
    bool operator<<(long double& a,         const std::string& str);
    bool operator<<(float& a,               const std::string& str);

    bool operator>>(const uint8_t& a,               std::string& str);
    bool operator>>(const uint16_t& a,              std::string& str);
    bool operator>>(const int& a,                   std::string& str);
    bool operator>>(const long& a,                  std::string& str);
    bool operator>>(const long long& a,             std::string& str);
    bool operator>>(const unsigned int& a,          std::string& str);
    bool operator>>(const unsigned long& a,         std::string& str);
    bool operator>>(const unsigned long long& a,    std::string& str);
    bool operator>>(const double& a,                std::string& str);
    bool operator>>(const long double& a,           std::string& str);
    bool operator>>(const float& a,                 std::string& str);

    std::string to_string(const uint8_t& a);
    std::string to_string(const uint16_t& a);
    std::string to_string(const int& a);
    std::string to_string(const long& a);
    std::string to_string(const long long& a);
    std::string to_string(const unsigned int& a);
    std::string to_string(const unsigned long& a);
    std::string to_string(const unsigned long long& a);
    std::string to_string(const double& a);
    std::string to_string(const long double& a);
    std::string to_string(const float& a);

    std::ostream& operator<<(std::ostream& out, const parsing_type& type);
}