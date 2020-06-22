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
 * @brief Database interface.
 * @file
 */

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <memory>

#include "resultset.hpp"
#include "statement.hpp"
#include "transaction.hpp"

namespace cppdbc {

/**
 * @brief Database interface.
 *
 * The database object manages the connection with the database.
 */
class Database {
public:
    /**
     * @brief Destroy database.
     *
     * Destructor of the database connection.
     */
    virtual ~Database() = default;

    /**
     * @brief Check if database is valid.
     *
     * Check if the database connection is valid still valid. An invalid
     * connection can't be used to interact with the database.
     *
     * @retval true - database connection is valid.
     * @retval false - database connection is invalid.
     */
    [[nodiscard]] virtual bool valid() const noexcept = 0;

    /**
     * @brief Create statement.
     *
     * Create SQL statement to be executed in the database.
     *
     * @param[in] query SQL to be executed.
     *
     * @return Pointer to the created statement.
     * @throw std::invalid_argument in case of invalid query.
     * @throw std::logic_error in case of failure to create statement.
     */
    virtual std::shared_ptr<Statement> create_statement(const std::string& query) = 0;

    /**
     * @brief Create transaction.
     *
     * Create transaction in the database. A transaction shall be used to
     * execute various SQL statement as a single operation.
     *
     * @return Pointer to the created transaction.
     * @throw std::logic_error in case of failure to create transaction.
     */
    virtual std::shared_ptr<Transaction> create_transaction() = 0;

    /**
     * @brief Check if table exists.
     *
     * Check if table exists in the database.
     *
     * @param[in] tableName Name of the table to check.
     *
     * @retval true - table exists.
     * @retval false - table doesn't exist.
     * @throw std::logic_error in case of failure to check if table exists.
     */
    virtual bool has_table(const std::string& tableName) = 0;
};

} // namespace cppdbc

#endif // DATABASE_HPP
