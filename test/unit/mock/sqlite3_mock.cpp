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

#include <stdexcept>
#include <memory>

#include "sqlite3_mock.hpp"

namespace cppdbc {

static std::shared_ptr<SQLite3Mock> mock_;

void SQLite3Mock::registerMock(const std::shared_ptr<SQLite3Mock>& mock) {
    mock_ = mock;
}

SQLite3Mock& SQLite3Mock::getInstance() {
    if (!mock_) {
        throw std::runtime_error("SQLite3Mock not registered");
    }

    return *mock_;
}

void SQLite3Mock::destroy() {
    mock_.reset();
}

} // namespace cppdbc

using cppdbc::SQLite3Mock;

int sqlite3_open_v2(const char* filename, sqlite3** db, int flags, const char* zVfs) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_open_v2(filename, db, flags, zVfs);
}

int sqlite3_close(sqlite3* db) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_close(db);
}

int sqlite3_prepare_v2(sqlite3* db, const char* sql, int byte, sqlite3_stmt** stmt,
        const char** tail) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_prepare_v2(db, sql, byte, stmt, tail);
}

int sqlite3_finalize(sqlite3_stmt* stmt) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_finalize(stmt);
}

int sqlite3_step(sqlite3_stmt* stmt) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_step(stmt);
}

int sqlite3_bind_int(sqlite3_stmt* stmt, int col, int value) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_bind_int(stmt, col, value);
}

int sqlite3_bind_int64(sqlite3_stmt* stmt, int col, sqlite3_int64 value) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_bind_int64(stmt, col, value);
}

int sqlite3_bind_double(sqlite3_stmt* stmt, int col, double value) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_bind_double(stmt, col, value);
}

int sqlite3_bind_text(sqlite3_stmt* stmt, int col, const char* str, int len,
        void (* ptr)(void*)) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_bind_text(stmt, col, str, len, ptr);
}

int sqlite3_bind_blob(sqlite3_stmt* stmt, int col, const void* blob, int len,
        void (* ptr)(void*)) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_bind_blob(stmt, col, blob, len, ptr);
}

int sqlite3_column_type(sqlite3_stmt* stmt, int col) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_column_type(stmt, col);
}

int sqlite3_column_int(sqlite3_stmt* stmt, int col) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_column_int(stmt, col);
}

sqlite3_int64 sqlite3_column_int64(sqlite3_stmt* stmt, int col) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_column_int64(stmt, col);
}

double sqlite3_column_double(sqlite3_stmt* stmt, int col) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_column_double(stmt, col);
}

const unsigned char* sqlite3_column_text(sqlite3_stmt* stmt, int col) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_column_text(stmt, col);
}

const void* sqlite3_column_blob(sqlite3_stmt* stmt, int col) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_column_blob(stmt, col);
}

int sqlite3_column_bytes(sqlite3_stmt* stmt, int col) {
    SQLite3Mock& mock = SQLite3Mock::getInstance();
    return mock.sqlite3_column_bytes(stmt, col);
}
