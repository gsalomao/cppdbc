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

#include "sqlite_statement.hpp"

#include "cppdbc/sqlite/sqlite_database.hpp"
#include "sqlite_resultset.hpp"

namespace cppdbc {

SQLiteStatement::SQLiteStatement(const std::shared_ptr<SQLiteDatabase>& database,
        const std::string& query) :
        database_{database} {

    if (database_ == nullptr) {
        throw std::invalid_argument(
                "Cannot create statement for invalid database");
    }

    int result = sqlite3_prepare_v2(database_->sqlite_, query.c_str(),
            static_cast<int>(query.size()), &statement_, nullptr);

    check_sqlite_result(result, "Failed to create statement");
}

SQLiteStatement::SQLiteStatement(SQLiteStatement&& other) noexcept:
        pending_{other.pending_},
        statement_{other.statement_},
        database_{std::move(other.database_)} {

    other.pending_ = false;
    other.statement_ = nullptr;
}

SQLiteStatement::~SQLiteStatement() {
    if (statement_ != nullptr) {
        sqlite3_finalize(statement_);
    }
}

SQLiteStatement& SQLiteStatement::operator=(cppdbc::SQLiteStatement&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    if (statement_ != nullptr) {
        sqlite3_finalize(statement_);
    }

    database_ = std::move(other.database_);
    statement_ = other.statement_;
    pending_ = other.pending_;

    other.statement_ = nullptr;
    other.pending_ = false;

    return *this;
}

bool SQLiteStatement::pending() {
    return pending_;
}

std::shared_ptr<ResultSet> SQLiteStatement::execute() {
    if (!pending_) {
        throw std::logic_error("Statement already executed");
    }

    if (statement_ == nullptr) {
        throw std::logic_error("Cannot execute invalid statement");
    }

    int result = sqlite3_step(statement_);
    pending_ = false;

    switch (result) {
        case SQLITE_ROW:
            return std::make_shared<SQLiteResultSet>(shared_from_this());
        case SQLITE_DONE:
            return nullptr;
        case SQLITE_CONSTRAINT:
            throw std::logic_error("Constraint violation");
        default:
            throw std::logic_error("Failed to execute SQLite statement");
    }
}

void SQLiteStatement::bind(uint8_t value, uint16_t index) {
    int result = sqlite3_bind_int(statement_, index + 1, value);
    check_sqlite_result(result, "Failed to bind uint8");
}

void SQLiteStatement::bind(uint16_t value, uint16_t index) {
    int result = sqlite3_bind_int(statement_, index + 1, value);
    check_sqlite_result(result, "Failed to bind uint16");
}

void SQLiteStatement::bind(uint32_t value, uint16_t index) {
    int result = sqlite3_bind_int(statement_, index + 1, value);
    check_sqlite_result(result, "Failed to bind uint32");
}

void SQLiteStatement::bind(uint64_t value, uint16_t index) {
    int result = sqlite3_bind_int64(statement_, index + 1,
            static_cast<sqlite3_int64>(value));
    check_sqlite_result(result, "Failed to bind uint64");
}

void SQLiteStatement::bind(int8_t value, uint16_t index) {
    int result = sqlite3_bind_int(statement_, index + 1, value);
    check_sqlite_result(result, "Failed to bind int8");
}

void SQLiteStatement::bind(int16_t value, uint16_t index) {
    int result = sqlite3_bind_int(statement_, index + 1, value);
    check_sqlite_result(result, "Failed to bind int16");
}

void SQLiteStatement::bind(int32_t value, uint16_t index) {
    int result = sqlite3_bind_int(statement_, index + 1, value);
    check_sqlite_result(result, "Failed to bind int32");
}

void SQLiteStatement::bind(int64_t value, uint16_t index) {
    int result = sqlite3_bind_int64(statement_, index + 1, value);
    check_sqlite_result(result, "Failed to bind int64");
}

void SQLiteStatement::bind(bool value, uint16_t index) {
    int result = sqlite3_bind_int(statement_, index + 1, value);
    check_sqlite_result(result, "Failed to bind bool");
}

void SQLiteStatement::bind(std::string value, uint16_t index) {
    int result = sqlite3_bind_text(statement_, index + 1, value.c_str(),
            static_cast<int>(value.size()), nullptr);

    check_sqlite_result(result, "Failed to bind string");
}

void SQLiteStatement::bind(float value, uint16_t index) {
    int result = sqlite3_bind_double(statement_, index + 1, value);
    check_sqlite_result(result, "Failed to bind float");
}

void SQLiteStatement::bind(double value, uint16_t index) {
    int result = sqlite3_bind_double(statement_, index + 1, value);
    check_sqlite_result(result, "Failed to bind double");
}

void SQLiteStatement::bind(const void* value, size_t size, uint16_t index) {
    int result = sqlite3_bind_blob(statement_, index + 1, value,
            static_cast<int>(size), nullptr);

    check_sqlite_result(result, "Failed to bind blob");
}

void SQLiteStatement::check_sqlite_result(int result, const std::string& message) {
    if (result != SQLITE_OK) {
        throw std::invalid_argument(message);
    }
}

} // namespace cppdbc
