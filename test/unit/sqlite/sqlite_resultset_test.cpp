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
#include "sqlite/sqlite_resultset.hpp"
#include "sqlite/sqlite_statement.hpp"
#include "mock/sqlite3_mock.hpp"

using ::testing::DoAll;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SetArgPointee;

namespace cppdbc {

class SQLiteResultSetTest : public ::testing::Test {
protected:
    void SetUp() override;

    void TearDown() override;

    void checkGetDataType(uint16_t index, uint16_t sqliteType,
            ResultSet::DataType expectedType);

    std::shared_ptr<SQLite3Mock> mock_;

    sqlite3_stmt* fakeStmt_{nullptr};
    std::shared_ptr<SQLiteDatabase> database_;
    std::shared_ptr<ResultSet> resultSet_;
};

void SQLiteResultSetTest::SetUp() {
    mock_ = std::make_shared<NiceMock<SQLite3Mock>>();
    SQLite3Mock::registerMock(mock_);

    fakeStmt_ = reinterpret_cast<sqlite3_stmt*>(new int(1));

    ON_CALL(*mock_, sqlite3_prepare_v2)
            .WillByDefault(DoAll(SetArgPointee<3>(fakeStmt_), Return(SQLITE_OK)));

    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_DONE));

    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_INTEGER));

    database_ = std::make_shared<SQLiteDatabase>("tmp.db");
    auto statement = std::make_shared<SQLiteStatement>(database_, "SQL");
    resultSet_ = std::make_shared<SQLiteResultSet>(statement);
}

void SQLiteResultSetTest::TearDown() {
    delete reinterpret_cast<int*>(fakeStmt_);

    resultSet_.reset();
    SQLite3Mock::destroy();
}

void SQLiteResultSetTest::checkGetDataType(uint16_t index, uint16_t sqliteType,
        ResultSet::DataType expectedType) {

    EXPECT_CALL(*mock_, sqlite3_column_type(fakeStmt_, index))
            .WillOnce(Return(sqliteType));

    EXPECT_EQ(resultSet_->dataType(index), expectedType);
}

TEST_F(SQLiteResultSetTest, ConstructorWithNullStatementThrowsException) {
    EXPECT_THROW(std::make_shared<SQLiteResultSet>(nullptr), std::invalid_argument);
}

TEST_F(SQLiteResultSetTest, NextCallsStep) {
    EXPECT_CALL(*mock_, sqlite3_step(fakeStmt_));
    resultSet_->next();
}

TEST_F(SQLiteResultSetTest, NextReturnsTrueWhenRowExists) {
    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_ROW));

    EXPECT_TRUE(resultSet_->next());
}

TEST_F(SQLiteResultSetTest, NextReturnsFalseWhenFinished) {
    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_DONE));

    resultSet_->next();
    EXPECT_FALSE(resultSet_->next());
}

TEST_F(SQLiteResultSetTest, NextWithInvalidStatementThrowsException) {
    ON_CALL(*mock_, sqlite3_prepare_v2)
            .WillByDefault(Return(SQLITE_OK));

    auto invalidStatement = std::make_shared<SQLiteStatement>(database_, "SQL");
    auto resultSet = std::make_shared<SQLiteResultSet>(invalidStatement);

    EXPECT_THROW(resultSet->next(), std::logic_error);
}

TEST_F(SQLiteResultSetTest, GetDataTypeInteger) {
    checkGetDataType(2, SQLITE_INTEGER, ResultSet::DataType::INTEGER);
}

TEST_F(SQLiteResultSetTest, GetDataTypeFloat) {
    checkGetDataType(1, SQLITE_FLOAT, ResultSet::DataType::FLOAT);
}

TEST_F(SQLiteResultSetTest, GetDataTypeText) {
    checkGetDataType(5, SQLITE_TEXT, ResultSet::DataType::TEXT);
}

TEST_F(SQLiteResultSetTest, GetDataTypeBlob) {
    checkGetDataType(8, SQLITE_BLOB, ResultSet::DataType::BLOB);
}

TEST_F(SQLiteResultSetTest, GetUnsignedInteger8) {
    EXPECT_CALL(*mock_, sqlite3_column_int(fakeStmt_, 2))
            .WillOnce(Return(10));

    EXPECT_EQ(resultSet_->uint8(2), 10);
}

TEST_F(SQLiteResultSetTest, GetUnsignedInteger16) {
    EXPECT_CALL(*mock_, sqlite3_column_int(fakeStmt_, 2))
            .WillOnce(Return(10));

    EXPECT_EQ(resultSet_->uint16(2), 10);
}

TEST_F(SQLiteResultSetTest, GetUnsignedInteger32) {
    EXPECT_CALL(*mock_, sqlite3_column_int(fakeStmt_, 2))
            .WillOnce(Return(10));

    EXPECT_EQ(resultSet_->uint32(2), 10);
}

TEST_F(SQLiteResultSetTest, GetUnsignedInteger64) {
    EXPECT_CALL(*mock_, sqlite3_column_int64(fakeStmt_, 2))
            .WillOnce(Return(10));

    EXPECT_EQ(resultSet_->uint64(2), 10);
}

TEST_F(SQLiteResultSetTest, GetSignedInteger8) {
    EXPECT_CALL(*mock_, sqlite3_column_int(fakeStmt_, 2))
            .WillOnce(Return(-10));

    EXPECT_EQ(resultSet_->int8(2), -10);
}

TEST_F(SQLiteResultSetTest, GetSignedInteger16) {
    EXPECT_CALL(*mock_, sqlite3_column_int(fakeStmt_, 2))
            .WillOnce(Return(-10));

    EXPECT_EQ(resultSet_->int16(2), -10);
}

TEST_F(SQLiteResultSetTest, GetSignedInteger32) {
    EXPECT_CALL(*mock_, sqlite3_column_int(fakeStmt_, 2))
            .WillOnce(Return(-10));

    EXPECT_EQ(resultSet_->int32(2), -10);
}

TEST_F(SQLiteResultSetTest, GetSignedInteger64) {
    EXPECT_CALL(*mock_, sqlite3_column_int64(fakeStmt_, 2))
            .WillOnce(Return(-10));

    EXPECT_EQ(resultSet_->int64(2), -10);
}

TEST_F(SQLiteResultSetTest, GetFloat) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_FLOAT));

    EXPECT_CALL(*mock_, sqlite3_column_double(fakeStmt_, 2))
            .WillOnce(Return(1.53));

    EXPECT_FLOAT_EQ(resultSet_->flt(2), 1.53);
}

TEST_F(SQLiteResultSetTest, GetDouble) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_FLOAT));

    EXPECT_CALL(*mock_, sqlite3_column_double(fakeStmt_, 2))
            .WillOnce(Return(0.29));

    EXPECT_DOUBLE_EQ(resultSet_->dbl(2), 0.29);
}

TEST_F(SQLiteResultSetTest, GetText) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_TEXT));

    const char* expected = "Test";

    EXPECT_CALL(*mock_, sqlite3_column_text(fakeStmt_, 2))
            .WillOnce(Return(reinterpret_cast<const unsigned char*>(expected)));

    EXPECT_STREQ(resultSet_->str(2).c_str(), expected);
}

MATCHER_P2(MemEq, bytes, size,
        std::string(negation ? "isn't" : "is") + " memory equals") {
    return !memcmp(reinterpret_cast<const char*>(arg), bytes, size);
}

TEST_F(SQLiteResultSetTest, GetBlob) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_BLOB));

    const char* expected = "Test";

    EXPECT_CALL(*mock_, sqlite3_column_bytes(fakeStmt_, 2))
            .WillOnce(Return(strlen(expected)));

    EXPECT_CALL(*mock_, sqlite3_column_blob(fakeStmt_, 2))
            .WillOnce(Return(reinterpret_cast<const unsigned char*>(expected)));

    size_t size = 0;
    std::unique_ptr<const void*> actual = resultSet_->blob(2, &size);

    ASSERT_NE(actual, nullptr);
    EXPECT_EQ(size, strlen(expected));
    EXPECT_THAT(*actual, MemEq(expected, size));
}

TEST_F(SQLiteResultSetTest, GetBlobNoDataReturnsNullptr) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_BLOB));

    EXPECT_CALL(*mock_, sqlite3_column_bytes(fakeStmt_, 2))
            .WillOnce(Return(0));

    size_t size = 0;
    std::unique_ptr<const void*> actual = resultSet_->blob(2, &size);

    EXPECT_EQ(size, 0);
    EXPECT_EQ(actual, nullptr);
}

TEST_F(SQLiteResultSetTest, GetIntegerFromDifferentDataTypeThrowsException) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_TEXT));

    EXPECT_THROW(resultSet_->uint8(0), std::invalid_argument);
    EXPECT_THROW(resultSet_->uint16(0), std::invalid_argument);
    EXPECT_THROW(resultSet_->uint32(0), std::invalid_argument);
    EXPECT_THROW(resultSet_->uint64(0), std::invalid_argument);
    EXPECT_THROW(resultSet_->int8(0), std::invalid_argument);
    EXPECT_THROW(resultSet_->int16(0), std::invalid_argument);
    EXPECT_THROW(resultSet_->int32(0), std::invalid_argument);
    EXPECT_THROW(resultSet_->int64(0), std::invalid_argument);
}

TEST_F(SQLiteResultSetTest, GetFloatFromDifferentDataTypeThrowsException) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_TEXT));

    EXPECT_THROW(resultSet_->flt(0), std::invalid_argument);
    EXPECT_THROW(resultSet_->dbl(0), std::invalid_argument);
}

TEST_F(SQLiteResultSetTest, GetTextFromDifferentDataTypeThrowsException) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_INTEGER));

    EXPECT_THROW(resultSet_->str(0), std::invalid_argument);
}

TEST_F(SQLiteResultSetTest, GetBlobFromDifferentDataTypeThrowsException) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_INTEGER));

    EXPECT_THROW(resultSet_->blob(0, nullptr), std::invalid_argument);
}

} // namespace cppdbc
