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
#include "sqlite_resultset.hpp"

namespace cppdbc {

SQLiteStatement::SQLiteStatement(const std::shared_ptr<SQLiteDatabase>& database,
        const std::string& query) {
    (void) database;
    (void) query;
}

SQLiteStatement::SQLiteStatement(SQLiteStatement&& other) noexcept {
    (void) other;
}

SQLiteStatement& SQLiteStatement::operator=(cppdbc::SQLiteStatement&& other) noexcept {
    (void) other;
    return *this;
}

bool SQLiteStatement::pending() {
    return true;
}

std::shared_ptr<IResultSet> SQLiteStatement::execute() {
    return std::make_shared<SQLiteResultSet>(shared_from_this());
}

void SQLiteStatement::bind(uint8_t value, uint16_t index) {
    (void) value;
    (void) index;
}

void SQLiteStatement::bind(uint16_t value, uint16_t index) {
    (void) value;
    (void) index;
}

void SQLiteStatement::bind(uint32_t value, uint16_t index) {
    (void) value;
    (void) index;
}

void SQLiteStatement::bind(uint64_t value, uint16_t index) {
    (void) value;
    (void) index;
}

void SQLiteStatement::bind(int8_t value, uint16_t index) {
    (void) value;
    (void) index;
}

void SQLiteStatement::bind(int16_t value, uint16_t index) {
    (void) value;
    (void) index;
}

void SQLiteStatement::bind(int32_t value, uint16_t index) {
    (void) value;
    (void) index;
}

void SQLiteStatement::bind(int64_t value, uint16_t index) {
    (void) value;
    (void) index;
}

void SQLiteStatement::bind(std::string value, uint16_t index) {
    (void) value;
    (void) index;
}

void SQLiteStatement::bind(float value, uint16_t index) {
    (void) value;
    (void) index;
}

void SQLiteStatement::bind(double value, uint16_t index) {
    (void) value;
    (void) index;
}

void SQLiteStatement::bind(const void* value, size_t size, uint16_t index) {
    (void) value;
    (void) size;
    (void) index;
}

} // namespace cppdbc
