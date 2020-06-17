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
    const char* SQL_CREATE_TABLE_TEXT = "CREATE TABLE test(id TEXT(10));";
    const char* SQL_INSERT_VALUE = "INSERT INTO test VALUES(?);";
    const char* SQL_SELECT_VALUE = "SELECT id FROM test;";

    void SetUp() override;
    void TearDown() override;

    std::shared_ptr<Database> database_;
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
    EXPECT_THROW(database_->create_statement("SELECT SQLITE_"),
            std::invalid_argument);
}

TEST_F(SQLiteDatabaseTest, ExecuteStatement) {
    auto statement = database_->create_statement(SQL_GET_VERSION);
    auto result = statement->execute();
    EXPECT_NE(result, nullptr);
}

TEST_F(SQLiteDatabaseTest, ExecuteStatementWithoutOutputReturnsNullptr) {
    auto statement = database_->create_statement(SQL_CREATE_TABLE_INT);
    EXPECT_EQ(statement->execute(), nullptr);
}

TEST_F(SQLiteDatabaseTest, ExecuteStatemetViolatesConstraintThrowsException) {
    auto statement = database_->create_statement(SQL_CREATE_TABLE_INT);
    statement->execute();

    statement = database_->create_statement(SQL_INSERT_VALUE);
    statement->bind(1, 0);
    statement->execute();

    statement = database_->create_statement(SQL_INSERT_VALUE);
    statement->bind(1, 0);
    EXPECT_THROW(statement->execute(), std::logic_error);
}

TEST_F(SQLiteDatabaseTest, GetResultSet) {
    auto statement = database_->create_statement(SQL_CREATE_TABLE_INT);
    statement->execute();

    statement = database_->create_statement(SQL_INSERT_VALUE);
    statement->bind(1, 0);
    statement->execute();

    statement = database_->create_statement(SQL_INSERT_VALUE);
    statement->bind(2, 0);
    statement->execute();

    statement = database_->create_statement(SQL_SELECT_VALUE);
    auto result = statement->execute();
    EXPECT_EQ(result->uint8(0), 1);

    ASSERT_TRUE(result->next());
    EXPECT_EQ(result->uint8(0), 2);

    EXPECT_FALSE(result->next());
    EXPECT_FALSE(result->next());
}

TEST_F(SQLiteDatabaseTest, CheckIfTableExists) {
    EXPECT_FALSE(database_->has_table("test"));

    auto statement = database_->create_statement(SQL_CREATE_TABLE_TEXT);
    statement->execute();

    EXPECT_TRUE(database_->has_table("test"));
}

TEST_F(SQLiteDatabaseTest, CreateTransaction) {
    auto transaction = database_->create_transaction();
    EXPECT_NE(transaction, nullptr);
}

TEST_F(SQLiteDatabaseTest, CommitTransaction) {
    auto statement = database_->create_statement(SQL_CREATE_TABLE_INT);
    statement->execute();

    auto transaction = database_->create_transaction();

    statement = database_->create_statement(SQL_INSERT_VALUE);
    statement->bind(1, 0);
    statement->execute();

    auto database2 = std::make_shared<SQLiteDatabase>("test.db",
            SQLiteDatabase::SQLiteMode::READ_ONLY);

    statement = database2->create_statement(SQL_SELECT_VALUE);
    ASSERT_EQ(statement->execute(), nullptr);

    ASSERT_NO_THROW(transaction->commit());

    statement = database2->create_statement(SQL_SELECT_VALUE);
    auto result = statement->execute();
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->uint8(0), 1);
}

TEST_F(SQLiteDatabaseTest, CommitTransactionThrowsExceptionWhenFailure) {
    auto statement = database_->create_statement(SQL_CREATE_TABLE_INT);
    statement->execute();

    statement = database_->create_statement(SQL_INSERT_VALUE);
    statement->bind(1, 0);
    statement->execute();

    statement = database_->create_statement(SQL_INSERT_VALUE);
    statement->bind(2, 0);
    statement->execute();

    statement = database_->create_statement(SQL_SELECT_VALUE);
    auto result = statement->execute();

    auto database2 = std::make_shared<SQLiteDatabase>("test.db",
            SQLiteDatabase::SQLiteMode::READ_WRITE);

    auto transaction = database2->create_transaction();
    statement = database2->create_statement(SQL_INSERT_VALUE);
    statement->bind(3, 0);
    statement->execute();

    // When commit a transaction while there's an open read connection, the
    // commit fails
    EXPECT_THROW(transaction->commit(), std::logic_error);
}

TEST_F(SQLiteDatabaseTest, RollbackTransaction) {
    auto statement = database_->create_statement(SQL_CREATE_TABLE_INT);
    statement->execute();

    auto transaction = database_->create_transaction();

    statement = database_->create_statement(SQL_INSERT_VALUE);
    statement->bind(1, 0);
    statement->execute();

    ASSERT_NO_THROW(transaction->rollback());

    statement = database_->create_statement(SQL_SELECT_VALUE);
    EXPECT_EQ(statement->execute(), nullptr);
}

TEST_F(SQLiteDatabaseTest, RollbackNonCommittedTransactionOnDestructor) {
    auto statement = database_->create_statement(SQL_CREATE_TABLE_INT);
    statement->execute();

    {
        auto transaction = database_->create_transaction();

        statement = database_->create_statement(SQL_INSERT_VALUE);
        statement->bind(1, 0);
        statement->execute();
    }

    auto database2 = std::make_shared<SQLiteDatabase>("test.db",
            SQLiteDatabase::SQLiteMode::READ_ONLY);

    statement = database2->create_statement(SQL_SELECT_VALUE);
    EXPECT_EQ(statement->execute(), nullptr);
}

TEST_F(SQLiteDatabaseTest, CommitTransactionMoreThanOnceThrowsException) {
    auto statement = database_->create_statement(SQL_CREATE_TABLE_INT);
    statement->execute();

    auto transaction = database_->create_transaction();

    statement = database_->create_statement(SQL_INSERT_VALUE);
    statement->bind(1, 0);
    statement->execute();

    transaction->commit();
    ASSERT_THROW(transaction->commit(), std::logic_error);
}

TEST_F(SQLiteDatabaseTest, RollbackTransactionMoreThanOnceThrowsException) {
    auto statement = database_->create_statement(SQL_CREATE_TABLE_INT);
    statement->execute();

    auto transaction = database_->create_transaction();

    statement = database_->create_statement(SQL_INSERT_VALUE);
    statement->bind(1, 0);
    statement->execute();

    transaction->rollback();
    ASSERT_THROW(transaction->rollback(), std::logic_error);
}

TEST_F(SQLiteDatabaseTest, ExecuteStatementWhileOtherTransactionIsOpenedThrowsException) {
    auto statement = database_->create_statement(SQL_CREATE_TABLE_INT);
    statement->execute();

    auto database2 = std::make_shared<SQLiteDatabase>("test.db",
            SQLiteDatabase::SQLiteMode::READ_WRITE);

    auto transaction = database2->create_transaction();

    statement = database2->create_statement(SQL_INSERT_VALUE);
    statement->bind(1, 0);
    statement->execute();

    statement = database_->create_statement(SQL_INSERT_VALUE);
    statement->bind(2, 0);
    EXPECT_THROW(statement->execute(), std::logic_error);
}

} // namespace cppdbc
