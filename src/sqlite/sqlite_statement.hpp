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

/**
 * @brief SQLite statement.
 * @file
 */

#ifndef SQLITE_STATEMENT_HPP
#define SQLITE_STATEMENT_HPP

#include <memory>
#include <string>
#include <sqlite3.h>

#include "cppdbc/statement.hpp"

namespace cppdbc {

// Forward declarations
class SQLiteDatabase;

/**
 * @brief SQLite statement.
 *
 * A SQLite statement object manages a single SQL query for a SQLite database.
 */
class SQLiteStatement : public Statement, public std::enable_shared_from_this<SQLiteStatement> {
public:
    /**
     * @brief Create SQLite statement.
     *
     * Constructor of the SQLite statements.
     *
     * @param[in] database SQLite database which the statement will be executed.
     * @param[in] query SQL query to be executed on database.
     *
     * @throw std::invalid_argument in case of invalid database or query.
     */
    SQLiteStatement(const std::shared_ptr<SQLiteDatabase>& database, const std::string& query);

    /**
     * @brief Remove copy constructor.
     *
     * SQLite statement is not copyable.
     */
    SQLiteStatement(const SQLiteStatement&) = delete;

    /**
     * @brief Move constructor.
     *
     * Move constructor of the SQLite statement.
     */
    SQLiteStatement(SQLiteStatement&& other) noexcept;

    /**
     * @brief Destroy SQLite statement.
     *
     * Destructor of the SQLite statement.
     */
    ~SQLiteStatement() override;

    /**
     * @brief Remove copy assignment.
     *
     * SQLite statement is not copyable.
     */
    SQLiteStatement& operator=(const SQLiteStatement&) = delete;

    /**
     * @brief Move assignment.
     *
     * Move assignment of the SQLite statement.
     */
    SQLiteStatement& operator=(SQLiteStatement&& other) noexcept;

    /**
     * @brief Check if SQLite statement is pending.
     *
     * Check if the SQLite statement is pending. When a statement is created it
     * shall be pending and can be executed only once. After that, it shall
     * not be pending anymore.
     *
     * @retval true - statement pending.
     * @retval false - statement is not pending.
     */
    [[nodiscard]] bool pending() const noexcept override;

    /**
     * @brief Execute SQLite statement.
     *
     * Once the SQLite statement has been created, it can be executed in the
     * SQLite database. Statements, when executed, it generates a result set or
     * not.
     *
     * @retval Pointer to the result set.
     * @retval nullptr when there's no result set.
     * @throw cppdbc::constraint_violation in case of the statement violates
     * any constraint.
     * @throw std::logic_error in case of failure to execute the statement.
     */
    std::shared_ptr<ResultSet> execute() override;

    /**
     * @brief Bind unsigned integer (8-bits).
     *
     * Bind unsigned integer (8-bits) value to a given index of the statement.
     *
     * @param[in] value Value to be bound.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(uint8_t value, uint16_t index) override;

    /**
     * @brief Bind unsigned integer (16-bits).
     *
     * Bind unsigned integer (16-bits) value to a given index of the statement.
     *
     * @param[in] value Value to be bound.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(uint16_t value, uint16_t index) override;

    /**
     * @brief Bind unsigned integer (32-bits).
     *
     * Bind unsigned integer (32-bits) value to a given index of the statement.
     *
     * @param[in] value Value to be bound.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(uint32_t value, uint16_t index) override;

    /**
     * @brief Bind unsigned integer (64-bits).
     *
     * Bind unsigned integer (64-bits) value to a given index of the statement.
     *
     * @param[in] value Value to be bound.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(uint64_t value, uint16_t index) override;

    /**
     * @brief Bind signed integer (8-bits).
     *
     * Bind signed integer (8-bits) value to a given index of the statement.
     *
     * @param[in] value Value to be bound.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(int8_t value, uint16_t index) override;

    /**
     * @brief Bind signed integer (16-bits).
     *
     * Bind signed integer (16-bits) value to a given index of the statement.
     *
     * @param[in] value Value to be bound.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(int16_t value, uint16_t index) override;

    /**
     * @brief Bind signed integer (32-bits).
     *
     * Bind signed integer (32-bits) value to a given index of the statement.
     *
     * @param[in] value Value to be bound.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(int32_t value, uint16_t index) override;

    /**
     * @brief Bind signed integer (64-bits).
     *
     * Bind signed integer (64-bits) value to a given index of the statement.
     *
     * @param[in] value Value to be bound.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(int64_t value, uint16_t index) override;

    /**
     * @brief Bind boolean.
     *
     * Bind boolean value to a given index of the statement.
     *
     * @param[in] value Value to be bound.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(bool value, uint16_t index) override;

    /**
     * @brief Bind string.
     *
     * Bind string value to a given index of the statement.
     *
     * @param[in] value Value to be bound.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(std::string value, uint16_t index) override;

    /**
     * @brief Bind float.
     *
     * Bind float value to a given index of the statement.
     *
     * @param[in] value Value to be bound.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(float value, uint16_t index) override;

    /**
     * @brief Bind double.
     *
     * Bind double value to a given index of the statement.
     *
     * @param[in] value Value to be bound.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(double value, uint16_t index) override;

    /**
     * @brief Bind BLOB.
     *
     * Bind BLOB to a given index of the statement.
     *
     * @param[in] value Pointer to memory location to be bound.
     * @param[in] size Number of bytes of the bound memory.
     * @param[in] index Index which the value shall be bound.
     *
     * @throw std::invalid_argument in case of failure to bind the value to
     * column.
     */
    void bind(const void* value, size_t size, uint16_t index) override;

private:
    /**
     * @brief SQLite result set is friend.
     *
     * Defining SQL result set as friend of SQLite statement, the result set
     * can execute statement directly.
     */
    friend class SQLiteResultSet;

    /**
     * @brief Check SQLite result.
     *
     * Check if result returned from SQLite is SQLITE_OK. If it's not, it
     * throws an exception.
     *
     * @param[in] result Result value from SQLite.
     * @param[in] message Message of the exception.
     *
     * @throw std::invalid_argument in case of the result value is not
     * SQLITE_OK.
     */
    static void check_sqlite_result(int result, const std::string& message);

    /**
     * @brief Indicates if the statement has not completed.
     *
     * @note A statement is pending while it wasn't executed yet.
     */
    bool pending_ = true;

    /**
     * @brief SQLite statement handler.
     */
    sqlite3_stmt* statement_ = nullptr;

    /**
     * @brief SQLite database object.
     */
    std::shared_ptr<SQLiteDatabase> database_;
};

} // namespace cppdbc

#endif // SQLITE_STATEMENT_HPP
