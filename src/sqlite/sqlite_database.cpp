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

#include "cppdbc/sqlite/sqlite_database.hpp"

#include <stdexcept>

#include "sqlite_statement.hpp"
#include "sqlite_transaction.hpp"

namespace cppdbc {

SQLiteDatabase::SQLiteDatabase(const std::string& filename) :
        SQLiteDatabase(filename, SQLiteMode::READ_ONLY) {}

SQLiteDatabase::SQLiteDatabase(const std::string& filename, SQLiteMode mode) {
    int32_t flags = parse_sqlite_mode(mode);

    int result = sqlite3_open_v2(filename.c_str(), &sqlite_, flags, nullptr);

    if (result != SQLITE_OK) {
        sqlite3_close(sqlite_);
        throw std::invalid_argument("Failed to create database " + filename);
    }
}

SQLiteDatabase::SQLiteDatabase(SQLiteDatabase&& other) noexcept:
        sqlite_{other.sqlite_} {

    other.sqlite_ = nullptr;
}

SQLiteDatabase::~SQLiteDatabase() {
    if (sqlite_ != nullptr) {
        sqlite3_close(sqlite_);
    }
}

SQLiteDatabase& SQLiteDatabase::operator=(SQLiteDatabase&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    if (sqlite_ != nullptr) {
        sqlite3_close(sqlite_);
    }

    sqlite_ = other.sqlite_;
    other.sqlite_ = nullptr;

    return *this;
}

bool SQLiteDatabase::valid() const noexcept {
    return this->sqlite_;
}

std::shared_ptr<Statement> SQLiteDatabase::create_statement(const std::string& sql) {
    if (this->sqlite_ == nullptr) {
        throw std::logic_error("Cannot create statement for invalid database");
    }

    return std::make_shared<SQLiteStatement>(shared_from_this(), sql);
}

std::shared_ptr<Transaction> SQLiteDatabase::create_transaction() {
    return std::make_shared<SQLiteTransaction>(shared_from_this());
}

int32_t SQLiteDatabase::parse_sqlite_mode(SQLiteDatabase::SQLiteMode mode) {
    switch (mode) {
        default:
            return SQLITE_OPEN_READONLY;

        case SQLiteMode::READ_WRITE:
            return SQLITE_OPEN_READWRITE;

        case SQLiteMode::CREATE:
            return static_cast<uint32_t>(SQLITE_OPEN_READWRITE) |
                   static_cast<uint32_t>(SQLITE_OPEN_CREATE);

        case SQLiteMode::IN_MEMORY:
            return static_cast<uint32_t>(SQLITE_OPEN_READWRITE) |
                   static_cast<uint32_t>(SQLITE_OPEN_CREATE) |
                   static_cast<uint32_t>(SQLITE_OPEN_MEMORY);
    }
}

bool SQLiteDatabase::has_table(const std::string& tableName) {
    auto statement = create_statement(
            "SELECT count(*) FROM sqlite_master WHERE type='table' AND name=?");

    statement->bind(tableName, 0);
    auto result = statement->execute();

    return result->uint8(0) == 1;
}

} // namespace cppdbc
