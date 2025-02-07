//! @file
//! \brief Реализация пользовательского класса хранения даты
//===========================================================================================================

#pragma once
#include <chrono>
#include <string>
#include <iostream>
#include <vector>
#include "Parse.h"


namespace DB    /// пространство имён классов и функция для задания
{
//===============================================================================================================
//! \class Time
//! \brief Класс харнение даты
//---------------------------------------------------------------------------------------------------------------

    class Time : public Parseable 
    {
    public:
        typedef long Years;         ///< тип хранения числа лет
        typedef uint8_t Months;     ///< тип хранения числа месяцев
        typedef long long Days;     ///< тип хранения числа дней
        
    //-----------------------------------------------------------------------------------------------------------
    //! \brief Консруктор по-умолчанию
    //!
    //! \param day          число дней
    //! \param month        номер месяца
    //! \param year         год
    //! \param separator    разделитель между элементами даты
    //-----------------------------------------------------------------------------------------------------------

        Time(Days day = 10, Months month = 9, Years year = 2008,  char separator = '/');
    
    //-----------------------------------------------------------------------------------------------------------
    //! \brief Консруктор создания из строки
    //!
    //! \param str          строка с датой
    //-----------------------------------------------------------------------------------------------------------

        Time(const std::string& str);

    //-----------------------------------------------------------------------------------------------------------
    //! \brief Считает число дней от начала года
    //!
    //! \return     число дней
    //-----------------------------------------------------------------------------------------------------------

        Days days_from_begin_year();
    
    //-----------------------------------------------------------------------------------------------------------
    //! \brief Представляет доступ к числу дней
    //!
    //! \return     число дней
    //-----------------------------------------------------------------------------------------------------------

        Days get_days() const;

    //-----------------------------------------------------------------------------------------------------------
    //! \brief Представляет доступ к номеру месяца
    //!
    //! \return     номер месяца
    //-----------------------------------------------------------------------------------------------------------

        Months get_months() const;

    //-----------------------------------------------------------------------------------------------------------
    //! \brief Представляет доступ к году
    //!
    //! \return     год
    //-----------------------------------------------------------------------------------------------------------

        Years get_years() const;

    //-----------------------------------------------------------------------------------------------------------
    //! \brief  Парсинг объекта из строки
    //!
    //! \param str          строка
    //!
    //! \return     false если парсинг не удался
    //-----------------------------------------------------------------------------------------------------------

        virtual bool operator<<(const std::string& str) final;
    
    //-----------------------------------------------------------------------------------------------------------
    //! \brief  Парсинг объекта в строку
    //!
    //! \param str          строка
    //!
    //! \return     false если парсинг не удался
    //-----------------------------------------------------------------------------------------------------------

        virtual bool operator>>(std::string& str) const final;

    //-----------------------------------------------------------------------------------------------------------
    //! \brief Оператор меньше
    //!
    //! \param time         объект класса
    //!
    //! \return результат сравнения
    //-----------------------------------------------------------------------------------------------------------

        bool operator<(const Time& time) const;

    //!----------------------------------------------------------------------------------------------------------
    //! \brief Оператор меньше или равно
    //!
    //! \param time         объект класса
    //!
    //! \return результат сравнения
    //-----------------------------------------------------------------------------------------------------------

        bool operator<=(const Time& time) const;

    //!----------------------------------------------------------------------------------------------------------
    //! \brief Оператор больше
    //!
    //! \param time         объект класса
    //!
    //! \return результат сравнения
    //-----------------------------------------------------------------------------------------------------------

        bool operator>(const Time& time) const;

    //!----------------------------------------------------------------------------------------------------------
    //! \brief Оператор больше или равно
    //!
    //! \param time         объект класса
    //!
    //! \return результат сравнения
    //-----------------------------------------------------------------------------------------------------------

        bool operator>=(const Time& time) const;

    //!----------------------------------------------------------------------------------------------------------
    //! \brief Оператор равенства
    //!
    //! \param time         объект класса
    //!
    //! \return результат сравнения
    //-----------------------------------------------------------------------------------------------------------

        bool operator==(const Time& time) const;

    //!----------------------------------------------------------------------------------------------------------
    //! \brief Оператор не равенства
    //!
    //! \param time         объект класса
    //!
    //! \return результат сравнения
    //-----------------------------------------------------------------------------------------------------------

        bool operator!=(const Time& time) const;

    //-----------------------------------------------------------------------------------------------------------
    //! \brief Прибавления дней к дате
    //!
    //! \param day          число дней
    //!
    //! \return результат прибавления
    //-----------------------------------------------------------------------------------------------------------

        Time operator+(Days day) const;

    //-----------------------------------------------------------------------------------------------------------
    //! \brief Увеличения даты на число ней
    //!
    //! \param day          число дней
    //!
    //! \return результат увеличнения
    //-----------------------------------------------------------------------------------------------------------

        Time& operator+=(Days day);

    //-----------------------------------------------------------------------------------------------------------
    //! \brief Вычитание из даты некоторого числа дней
    //!
    //! \param day          число дней
    //!
    //! \return результат вычитания
    //-----------------------------------------------------------------------------------------------------------

        Time operator-(Days day) const;

    //-----------------------------------------------------------------------------------------------------------
    //! \brief Уменьшение даты на число ней
    //!
    //! \param day          число дней
    //!
    //! \return результат уменьшения
    //-----------------------------------------------------------------------------------------------------------

        Time& operator-=(Days day);

    //-----------------------------------------------------------------------------------------------------------
    //! \brief Разница между двумя датами в днях
    //!
    //! \param time         объект класса
    //! 
    //! \return разница в днях
    //-----------------------------------------------------------------------------------------------------------

        Days operator-(const Time& time) const;

    //-----------------------------------------------------------------------------------------------------------
    //! \brief Перегрузка оператора ввода
    //!
    //! \param in           поток ввода
    //! \param time         вводимый объект класса
    //-----------------------------------------------------------------------------------------------------------

        friend
            std::istream& operator>>(std::istream& in, Time& time);
        
    //-----------------------------------------------------------------------------------------------------------
    //! \brief Перегрузка оператора вывода
    //!
    //! \param out          поток вывода
    //! \param time         выводимый объект класса
    //-----------------------------------------------------------------------------------------------------------

        friend
            std::ostream& operator<<(std::ostream& out, const Time& time);

    //-----------------------------------------------------------------------------------------------------------
    //! \brief ПРоверяет является ли год високосный
    //!
    //! \param  year        год
    //!
    //! \return true если год високосный
    //-----------------------------------------------------------------------------------------------------------

        static bool is_leap_year(Years year);

    //-----------------------------------------------------------------------------------------------------------
    //! \brief Возвращает число дней в месяце
    //! Если не указывать год, он считается не високосным
    //!
    //! \param month        номер месяца
    //! \param year         год
    //!
    //! \return     число дней в месяце
    //-----------------------------------------------------------------------------------------------------------

        static Days day_in_month(Months month, Years year = 1);
    private:
        Days _day;          ///< число дней
        Years _year;        ///< год
        Months _month;      ///< номер месяца

        char _separator;    ///< разделитель между элементами даты

        static const std::vector<Days> _months; ///< число дней в каждом месяце, если год не високосный
    };

    //-----------------------------------------------------------------------------------------------------------
    //! \brief Парсинг даты в строку
    //!
    //! \param time         объект класса
    //!
    //! \return     строка с значением даты или пустая если парсинг не возможен
    //-----------------------------------------------------------------------------------------------------------

    std::string to_string(const Time& time);
}