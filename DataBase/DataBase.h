#pragma once
#include "CSV.h"
#include "Parse.h"

namespace DB
{
	class Column : public csv_format::Column_header
	{
		parsing_type _type;
		size_t _out_lenght;

		Column() = default;
		Column(std::string name, size_t id, parsing_type type, size_t out_lenght);
		Column(std::ifstream & in);

		friend
			std::ifstream& operator>>(std::ifstream& in, Column& header);

		friend
			std::ofstream& operator<<(std::ofstream& out, Column& header);
	};

	class DataBase : public csv_format::CSV_table<Column>
	{
	public:
		DataBase();
		~DataBase();

	private:

	};
}