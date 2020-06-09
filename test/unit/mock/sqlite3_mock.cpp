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
