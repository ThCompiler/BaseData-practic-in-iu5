#include "Library.h"
#include <iostream>

int main()
{
    try {
        Library lib("");

        std::string student_name, subject_name;

        std::cout << "Enter student name\n";
        std::cin >> student_name;

        std::cout << "Enter subject name\n";
        std::cin >> subject_name;

        std::vector<std::string> ans = lib.get_list_of_book(student_name, subject_name);

        if (ans.size() == 0) {
            return 0;
        }

        std::cout << "Books:\n";

        for (size_t i = 0; i < ans.size(); ++i)
        {
            std::cout << ans[i] << "\n";
        }
    }
    catch (std::exception e)
    {
        std::cout << "Sorry, " << e.what();
    }

    return 0;
}