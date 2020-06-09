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

namespace cppdbc {

SQLiteResultSet::SQLiteResultSet(const std::shared_ptr<SQLiteStatement>& statement) {
    (void) statement;
}

bool SQLiteResultSet::next() {
    return false;
}

IResultSet::DataType SQLiteResultSet::dataType(column_t column) const {
    (void) column;
    return DataType::INTEGER;
}

uint8_t SQLiteResultSet::uint8(column_t column) const {
    (void) column;
    return 1;
}

uint16_t SQLiteResultSet::uint16(column_t column) const {
    (void) column;
    return 0;
}

uint32_t SQLiteResultSet::uint32(column_t column) const {
    (void) column;
    return 0;
}

uint64_t SQLiteResultSet::uint64(column_t column) const {
    (void) column;
    return 0;
}

int8_t SQLiteResultSet::int8(column_t column) const {
    (void) column;
    return 0;
}

int16_t SQLiteResultSet::int16(column_t column) const {
    (void) column;
    return 0;
}

int32_t SQLiteResultSet::int32(column_t column) const {
    (void) column;
    return 0;
}

int64_t SQLiteResultSet::int64(column_t column) const {
    (void) column;
    return 0;
}

float SQLiteResultSet::flt(column_t column) const {
    (void) column;
    return 0;
}

double SQLiteResultSet::dbl(column_t column) const {
    (void) column;
    return 0;
}

std::string SQLiteResultSet::str(column_t column) const {
    (void) column;
    return "";
}

std::unique_ptr<const void*> SQLiteResultSet::blob(column_t column, size_t* size) const {
    (void) column;
    (void) size;
    return nullptr;
}

} // namespace cppdbc
