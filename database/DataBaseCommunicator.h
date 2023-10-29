#pragma once
#include <sqlite3.h>

#include <string>
#include <vector>

namespace am::database
{
    class DataBaseCommunicator
    {
    public:
        explicit DataBaseCommunicator(const char *database_name = "results.db");

        ~DataBaseCommunicator();

        bool add_results_table();

        bool insert_records2results(const std::vector<std::string> &records);

        bool print_all_colected_results();

        sqlite3 *get_database_ptr() const;

    private:
        char *zErrMsg;
        sqlite3 *database;
    };

} // namespace am::database
