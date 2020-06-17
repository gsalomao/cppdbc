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
#include "sqlite/sqlite_transaction.hpp"
#include "mock/sqlite3_mock.hpp"

using ::testing::DoAll;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::StrEq;
using ::testing::_; // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

namespace cppdbc {

class SQLiteTransactionTest : public ::testing::Test {
protected:
    void SetUp() override;

    void TearDown() override;

    sqlite3* fake_sqlite_{nullptr};

    std::shared_ptr<SQLite3Mock> mock_;
    std::shared_ptr<SQLiteTransaction> transaction_;
    std::shared_ptr<SQLiteDatabase> database_;
};

void SQLiteTransactionTest::SetUp() {
    mock_ = std::make_shared<NiceMock<SQLite3Mock>>();
    SQLite3Mock::register_mock(mock_);

    fake_sqlite_ = reinterpret_cast<sqlite3*>(new int(1));

    ON_CALL(*mock_, sqlite3_open_v2)
            .WillByDefault(DoAll(SetArgPointee<1>(fake_sqlite_), Return(SQLITE_OK)));

    database_ = std::make_shared<SQLiteDatabase>("tmp.db");
    transaction_ = std::make_shared<SQLiteTransaction>(database_);
}

void SQLiteTransactionTest::TearDown() {
    delete reinterpret_cast<int*>(fake_sqlite_);

    if (transaction_->pending()) {
        EXPECT_CALL(*mock_, sqlite3_exec);
    }

    transaction_.reset();
    database_.reset();
    SQLite3Mock::destroy();
}

TEST_F(SQLiteTransactionTest, ConstructorCallsExec) {
    EXPECT_CALL(*mock_, sqlite3_exec(_, StrEq("BEGIN TRANSACTION;"), _, _, _));

    auto transaction = std::make_shared<SQLiteTransaction>(database_);
    EXPECT_NE(transaction, nullptr);

    EXPECT_CALL(*mock_, sqlite3_exec);
}

TEST_F(SQLiteTransactionTest, ConstructorWithDatabaseThrowsException) {
    EXPECT_THROW(std::make_shared<SQLiteTransaction>(nullptr), std::invalid_argument);
}

TEST_F(SQLiteTransactionTest, MoveTransaction) {
    auto transaction1 = SQLiteTransaction(database_);
    auto transaction2{std::move(transaction1)};
    EXPECT_TRUE(transaction2.pending());

    auto transaction3 = std::make_shared<SQLiteTransaction>(database_);
    *transaction_ = std::move(*transaction3);
    EXPECT_TRUE(transaction_->pending());
}

TEST_F(SQLiteTransactionTest, MoveTransactionToItselfDoNothing) {
    auto transaction = transaction_;
    *transaction_ = std::move(*transaction);
    EXPECT_TRUE(transaction_->pending());
}

TEST_F(SQLiteTransactionTest, DestructorCallsExec) {
    auto transaction = std::make_shared<SQLiteTransaction>(database_);
    EXPECT_CALL(*mock_, sqlite3_exec(_, StrEq("ROLLBACK;"), _, _, _));
}

TEST_F(SQLiteTransactionTest, CommitTransactionCallsExec) {
    EXPECT_CALL(*mock_, sqlite3_exec(_, StrEq("COMMIT;"), _, _, _))
            .WillOnce(Return(SQLITE_OK));

    EXPECT_NO_THROW(transaction_->commit());
}

TEST_F(SQLiteTransactionTest, TransactionNotPendingAfterCommit) {
    EXPECT_TRUE(transaction_->pending());
    transaction_->commit();
    EXPECT_FALSE(transaction_->pending());
}

TEST_F(SQLiteTransactionTest, CommitNonPendingTransactionThrowsException) {
    EXPECT_NO_THROW(transaction_->commit());
    EXPECT_THROW(transaction_->commit(), std::logic_error);
}

TEST_F(SQLiteTransactionTest, CommitTransactionThrowsExceptionWhenFailure) {
    ON_CALL(*mock_, sqlite3_exec)
            .WillByDefault(Return(SQLITE_ERROR));
    EXPECT_THROW(transaction_->commit(), std::logic_error);
}

TEST_F(SQLiteTransactionTest, CommitTransactionWithInvalidDatabaseThrowsException) {
    ON_CALL(*mock_, sqlite3_open_v2)
            .WillByDefault(DoAll(SetArgPointee<1>(nullptr), Return(SQLITE_OK)));

    auto database = std::make_shared<SQLiteDatabase>("tmp.db");
    auto transaction = std::make_shared<SQLiteTransaction>(database);

    EXPECT_THROW(transaction->commit(), std::logic_error);
}

TEST_F(SQLiteTransactionTest, RollbackTransactionCallsExec) {
    EXPECT_CALL(*mock_, sqlite3_exec(_, StrEq("ROLLBACK;"), _, _, _))
            .WillOnce(Return(SQLITE_OK));

    EXPECT_NO_THROW(transaction_->rollback());
}

TEST_F(SQLiteTransactionTest, TransactionNotPendingAfterRollback) {
    EXPECT_TRUE(transaction_->pending());
    transaction_->rollback();
    EXPECT_FALSE(transaction_->pending());
}

TEST_F(SQLiteTransactionTest, RollbackNonPendingTransactionThrowsException) {
    EXPECT_NO_THROW(transaction_->rollback());
    EXPECT_THROW(transaction_->rollback(), std::logic_error);
}

TEST_F(SQLiteTransactionTest, RollbackTransactionThrowsExceptionWhenFailure) {
    ON_CALL(*mock_, sqlite3_exec)
            .WillByDefault(Return(SQLITE_ERROR));
    EXPECT_THROW(transaction_->rollback(), std::logic_error);
}

TEST_F(SQLiteTransactionTest, RollbackTransactionWithInvalidDatabaseThrowsException) {
    ON_CALL(*mock_, sqlite3_open_v2)
            .WillByDefault(DoAll(SetArgPointee<1>(nullptr), Return(SQLITE_OK)));

    auto database = std::make_shared<SQLiteDatabase>("tmp.db");
    auto transaction = std::make_shared<SQLiteTransaction>(database);

    EXPECT_THROW(transaction->rollback(), std::logic_error);
}

} // namespace cppdbc
