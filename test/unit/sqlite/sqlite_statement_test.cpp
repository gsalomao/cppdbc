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
#include "sqlite/sqlite_statement.hpp"
#include "mock/sqlite3_mock.hpp"

using ::testing::DoAll;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::StrEq;
using ::testing::_; // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

namespace cppdbc {

class SQLiteStatementTest : public ::testing::Test {
protected:
    static constexpr const char* SQL_QUERY = "SQL QUERY";

    void SetUp() override;

    void TearDown() override;

    template<typename T>
    void check_bind_integer(T value, uint16_t index);

    template<typename T>
    void check_bind_integer64(T value, uint16_t index);

    template<typename T>
    void check_bind_double(T value, uint16_t index);

    sqlite3* fake_sqlite_{nullptr};
    sqlite3_stmt* fake_stmt_{nullptr};

    std::shared_ptr<SQLite3Mock> mock_;
    std::shared_ptr<SQLiteStatement> statement_;
    std::shared_ptr<SQLiteDatabase> database_;
};

void SQLiteStatementTest::SetUp() {
    mock_ = std::make_shared<NiceMock<SQLite3Mock>>();
    SQLite3Mock::register_mock(mock_);

    fake_sqlite_ = reinterpret_cast<sqlite3*>(new int(1));
    fake_stmt_ = reinterpret_cast<sqlite3_stmt*>(new int(1));

    ON_CALL(*mock_, sqlite3_open_v2)
            .WillByDefault(DoAll(SetArgPointee<1>(fake_sqlite_), Return(SQLITE_OK)));

    ON_CALL(*mock_, sqlite3_prepare_v2)
            .WillByDefault(DoAll(SetArgPointee<3>(fake_stmt_), Return(SQLITE_OK)));

    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_DONE));

    database_ = std::make_shared<SQLiteDatabase>("tmp.db");
    statement_ = std::make_shared<SQLiteStatement>(database_, "SQL");
}

void SQLiteStatementTest::TearDown() {
    delete reinterpret_cast<int*>(fake_stmt_);
    delete reinterpret_cast<int*>(fake_sqlite_);

    if (statement_) {
        EXPECT_CALL(*mock_, sqlite3_finalize);
        statement_.reset();
    }

    database_.reset();
    SQLite3Mock::destroy();
}

template<typename T>
void SQLiteStatementTest::check_bind_integer(T value, uint16_t index) {
    EXPECT_CALL(*mock_, sqlite3_bind_int(fake_stmt_, (index + 1), value))
            .WillOnce(Return(SQLITE_OK));

    statement_->bind(value, index);
}

template<typename T>
void SQLiteStatementTest::check_bind_integer64(T value, uint16_t index) {
    EXPECT_CALL(*mock_, sqlite3_bind_int64(fake_stmt_, (index + 1), value))
            .WillOnce(Return(SQLITE_OK));

    statement_->bind(value, index);
}

template<typename T>
void SQLiteStatementTest::check_bind_double(T value, uint16_t index) {
    EXPECT_CALL(*mock_, sqlite3_bind_double(fake_stmt_, (index + 1), value))
            .WillOnce(Return(SQLITE_OK));

    statement_->bind(value, index);
}

TEST_F(SQLiteStatementTest, ConstructorPreparesStatement) {
    EXPECT_CALL(*mock_, sqlite3_prepare_v2(_, StrEq(SQL_QUERY), strlen(SQL_QUERY), _, _))
            .WillOnce(DoAll(SetArgPointee<3>(fake_stmt_), Return(SQLITE_OK)));

    auto statement = std::make_shared<SQLiteStatement>(database_, SQL_QUERY);
    EXPECT_NE(statement, nullptr);
}

TEST_F(SQLiteStatementTest, ConstructortWithInvalidDatabaseThrowsException) {
    EXPECT_THROW(std::make_shared<SQLiteStatement>(nullptr, SQL_QUERY),
            std::invalid_argument);
}

TEST_F(SQLiteStatementTest, ConstructorWithInvalidQueryThrowsException) {
    ON_CALL(*mock_, sqlite3_prepare_v2)
            .WillByDefault(Return(SQLITE_ERROR));

    EXPECT_THROW(std::make_shared<SQLiteStatement>(database_, SQL_QUERY),
            std::invalid_argument);
}

TEST_F(SQLiteStatementTest, MoveStatementFinilizeOldStatement) {
    {
        auto statement1 = SQLiteStatement(database_, SQL_QUERY);
        auto statement2{std::move(statement1)};
        EXPECT_TRUE(statement2.pending());
        EXPECT_CALL(*mock_, sqlite3_finalize);
    }

    auto statement1 = std::make_shared<SQLiteStatement>(database_, SQL_QUERY);
    EXPECT_CALL(*mock_, sqlite3_finalize);
    *statement_ = std::move(*statement1);

    EXPECT_FALSE(statement1->pending());
    EXPECT_TRUE(statement_->pending());
}

TEST_F(SQLiteStatementTest, MoveStatementToItselfDoNotFinilizeIt) {
    auto statement = statement_;

    EXPECT_CALL(*mock_, sqlite3_finalize)
            .Times(0);
    *statement_ = std::move(*statement);
}

TEST_F(SQLiteStatementTest, DestructorCallsSQLite) {
    EXPECT_CALL(*mock_, sqlite3_finalize(fake_stmt_));
    statement_.reset();
}

TEST_F(SQLiteStatementTest, ExecuteStatementCallsStep) {
    EXPECT_CALL(*mock_, sqlite3_step(fake_stmt_));
    statement_->execute();
}

TEST_F(SQLiteStatementTest, ExecuteStatementReturnsResultSet) {
    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_ROW));

    auto result = statement_->execute();
    EXPECT_NE(result, nullptr);
}

TEST_F(SQLiteStatementTest, ExecuteStatementReturnsNullWhenNoResult) {
    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_DONE));

    auto result = statement_->execute();
    EXPECT_EQ(result, nullptr);
}

TEST_F(SQLiteStatementTest, ExecuteStatementWhichViolatesConstraintThrowsException) {
    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_CONSTRAINT));

    EXPECT_THROW(statement_->execute(), constraint_violation);
}

TEST_F(SQLiteStatementTest, ExecuteStatementThrowsExceptionWhenFailure) {
    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_ERROR));

    EXPECT_THROW(statement_->execute(), std::logic_error);
}

TEST_F(SQLiteStatementTest, ExecuteStatemetTwiceThrowsException) {
    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_DONE));

    statement_->execute();
    EXPECT_THROW(statement_->execute(), std::logic_error);
}

TEST_F(SQLiteStatementTest, ExecuteInvalidStatemetThrowsException) {
    EXPECT_CALL(*mock_, sqlite3_prepare_v2)
            .WillOnce(DoAll(SetArgPointee<3>(nullptr), Return(SQLITE_OK)));

    auto statement = std::make_shared<SQLiteStatement>(database_, SQL_QUERY);
    EXPECT_THROW(statement->execute(), std::logic_error);
}

TEST_F(SQLiteStatementTest, CheckIfStatementIsPending) {
    ASSERT_TRUE(statement_->pending());

    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_DONE));

    statement_->execute();
    EXPECT_FALSE(statement_->pending());
}

TEST_F(SQLiteStatementTest, BindUnsignedInteger8) {
    check_bind_integer<uint8_t>(20, 2);
}

TEST_F(SQLiteStatementTest, BindUnsignedInteger16) {
    check_bind_integer<uint16_t>(50, 5);
}

TEST_F(SQLiteStatementTest, BindUnsignedInteger32) {
    check_bind_integer<uint32_t>(30, 3);
}

TEST_F(SQLiteStatementTest, BindUnsignedInteger64) {
    check_bind_integer64<uint64_t>(0, 0);
}

TEST_F(SQLiteStatementTest, BindSignedInteger8) {
    check_bind_integer<int8_t>(-20, 2);
}

TEST_F(SQLiteStatementTest, BindSignedInteger16) {
    check_bind_integer<int16_t>(-50, 5);
}

TEST_F(SQLiteStatementTest, BindSignedInteger32) {
    check_bind_integer<int32_t>(-30, 3);
}

TEST_F(SQLiteStatementTest, BindSignedInteger64) {
    check_bind_integer64<int64_t>(-2, 0);
}

TEST_F(SQLiteStatementTest, BindFloat) {
    check_bind_double<float>(1.2, 6);
}

TEST_F(SQLiteStatementTest, BindDouble) {
    check_bind_double<double>(0.59, 1);
}

TEST_F(SQLiteStatementTest, BindBool) {
    check_bind_integer<bool>(true, 1);
}

TEST_F(SQLiteStatementTest, BindText) {
    uint16_t index = 2;
    const char* value = "Test";

    EXPECT_CALL(*mock_, sqlite3_bind_text(fake_stmt_, (index + 1), StrEq(value),
            strlen(value), nullptr))
            .WillOnce(Return(SQLITE_OK));

    statement_->bind(std::string(value), index);
}

TEST_F(SQLiteStatementTest, BindBlob) {
    uint16_t index = 2;
    const char* value = "Test";

    EXPECT_CALL(*mock_, sqlite3_bind_blob(fake_stmt_, (index + 1), value,
            strlen(value), nullptr))
            .WillOnce(Return(SQLITE_OK));

    statement_->bind(value, strlen(value), index);
}

} // namespace cppdbc
