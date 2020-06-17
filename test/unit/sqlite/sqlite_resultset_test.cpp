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

    void check_data_type(uint16_t index, uint16_t sqlite_type,
            ResultSet::DataType expected_type);

    std::shared_ptr<SQLite3Mock> mock_;

    sqlite3_stmt* fake_stmt_{nullptr};
    std::shared_ptr<SQLiteDatabase> database_;
    std::shared_ptr<ResultSet> result_set_;
};

void SQLiteResultSetTest::SetUp() {
    mock_ = std::make_shared<NiceMock<SQLite3Mock>>();
    SQLite3Mock::register_mock(mock_);

    fake_stmt_ = reinterpret_cast<sqlite3_stmt*>(new int(1));

    ON_CALL(*mock_, sqlite3_prepare_v2)
            .WillByDefault(DoAll(SetArgPointee<3>(fake_stmt_), Return(SQLITE_OK)));

    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_DONE));

    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_INTEGER));

    database_ = std::make_shared<SQLiteDatabase>("tmp.db");
    auto statement = std::make_shared<SQLiteStatement>(database_, "SQL");
    result_set_ = std::make_shared<SQLiteResultSet>(statement);
}

void SQLiteResultSetTest::TearDown() {
    delete reinterpret_cast<int*>(fake_stmt_);

    result_set_.reset();
    SQLite3Mock::destroy();
}

TEST_F(SQLiteResultSetTest, ConstructorWithNullStatementThrowsException) {
    EXPECT_THROW(std::make_shared<SQLiteResultSet>(nullptr), std::invalid_argument);
}

TEST_F(SQLiteResultSetTest, NextCallsStep) {
    EXPECT_CALL(*mock_, sqlite3_step(fake_stmt_));
    result_set_->next();
}

TEST_F(SQLiteResultSetTest, NextReturnsTrueWhenRowExists) {
    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_ROW));

    EXPECT_TRUE(result_set_->next());
}

TEST_F(SQLiteResultSetTest, NextReturnsFalseWhenFinished) {
    ON_CALL(*mock_, sqlite3_step)
            .WillByDefault(Return(SQLITE_DONE));

    result_set_->next();
    EXPECT_FALSE(result_set_->next());
}

TEST_F(SQLiteResultSetTest, NextWithInvalidStatementThrowsException) {
    ON_CALL(*mock_, sqlite3_prepare_v2)
            .WillByDefault(Return(SQLITE_OK));

    auto invalid_statement = std::make_shared<SQLiteStatement>(database_, "SQL");
    auto result_set = std::make_shared<SQLiteResultSet>(invalid_statement);

    EXPECT_THROW(result_set->next(), std::logic_error);
}

void SQLiteResultSetTest::check_data_type(uint16_t index, uint16_t sqlite_type,
        ResultSet::DataType expected_type) {

    EXPECT_CALL(*mock_, sqlite3_column_type(fake_stmt_, index))
            .WillOnce(Return(sqlite_type));

    EXPECT_EQ(result_set_->data_type(index), expected_type);
}

TEST_F(SQLiteResultSetTest, GetDataTypeInteger) {
    check_data_type(2, SQLITE_INTEGER, ResultSet::DataType::INTEGER);
}

TEST_F(SQLiteResultSetTest, GetDataTypeFloat) {
    check_data_type(1, SQLITE_FLOAT, ResultSet::DataType::FLOAT);
}

TEST_F(SQLiteResultSetTest, GetDataTypeText) {
    check_data_type(5, SQLITE_TEXT, ResultSet::DataType::TEXT);
}

TEST_F(SQLiteResultSetTest, GetDataTypeBlob) {
    check_data_type(8, SQLITE_BLOB, ResultSet::DataType::BLOB);
}

TEST_F(SQLiteResultSetTest, GetUnsignedInteger8) {
    EXPECT_CALL(*mock_, sqlite3_column_int(fake_stmt_, 2))
            .WillOnce(Return(10));

    EXPECT_EQ(result_set_->uint8(2), 10);
}

TEST_F(SQLiteResultSetTest, GetUnsignedInteger16) {
    EXPECT_CALL(*mock_, sqlite3_column_int(fake_stmt_, 2))
            .WillOnce(Return(10));

    EXPECT_EQ(result_set_->uint16(2), 10);
}

TEST_F(SQLiteResultSetTest, GetUnsignedInteger32) {
    EXPECT_CALL(*mock_, sqlite3_column_int(fake_stmt_, 2))
            .WillOnce(Return(10));

    EXPECT_EQ(result_set_->uint32(2), 10);
}

TEST_F(SQLiteResultSetTest, GetUnsignedInteger64) {
    EXPECT_CALL(*mock_, sqlite3_column_int64(fake_stmt_, 2))
            .WillOnce(Return(10));

    EXPECT_EQ(result_set_->uint64(2), 10);
}

TEST_F(SQLiteResultSetTest, GetSignedInteger8) {
    EXPECT_CALL(*mock_, sqlite3_column_int(fake_stmt_, 2))
            .WillOnce(Return(-10));

    EXPECT_EQ(result_set_->int8(2), -10);
}

TEST_F(SQLiteResultSetTest, GetSignedInteger16) {
    EXPECT_CALL(*mock_, sqlite3_column_int(fake_stmt_, 2))
            .WillOnce(Return(-10));

    EXPECT_EQ(result_set_->int16(2), -10);
}

TEST_F(SQLiteResultSetTest, GetSignedInteger32) {
    EXPECT_CALL(*mock_, sqlite3_column_int(fake_stmt_, 2))
            .WillOnce(Return(-10));

    EXPECT_EQ(result_set_->int32(2), -10);
}

TEST_F(SQLiteResultSetTest, GetSignedInteger64) {
    EXPECT_CALL(*mock_, sqlite3_column_int64(fake_stmt_, 2))
            .WillOnce(Return(-10));

    EXPECT_EQ(result_set_->int64(2), -10);
}

TEST_F(SQLiteResultSetTest, GetFloat) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_FLOAT));

    EXPECT_CALL(*mock_, sqlite3_column_double(fake_stmt_, 2))
            .WillOnce(Return(1.53));

    EXPECT_FLOAT_EQ(result_set_->flt(2), 1.53);
}

TEST_F(SQLiteResultSetTest, GetDouble) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_FLOAT));

    EXPECT_CALL(*mock_, sqlite3_column_double(fake_stmt_, 2))
            .WillOnce(Return(0.29));

    EXPECT_DOUBLE_EQ(result_set_->dbl(2), 0.29);
}

TEST_F(SQLiteResultSetTest, GetText) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_TEXT));

    const char* expected = "Test";

    EXPECT_CALL(*mock_, sqlite3_column_text(fake_stmt_, 2))
            .WillOnce(Return(reinterpret_cast<const unsigned char*>(expected)));

    EXPECT_STREQ(result_set_->str(2).c_str(), expected);
}

MATCHER_P2(MemEq, bytes, size, std::string(negation ? "isn't" : "is") +
                               " memory equals") {
    return !memcmp(reinterpret_cast<const char*>(arg), bytes, size);
}

TEST_F(SQLiteResultSetTest, GetBlob) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_BLOB));

    const char* expected = "Test";

    EXPECT_CALL(*mock_, sqlite3_column_bytes(fake_stmt_, 2))
            .WillOnce(Return(strlen(expected)));

    EXPECT_CALL(*mock_, sqlite3_column_blob(fake_stmt_, 2))
            .WillOnce(Return(reinterpret_cast<const unsigned char*>(expected)));

    size_t size = 0;
    std::unique_ptr<const void*> actual = result_set_->blob(2, &size);

    ASSERT_NE(actual, nullptr);
    EXPECT_EQ(size, strlen(expected));
    EXPECT_THAT(*actual, MemEq(expected, size));
}

TEST_F(SQLiteResultSetTest, GetBlobNoDataReturnsNullptr) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_BLOB));

    EXPECT_CALL(*mock_, sqlite3_column_bytes(fake_stmt_, 2))
            .WillOnce(Return(0));

    size_t size = 0;
    std::unique_ptr<const void*> actual = result_set_->blob(2, &size);

    EXPECT_EQ(size, 0);
    EXPECT_EQ(actual, nullptr);
}

TEST_F(SQLiteResultSetTest, GetIntegerFromDifferentDataTypeThrowsException) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_TEXT));

    EXPECT_THROW(result_set_->uint8(0), std::invalid_argument);
    EXPECT_THROW(result_set_->uint16(0), std::invalid_argument);
    EXPECT_THROW(result_set_->uint32(0), std::invalid_argument);
    EXPECT_THROW(result_set_->uint64(0), std::invalid_argument);
    EXPECT_THROW(result_set_->int8(0), std::invalid_argument);
    EXPECT_THROW(result_set_->int16(0), std::invalid_argument);
    EXPECT_THROW(result_set_->int32(0), std::invalid_argument);
    EXPECT_THROW(result_set_->int64(0), std::invalid_argument);
}

TEST_F(SQLiteResultSetTest, GetFloatFromDifferentDataTypeThrowsException) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_TEXT));

    EXPECT_THROW(result_set_->flt(0), std::invalid_argument);
    EXPECT_THROW(result_set_->dbl(0), std::invalid_argument);
}

TEST_F(SQLiteResultSetTest, GetTextFromDifferentDataTypeThrowsException) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_INTEGER));

    EXPECT_THROW(result_set_->str(0), std::invalid_argument);
}

TEST_F(SQLiteResultSetTest, GetBlobFromDifferentDataTypeThrowsException) {
    ON_CALL(*mock_, sqlite3_column_type)
            .WillByDefault(Return(SQLITE_INTEGER));

    EXPECT_THROW(result_set_->blob(0, nullptr), std::invalid_argument);
}

} // namespace cppdbc
