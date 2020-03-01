#include "CSV.h"

int main() {
    csv_format::CSV_table<> a;

    std::ifstream in("Student.txt");
    in >> a;
    in.close();

    std::ofstream out("check.txt");
    out << a;
    out.close();
}