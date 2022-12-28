
#include "gtest/gtest.h"
#include "database/DataBaseCommunicator.h"
#include <fstream>

using namespace am::database;
static constexpr const char *ut_database = "ut_database.db";

static std::string row_comma_separated; // strore sql query result

static int callback_str(void *NotUsed, int argc, char **argv, char **azColName)
{
    row_comma_separated.clear();
    for (int i = 0; i < argc; i++)
    {
        row_comma_separated.append(argv[i] ? argv[i] : "NULL");
        row_comma_separated.push_back(',');
    }
    return 0;
}

class DataBaseCommunicatorTest : public ::testing::Test
{

protected:
    DataBaseCommunicator db;

    DataBaseCommunicatorTest() : db(ut_database) {}

    virtual void SetUp() {}

    virtual void TearDown()
    {
        std::remove(ut_database);
    }
};

TEST_F(DataBaseCommunicatorTest, AddResultsTwice)
{
    EXPECT_TRUE(db.add_results_table());
    EXPECT_FALSE(db.add_results_table());
}

TEST_F(DataBaseCommunicatorTest, AddRecordCheck)
{
    EXPECT_FALSE(db.insert_records2results({"'file1', 'file2', '12:12:2022 22:22:22.222', 0"}));
    EXPECT_TRUE(db.add_results_table());
    EXPECT_TRUE(db.insert_records2results({"'file1', 'file2', '12:12:2022 22:22:22.222', 0"}));
    EXPECT_FALSE(db.insert_records2results({"'file1', 'file2', '12:12:2022 22:22:22.222', 0 , 0"})); // wrong count of fields
}

TEST_F(DataBaseCommunicatorTest, PrintResultsWithoutTableFailure)
{
    EXPECT_FALSE(db.print_all_colected_results());
}

TEST_F(DataBaseCommunicatorTest, ValidDataSave)
{
    EXPECT_TRUE(db.add_results_table());
    EXPECT_TRUE(db.insert_records2results({"'file1', 'file2', '12:12:2022 22:22:22.222', 0"}));

    const char *data = "DataBaseCommunicator: Callback function called";
    const char *sql = "SELECT * from RESULTS";
    char *zErrMsg;

    EXPECT_TRUE(sqlite3_exec(db.get_database_ptr(), sql, callback_str, (void *)data, &zErrMsg) == SQLITE_OK);
    ASSERT_EQ("file1,file2,12:12:2022 22:22:22.222,0,", row_comma_separated);
    EXPECT_TRUE(db.print_all_colected_results());
}
