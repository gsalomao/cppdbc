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

#include "sqlite_transaction.hpp"

#include <stdexcept>

#include "cppdbc/sqlite/sqlite_database.hpp"

namespace cppdbc {

SQLiteTransaction::SQLiteTransaction(const std::shared_ptr<SQLiteDatabase>& database) :
        database_{database} {

    if (database_ == nullptr) {
        throw std::invalid_argument(
                "Cannot create transaction for invalid database");
    }

    sqlite3_exec(database_->sqlite_, "BEGIN TRANSACTION;", nullptr, nullptr,
            nullptr);
}

SQLiteTransaction::SQLiteTransaction(SQLiteTransaction&& other) noexcept:
        pending_{other.pending_},
        database_{std::move(other.database_)} {

    other.pending_ = false;
}

SQLiteTransaction::~SQLiteTransaction() {
    if (pending_) {
        execute_statement("ROLLBACK;");
    }
}

SQLiteTransaction& SQLiteTransaction::operator=(SQLiteTransaction&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    database_ = std::move(other.database_);
    pending_ = other.pending_;
    other.pending_ = false;

    return *this;
}

bool SQLiteTransaction::pending() const noexcept {
    return pending_;
}

void SQLiteTransaction::commit() {
    if (!pending_) {
        throw std::logic_error("Transaction already committed or rolled back");
    }

    if (database_->sqlite_ == nullptr) {
        throw std::logic_error("Cannot commit transaction with invalid database");
    }

    int result = execute_statement("COMMIT;");
    pending_ = false;

    if (result != SQLITE_OK) {
        throw std::logic_error("Failed to commit SQLite transaction");
    }
}

void SQLiteTransaction::rollback() {
    if (!pending_) {
        throw std::logic_error("Cannot rollback SQLite transaction");
    }

    if (database_->sqlite_ == nullptr) {
        throw std::logic_error("Cannot commit transaction with invalid database");
    }

    int result = execute_statement("ROLLBACK;");
    pending_ = false;

    if (result != SQLITE_OK) {
        throw std::logic_error("Failed to rollback SQLite transaction");
    }
}

int SQLiteTransaction::execute_statement(const std::string& stmt) {
    return sqlite3_exec(database_->sqlite_, stmt.c_str(), nullptr, nullptr,
            nullptr);
}

} // namespace cppdbc
