#include "DataBaseCommunicator.h"
#include <iostream>

static constexpr const char *INSERT_RESULTS_CMD = "INSERT INTO RESULTS (DATE_TIME,FILE_SRC,FILE_DST,OBECTS_FOUND) ";

static constexpr const char *CREATE_TABLE_CMD = "CREATE TABLE RESULTS("
                                                "DATE_TIME          CHAR(50),"
                                                "FILE_SRC           TEXT    NOT NULL,"
                                                "FILE_DST           TEXT    NOT NULL,"
                                                "OBECTS_FOUND       INT     NOT NULL);";

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

namespace am
{
    namespace database
    {
        DataBaseCommunicator::DataBaseCommunicator(const char *database_name) : zErrMsg(nullptr)
        {
            if (sqlite3_open(database_name, &database))
            {
                fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
                exit(0);
            }
        }

        DataBaseCommunicator::~DataBaseCommunicator()
        {
            sqlite3_close(database);
        }

        bool DataBaseCommunicator::add_results_table()
        {
            if (sqlite3_exec(database, CREATE_TABLE_CMD, callback, 0, &zErrMsg) != SQLITE_OK)
            {
                std::cout << "SQL warning:" << zErrMsg << std::endl;
                sqlite3_free(zErrMsg);
                return false;
            }
            return true;
        }

        bool DataBaseCommunicator::insert_records2results(const std::vector<std::string> &records)
        {
            std::string insert_request;
            for (auto &record : records)
            {
                insert_request.append(INSERT_RESULTS_CMD);
                insert_request.append("VALUES (");
                insert_request.append(record);
                insert_request.append(");");
                insert_request.push_back(' ');
            }
            if (sqlite3_exec(database, insert_request.c_str(), callback, 0, &zErrMsg) != SQLITE_OK)
            {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
                return false;
            }
            return true;
        }

        bool DataBaseCommunicator::print_all_colected_results()
        {
            char *data = "DataBaseCommunicator: Callback function called";
            const char *sql = "SELECT * from RESULTS";

            if (sqlite3_exec(database, sql, callback, static_cast<void *>(data), &zErrMsg) != SQLITE_OK)
            {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
                return false;
            }

            return true;
        }

        sqlite3 *DataBaseCommunicator::get_database_ptr() const
        {
            return database;
        }
    }
}
