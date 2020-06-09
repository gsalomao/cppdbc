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

#ifndef SQLITE3_MOCK_HPP
#define SQLITE3_MOCK_HPP

#include <gmock/gmock.h>
#include <memory>
#include <sqlite3.h>

namespace cppdbc {

/**
 * @brief SQLite mock.
 *
 * Mock class of the SQLite database.
 */
class SQLite3Mock {
public:
    /**
     * @brief Create SQLite mock.
     *
     * Default constuctor of the SQLite mock.
     */
    SQLite3Mock() = default;

    /**
     * @brief Register mock.
     *
     * Register the SQLite mock as static.
     *
     * @note Only one SQLite mock can be register at the time.
     *
     * @param mock Pointer to the SQLite mock to be register.
     */
    static void registerMock(const std::shared_ptr<SQLite3Mock>& mock);

    /**
     * @brief Get SQLite mock.
     *
     * Get instance of the registered SQLite mock.
     *
     * @return Reference to the registered SQLite mock.
     */
    static SQLite3Mock& getInstance();

    /**
     * @brief Destroy SQLite mock.
     *
     * Destroy the registered SQLite mock.
     */
    static void destroy();

    /**
     * @brief Delete copy constructor.
     *
     * Delete copy constructor as the SQLite mock is not copyable.
     */
    SQLite3Mock(const SQLite3Mock& other) = delete;

    /**
     * @brief Delete copy assignment.
     *
     * Delete copy assignment as the SQLite mock is not copyable.
     */
    SQLite3Mock& operator=(const SQLite3Mock& other) = delete;

    MOCK_METHOD(int, sqlite3_open_v2, (const char*, sqlite3**, int, const char*));
    MOCK_METHOD(int, sqlite3_close, (sqlite3*));
};

} // namespace cppdbc

#endif // SQLITE3_MOCK_HPP
