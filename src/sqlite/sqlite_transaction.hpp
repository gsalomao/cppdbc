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

#ifndef SQLITE_TRANSACTION_HPP
#define SQLITE_TRANSACTION_HPP

#include <string>
#include <memory>

#include "cppdbc/transaction.hpp"

namespace cppdbc {

// Forward declarations
class SQLiteDatabase;

/**
 * @brief SQL transaction for SQLite database.
 *
 * A SQLite transaction manages a transaction for a SQLite database.
 */
class SQLiteTransaction : public ITransaction {
public:
    /**
     * @brief Create SQLite transaction.
     *
     * Constructor of the SQLite transaction.
     *
     * @param[in] database SQLite database which the transaction shall be
     * created.
     *
     * @throw std::invalid_argument in case of invalid database.
     */
    explicit SQLiteTransaction(const std::shared_ptr<SQLiteDatabase>& database);

    /**
     * @brief Remove copy constructor.
     *
     * SQLite transaction is not copyable.
     */
    SQLiteTransaction(const SQLiteTransaction&) = delete;

    /**
     * @brief Move constructor.
     *
     * Move constructor of the SQLite transaction.
     */
    SQLiteTransaction(SQLiteTransaction&& other) noexcept;

    /**
     * @brief Destroy SQLite transaction.
     *
     * Destructor of the SQLite transaction.
     */
    ~SQLiteTransaction() override = default;

    /**
     * Remove copy assignment.
     *
     * SQLite transaction is not copyable.
     */
    SQLiteTransaction& operator=(const SQLiteTransaction&) = delete;

    /**
     * @brief Move assignment.
     *
     * Move assignment of the SQLite transaction.
     */
    SQLiteTransaction& operator=(SQLiteTransaction&& other) noexcept;

    /**
     * @brief Check if SQLite transaction is pending.
     *
     * Check if the SQLite transaction is pending. When the SQLite transaction
     * is created it shall be pending util it is committed.
     *
     * @retvar true - transaction is pending.
     * @retval false - transaction is not pending.
     */
    bool isPending() override;

    /**
     * @brief Commit SQLite transaction.
     *
     * Commit the SQLite transaction persisting all write operations.
     *
     * @throw std::logic_error in case of failure to commit the transaction.
     */
    void commit() override;

    /**
     * @brief Rollback SQLite transaction.
     *
     * Rollback the SQLite transaction discarding all write operations.
     *
     * @throw std::logic_error in case of failure to rollback the transaction.
     */
    void rollback() override;
};

} // namespace cppdbc

#endif // SQLITE_TRANSACTION_HPP
