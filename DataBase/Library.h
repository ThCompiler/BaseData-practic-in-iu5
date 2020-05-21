#pragma once
#include <fstream>
#include <vector>
#include <string>
#include "DataBase.h"

class Library
{
public:
    Library(std::string base_date_directory);

    std::vector<std::string> get_list_of_book(std::string student_name, std::string name_subjecct);

private:
    Library() = default;

    DB::DataBase _students;
    DB::DataBase _subjects;
    DB::DataBase _books;
    DB::DataBase _abonements;

    int get_subjectid(std::string name);

    int get_studentid(std::string name);

    std::vector<int> get_having_bookid(int studentid);
};