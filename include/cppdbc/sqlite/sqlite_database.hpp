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
 * @brief SQLite database.
 * @file
 */

#ifndef SQLITE_DATABASE_HPP
#define SQLITE_DATABASE_HPP

#include <memory>
#include <string>
#include <sqlite3.h>

#include "cppdbc/database.hpp"

namespace cppdbc {

// Forward declarations
class SQLiteStatement;
class SQLiteTransaction;

/**
 * @brief SQLite database.
 *
 * The database object manages the connection with the SQLite database.
 */
class SQLiteDatabase : public Database, public std::enable_shared_from_this<SQLiteDatabase> {
public:
    /**
     * @brief Database mode.
     *
     * When creating a SQLite database, the database must be in one of the
     * supported mode.
     */
    enum class SQLiteMode {
        READ_ONLY,     /*!< Read-only */
        READ_WRITE,    /*!< Read and write */
        CREATE,        /*!< Read and write or create if not exists */
        IN_MEMORY      /*!< In-memory */
    };

    /**
    * @brief Create SQLite database.
    *
    * Constructor of the SQLite database.
    *
    * @param[in] filename Path to the database file.
    *
    * @throw std::invalid_argument in case of failure to create the database.
    */
    explicit SQLiteDatabase(const std::string& filename);

    /**
     * @brief Create SQLite database.
     *
     * Constructor of the SQLite database.
     *
     * @param[in] filename Path to the database file.
     * @param[in] mode Mode to create database.
     *
     * @throw std::invalid_argument in case of failure to create database.
     */
    SQLiteDatabase(const std::string& filename, SQLiteMode mode);

    /**
     * @brief Remove copy constructor.
     *
     * SQLite database is not copyable.
     */
    SQLiteDatabase(const SQLiteDatabase&) = delete;

    /**
     * @brief Move constructor.
     *
     * Move constructor of the SQLite database.
     */
    SQLiteDatabase(SQLiteDatabase&& other) noexcept;

    /**
     * @brief Destroy SQLite database.
     *
     * Destructor of the SQLite database.
     */
    ~SQLiteDatabase() override;

    /**
     * @brief Remove copy assignment.
     *
     * SQLite database is not copyable.
     */
    SQLiteDatabase& operator=(const SQLiteDatabase&) = delete;

    /**
     * @brief Move assignment.
     *
     * Move SQLite database to new reference.
     */
    SQLiteDatabase& operator=(SQLiteDatabase&& other) noexcept;

    /**
     * @brief Check if SQLite database is valid.
     *
     * Check if the SQLite database is still valid. An invalid connection can't
     * be used to interact with the database.
     *
     * @retval true - database connection is valid.
     * @retval false - database connection is invalid.
     */
    bool valid() const noexcept override;

    /**
     * @brief Create statement.
     *
     * Create SQL statement to be executed in the SQLite database.
     *
     * @param[in] query SQL to be executed.
     *
     * @return Pointer to the created statement.
     * @throw std::invalid_argument in case of invalid query.
     * @throw std::logic_error in case of failure to create statement.
     */
    std::shared_ptr<Statement> createStatement(const std::string& query) override;

    /**
     * @brief Create transaction.
     *
     * Create transaction in the SQLite database. A transaction shall be used
     * to execute various SQL statement as a single operation.
     *
     * @return Pointer to the created transaction.
     * @throw std::logic_error in case of failure to create transaction.
     */
    std::shared_ptr<Transaction> createTransaction() override;

    /**
     * @brief Check if table exists.
     *
     * Check if table exists in the SQLite database.
     *
     * @param[in] tableName Name of the table to check.
     *
     * @retval true - if table exists.
     * @retval false - if table doesn't exist.
     * @throw std::logic_error in case of failure to check if table exists.
     */
    bool hasTable(const std::string& tableName) override;

private:
    /**
     * @brief SQLite statement is friend.
     *
     * Defining SQLite statement as friend of the SQLite database, the
     * statement can execute the queries directly.
     */
    friend class SQLiteStatement;

    /**
     * @brief SQLite transaction is friend.
     *
     * Defining SQLite transaction as friend of the SQLite database, the
     * transaction can execute statements directly.
     */
    friend class SQLiteTransaction;

    /**
     * @brief Parse SQLite database mode.
     *
     * Parse the SQLite database mode to SQLite flags used to open the
     * connection with the database.
     *
     * @param[in] mode SQLite mode used to create the database connection.
     *
     * @return SQLite flags.
     */
    static int32_t parseSQLiteMode(SQLiteMode mode);

    /**
     * @brief SQLite database handler.
     */
    sqlite3* sqlite_ = nullptr;
};

} // namespace cppdbc

#endif // SQLITE_DATABASE_HPP
