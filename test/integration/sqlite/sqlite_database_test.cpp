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
#include <memory>

#include "cppdbc/sqlite/sqlite_database.hpp"

namespace cppdbc {

class SQLiteDatabaseTest : public ::testing::Test {
protected:
    const char* SQL_GET_VERSION = "SELECT SQLITE_VERSION();";
    const char* SQL_CREATE_TABLE_INT = "CREATE TABLE test(id INTEGER NOT NULL, PRIMARY KEY(id));";
    const char* SQL_INSERT_VALUE = "INSERT INTO test VALUES(?);";

    void SetUp() override;
    void TearDown() override;

    std::shared_ptr<IDatabase> database_;
};

void SQLiteDatabaseTest::SetUp() {
    database_ = std::make_shared<SQLiteDatabase>("test.db", SQLiteDatabase::SQLiteMode::CREATE);
}

void SQLiteDatabaseTest::TearDown() {
    std::remove("test.db");
    std::remove("tmp.db");
}

TEST_F(SQLiteDatabaseTest, CreateInMemoryDatabase) {
    EXPECT_NO_THROW(std::make_shared<SQLiteDatabase>("tmp.db",
            SQLiteDatabase::SQLiteMode::IN_MEMORY));
}

TEST_F(SQLiteDatabaseTest, CreatingNonExistingDatabaseInReadOnlyModeThrowsException) {
    EXPECT_THROW(std::make_shared<SQLiteDatabase>("no.db",
            SQLiteDatabase::SQLiteMode::READ_ONLY), std::invalid_argument);
}

TEST_F(SQLiteDatabaseTest, CreatingNonExistingDatabaseInReadWriteModeThrowsException) {
    EXPECT_THROW(std::make_shared<SQLiteDatabase>("no.db",
            SQLiteDatabase::SQLiteMode::READ_WRITE), std::invalid_argument);
}

TEST_F(SQLiteDatabaseTest, CreateDatabaseInCreateMode) {
    ASSERT_NO_THROW(std::make_shared<SQLiteDatabase>("tmp.db",
            SQLiteDatabase::SQLiteMode::CREATE));

    /* The database can be created anytime even if it already exists */
    EXPECT_NO_THROW(std::make_shared<SQLiteDatabase>("tmp.db",
            SQLiteDatabase::SQLiteMode::CREATE));
}

TEST_F(SQLiteDatabaseTest, CreateDatabaseInReadOnlyModeWhenExists) {
    std::make_shared<SQLiteDatabase>("tmp.db", SQLiteDatabase::SQLiteMode::CREATE);
    EXPECT_NO_THROW(std::make_shared<SQLiteDatabase>("tmp.db",
            SQLiteDatabase::SQLiteMode::READ_ONLY));
}

TEST_F(SQLiteDatabaseTest, CreateDatabaseInReadWriteModeWhenExists) {
    std::make_shared<SQLiteDatabase>("tmp.db", SQLiteDatabase::SQLiteMode::CREATE);
    EXPECT_NO_THROW(std::make_shared<SQLiteDatabase>("tmp.db",
            SQLiteDatabase::SQLiteMode::READ_WRITE));
}

TEST_F(SQLiteDatabaseTest, CreateStatementWithInvalidSQLThrowsException) {
    EXPECT_THROW(database_->createStatement("SELECT SQLITE_"),
            std::invalid_argument);
}

TEST_F(SQLiteDatabaseTest, ExecuteStatement) {
    auto statement = database_->createStatement(SQL_GET_VERSION);
    auto result = statement->execute();
    EXPECT_NE(result, nullptr);
}

TEST_F(SQLiteDatabaseTest, ExecuteStatementWithoutOutputReturnsNullptr) {
    auto statement = database_->createStatement(SQL_CREATE_TABLE_INT);
    EXPECT_EQ(statement->execute(), nullptr);
}

TEST_F(SQLiteDatabaseTest, ExecuteStatemetViolatesConstraintThrowsException) {
    auto statement = database_->createStatement(SQL_CREATE_TABLE_INT);
    statement->execute();

    statement = database_->createStatement(SQL_INSERT_VALUE);
    statement->bind(1, 0);
    statement->execute();

    statement = database_->createStatement(SQL_INSERT_VALUE);
    statement->bind(1, 0);
    EXPECT_THROW(statement->execute(), std::logic_error);
}

} // namespace cppdbc
