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

#include "sqlite_resultset.hpp"

#include "sqlite_statement.hpp"

namespace cppdbc {

SQLiteResultSet::SQLiteResultSet(const std::shared_ptr<SQLiteStatement>& statement) :
        statement_{statement} {

    if (statement_ == nullptr) {
        throw std::invalid_argument("Cannot create result set for invalid statement");
    }
}

bool SQLiteResultSet::next() {
    if (!pending_) {
        return false;
    }

    if (statement_->statement_ == nullptr) {
        throw std::logic_error("Cannot get next result set for invalid statement");
    }

    int result = sqlite3_step(statement_->statement_);

    if (result == SQLITE_DONE) {
        pending_ = false;
    }

    return pending_;
}

ResultSet::DataType SQLiteResultSet::dataType(column_t column) const {
    int result = sqlite3_column_type(statement_->statement_, column);

    switch (result) {
        default:
        case SQLITE_INTEGER:
            return DataType::INTEGER;
        case SQLITE_FLOAT:
            return DataType::FLOAT;
        case SQLITE_TEXT:
            return DataType::TEXT;
        case SQLITE_BLOB:
        case SQLITE_NULL:
            return DataType::BLOB;
    }
}

uint8_t SQLiteResultSet::uint8(column_t column) const {
    expectDataType(column, DataType::INTEGER);
    return static_cast<uint8_t>(sqlite3_column_int(statement_->statement_, column));
}

uint16_t SQLiteResultSet::uint16(column_t column) const {
    expectDataType(column, DataType::INTEGER);
    return static_cast<uint16_t>(sqlite3_column_int(statement_->statement_, column));
}

uint32_t SQLiteResultSet::uint32(column_t column) const {
    expectDataType(column, DataType::INTEGER);
    return static_cast<uint32_t>(sqlite3_column_int(statement_->statement_, column));
}

uint64_t SQLiteResultSet::uint64(column_t column) const {
    expectDataType(column, DataType::INTEGER);
    return static_cast<uint64_t>(sqlite3_column_int64(statement_->statement_, column));
}

int8_t SQLiteResultSet::int8(column_t column) const {
    expectDataType(column, DataType::INTEGER);
    return static_cast<int8_t>(sqlite3_column_int(statement_->statement_, column));
}

int16_t SQLiteResultSet::int16(column_t column) const {
    expectDataType(column, DataType::INTEGER);
    return static_cast<int16_t>(sqlite3_column_int(statement_->statement_, column));
}

int32_t SQLiteResultSet::int32(column_t column) const {
    expectDataType(column, DataType::INTEGER);
    return static_cast<int32_t>(sqlite3_column_int(statement_->statement_, column));
}

int64_t SQLiteResultSet::int64(column_t column) const {
    expectDataType(column, DataType::INTEGER);
    return sqlite3_column_int64(statement_->statement_, column);
}

float SQLiteResultSet::flt(column_t column) const {
    expectDataType(column, DataType::FLOAT);
    return static_cast<float>(sqlite3_column_double(statement_->statement_, column));
}

double SQLiteResultSet::dbl(column_t column) const {
    expectDataType(column, DataType::FLOAT);
    return sqlite3_column_double(statement_->statement_, column);
}

std::string SQLiteResultSet::str(column_t column) const {
    expectDataType(column, DataType::TEXT);
    const unsigned char* result = sqlite3_column_text(statement_->statement_, column);

    return std::string(reinterpret_cast<const char*>(result));
}

std::unique_ptr<const void*> SQLiteResultSet::blob(column_t column, size_t* size) const {
    if (!size) {
        throw std::invalid_argument("Size's pointer cannot be null");
    }

    expectDataType(column, DataType::BLOB);
    *size = static_cast<size_t>(sqlite3_column_bytes(statement_->statement_, column));

    if (!(*size)) {
        return nullptr;
    }

    const void* blob = sqlite3_column_blob(statement_->statement_, column);

    return std::make_unique<const void*>(blob);
}

void SQLiteResultSet::expectDataType(column_t column, ResultSet::DataType type) const {
    if (dataType(column) != type) {
        throw std::invalid_argument("Column doesn't have the expected data type");
    }
}

} // namespace cppdbc
