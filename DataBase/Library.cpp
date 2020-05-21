#include "Library.h"

Library::Library(std::string base_date_directory)
{
    std::ifstream students(base_date_directory + "Students.csv");
    std::ifstream subjects(base_date_directory + "Subjects.csv");
    std::ifstream books(base_date_directory + "Books.csv");
    std::ifstream abonements(base_date_directory + "Abonements.csv");

    if (!students.is_open() || !subjects.is_open() || !books.is_open() || !abonements.is_open())
    {
        throw std::runtime_error("File of DataBase doesn't exist");
    }

    students >> _students;
    subjects >> _subjects;
    books >> _books;
    abonements >> _abonements;

    std::cout << _students << "\n";
    std::cout << _subjects << "\n";
    std::cout << _books << "\n";
    std::cout << _abonements << "\n";
}

std::vector<std::string> Library::get_list_of_book(std::string student_name, std::string name_subjecct)
{
    std::vector<int> bookid = get_having_bookid(get_studentid(student_name));
    
    if (bookid.size() == 0) {
        std::cout << "Sorry, " << student_name << " didn't take any books.\n";
        return std::vector<std::string>();
    }

    if (bookid[0] == INT_MIN) {
        std::cout << "Sorry, " << student_name << " doesn't exist in our base date.\n";
        return std::vector<std::string>();
    }

    int subjectid = get_subjectid(name_subjecct);

    if (subjectid == INT_MIN) {
        std::cout << "Sorry, " << name_subjecct << " doesn't exist in our base date.\n";
        return std::vector<std::string>();
    }

    _books.sort("BookID");
    std::sort(bookid.begin(), bookid.end());

    std::vector<std::string> ans;

    for (size_t i = 0, j = 0; j < bookid.size() && i < _books.height(); ++i)
    {
        if (_books("BookID", i) == bookid[j]) 
        {
            j++;
            if (_books("SubjectID", i) == subjectid) 
            {
                ans.push_back(_books("Title", i).as_string());
            }
        }
    }

    if (ans.size() == 0) {
        std::cout << "Sorry, " << student_name << " didn't take book of " << name_subjecct << ".\n";
    }

    return ans;
}

int Library::get_subjectid(std::string name)
{
    for (size_t i = 0; i < _subjects.height(); ++i)
    {
        if (_subjects("Subject", i) == name)
        {
            return *(int*)_subjects("SubjectID", i).get();
        }
    }
    return INT_MIN;
}

int Library::get_studentid(std::string name)
{
    for (size_t i = 0; i < _students.height(); ++i)
    {
        if (_students("Name", i) == name)
        {
            return *(int*)_students("StudentID", i).get();
        }
    }
    return INT_MIN;
}

std::vector<int> Library::get_having_bookid(int studentid)
{
    std::vector<int> ans;

    if (studentid == INT_MIN) 
    {
        ans.push_back(studentid);
        return ans;
    }

    for (size_t i = 0; i < _abonements.height(); ++i)
    {
        if (_abonements("StudentID", i) == studentid)
        {
            ans.push_back(*(int*)_abonements("BookID", i).get());
        }
    }
    return ans;
}
