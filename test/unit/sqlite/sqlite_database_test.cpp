/*
 * Copyright (C) 2020 Gustavo Salomao
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <gtest/gtest.h>

#include "cppdbc/sqlite/sqlite_database.hpp"
#include "mock/sqlite3_mock.hpp"

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::StrEq;
using ::testing::_; // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

namespace cppdbc {

class SQLiteDatabaseTest : public ::testing::Test {
protected:
    void SetUp() override;

    void TearDown() override;

    sqlite3* fake_sqlite_{nullptr};
    sqlite3_stmt* fake_statement_{nullptr};
    std::shared_ptr<SQLite3Mock> mock_;
    std::shared_ptr<SQLiteDatabase> database_;
};

void SQLiteDatabaseTest::SetUp() {
    mock_ = std::make_shared<NiceMock<SQLite3Mock>>();
    SQLite3Mock::register_mock(mock_);

    fake_sqlite_ = reinterpret_cast<sqlite3*>(new int(1));
    fake_statement_ = reinterpret_cast<sqlite3_stmt*>(new int(1));

    ON_CALL(*mock_, sqlite3_open_v2)
            .WillByDefault(DoAll(SetArgPointee<1>(fake_sqlite_), Return(SQLITE_OK)));

    ON_CALL(*mock_, sqlite3_prepare_v2)
            .WillByDefault(DoAll(SetArgPointee<3>(fake_statement_), Return(SQLITE_OK)));

    database_ = std::make_shared<SQLiteDatabase>("tmp.db");
}

void SQLiteDatabaseTest::TearDown() {
    delete reinterpret_cast<int*>(fake_statement_);
    delete reinterpret_cast<int*>(fake_sqlite_);

    if (database_) {
        EXPECT_CALL(*mock_, sqlite3_close);
        database_.reset();
    }

    SQLite3Mock::destroy();
}

TEST_F(SQLiteDatabaseTest, ConstructorOpenDatabaseFile) {
    EXPECT_CALL(*mock_, sqlite3_open_v2);
    std::make_shared<SQLiteDatabase>("tmp.db");
}

TEST_F(SQLiteDatabaseTest, CreateDatabaseInReadOnlyMode) {
    EXPECT_CALL(*mock_, sqlite3_open_v2(_, _, SQLITE_OPEN_READONLY, _));
    std::make_shared<SQLiteDatabase>("tmp.db",
            SQLiteDatabase::SQLiteMode::READ_ONLY);

    EXPECT_CALL(*mock_, sqlite3_open_v2(_, _, SQLITE_OPEN_READONLY, _));
    std::make_shared<SQLiteDatabase>("tmp.db");
}

TEST_F(SQLiteDatabaseTest, CreateDatabaseInReadWriteMode) {
    EXPECT_CALL(*mock_, sqlite3_open_v2(_, _, SQLITE_OPEN_READWRITE, _));
    std::make_shared<SQLiteDatabase>("tmp.db",
            SQLiteDatabase::SQLiteMode::READ_WRITE);
}

TEST_F(SQLiteDatabaseTest, CreateDatabaseInCreateMode) {
    uint32_t sqlite_flags = static_cast<uint32_t>(SQLITE_OPEN_READWRITE)
                            | static_cast<uint32_t>(SQLITE_OPEN_CREATE);

    EXPECT_CALL(*mock_, sqlite3_open_v2(_, _, sqlite_flags, _));
    std::make_shared<SQLiteDatabase>("tmp.db",
            SQLiteDatabase::SQLiteMode::CREATE);
}

TEST_F(SQLiteDatabaseTest, CreateDatabaseInMemoryMode) {
    uint32_t sqlite_flags = static_cast<uint32_t>(SQLITE_OPEN_READWRITE)
                            | static_cast<uint32_t>(SQLITE_OPEN_CREATE)
                            | static_cast<uint32_t>(SQLITE_OPEN_MEMORY);

    EXPECT_CALL(*mock_, sqlite3_open_v2(StrEq("tmp.db"), _, sqlite_flags, _));

    auto database = std::make_shared<SQLiteDatabase>("tmp.db",
            SQLiteDatabase::SQLiteMode::IN_MEMORY);

    EXPECT_NE(database, nullptr);
    EXPECT_TRUE(database->valid());
}

TEST_F(SQLiteDatabaseTest, CreateInvalidDatabaseThrowsExcption) {
    ON_CALL(*mock_, sqlite3_open_v2)
            .WillByDefault(Return(SQLITE_ERROR));

    EXPECT_CALL(*mock_, sqlite3_close);
    EXPECT_THROW(std::make_shared<SQLiteDatabase>("tmp.db"),
            std::invalid_argument);
}

TEST_F(SQLiteDatabaseTest, MoveDatabaseCloseOldDatabase) {
    {
        auto database1 = SQLiteDatabase("tmp.db");
        auto database2{std::move(database1)};
        EXPECT_TRUE(database2.valid());
        EXPECT_CALL(*mock_, sqlite3_close);
    }

    auto database1 = std::make_shared<SQLiteDatabase>("tmp.db");
    EXPECT_CALL(*mock_, sqlite3_close);
    *database_ = std::move(*database1);

    EXPECT_FALSE(database1->valid());
    EXPECT_TRUE(database_->valid());
}

TEST_F(SQLiteDatabaseTest, MoveDatabaseToItselfDoNotCloseIt) {
    auto database2 = database_;

    EXPECT_CALL(*mock_, sqlite3_close)
            .Times(0);
    *database_ = std::move(*database2);
}

TEST_F(SQLiteDatabaseTest, DestructorClosesDatabase) {
    std::shared_ptr<Database> database = std::make_shared<SQLiteDatabase>("tmp.db");
    EXPECT_CALL(*mock_, sqlite3_close);
    database.reset();
}

TEST_F(SQLiteDatabaseTest, CreateStatementFromDatabase) {
    auto database = std::make_shared<SQLiteDatabase>("tmp.db");
    auto statement = database->create_statement("SQL");
    EXPECT_NE(statement, nullptr);
}

TEST_F(SQLiteDatabaseTest, CreateStatementFromInvalidDatabaseThrowsException) {
    ON_CALL(*mock_, sqlite3_open_v2)
            .WillByDefault(DoAll(SetArgPointee<1>(nullptr), Return(SQLITE_OK)));

    auto database = std::make_shared<SQLiteDatabase>("tmp.db");
    EXPECT_THROW(database->create_statement("SQL"), std::logic_error);
}

TEST_F(SQLiteDatabaseTest, CheckIfTableExists) {
    auto database = std::make_shared<SQLiteDatabase>("tmp.db");

    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_ROW));

    ON_CALL(*mock_, sqlite3_column_int)
            .WillByDefault(Return(1));

    EXPECT_TRUE(database->has_table("test"));
}

TEST_F(SQLiteDatabaseTest, CreateTransactionFromDatabase) {
    auto database = std::make_shared<SQLiteDatabase>("tmp.db");
    auto transaction = database->create_transaction();
    EXPECT_NE(transaction, nullptr);
}

} // namespace cppdbc
