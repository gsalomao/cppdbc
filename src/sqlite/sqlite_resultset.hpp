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
 * @brief SQLite result set.
 * @file
 */

#include <memory>
#include <string>

#include "cppdbc/resultset.hpp"

#ifndef SQLITE_RESULT_SET_HPP
#define SQLITE_RESULT_SET_HPP

namespace cppdbc {

// Forward declarations
class SQLiteStatement;

/**
 * @brief SQLite result set.
 *
 * A SQLite result set object manages the result from a SQLite statement.
 */
class SQLiteResultSet : public IResultSet {
public:
    /**
     * @brief Create SQLite result set.
     *
     * Constructor of the SQLite result set.
     *
     * @param[in] statement SQLite statement to get the results.
     *
     * @throw std::invalid_argument in case of invalid statement.
     */
    explicit SQLiteResultSet(const std::shared_ptr<SQLiteStatement>& statement);

    /**
    * @brief Next result set.
    *
    * Move result set to the next result of the statement.
    *
    * @retval true - result set moved to next result.
    * @retval false - result set finished.
    * @throw std::logic_error in case of failure to move to next result set.
    */
    bool next() override;

    /**
    * @brief Get data type.
    *
    * Get the data type of a given column of the result set.
    *
    * @param[in] column Column to get the data type.
    *
    * @return Data type of the column.
    * @throw std::invalid_argument in case of invalid column.
    * @throw std::logic_error in case of failure to get the data type.
    */
    DataType dataType(column_t column) const override;

    /**
     * @brief Get unsigned integer (8-bits).
     *
     * Get unsigned integer (8-bits) value from a given column of the result
     * set.
     *
     * @param[in] column Column to get value.
     *
     * @return Value of the column.
     * @throw std::invalid_argument in case of invalid column.
     * @throw std::logic_error in case of failure to get value.
     */
    uint8_t uint8(column_t column) const override;

    /**
     * @brief Get unsigned integer (16-bits).
     *
     * Get unsigned integer (16-bits) value from a given column of the result
     * set.
     *
     * @param[in] column Column to get value.
     *
     * @return Value of the column.
     * @throw std::invalid_argument in case of invalid column.
     * @throw std::logic_error in case of failure to get value.
     */
    uint16_t uint16(column_t column) const override;

    /**
     * @brief Get unsigned integer (32-bits).
     *
     * Get unsigned integer (32-bits) value from a given column of the result
     * set.
     *
     * @param[in] column Column to get value.
     *
     * @return Value of the column.
     * @throw std::invalid_argument in case of invalid column.
     * @throw std::logic_error in case of failure to get value.
     */
    uint32_t uint32(column_t column) const override;

    /**
     * @brief Get unsigned integer (64-bits).
     *
     * Get unsigned integer (64-bits) value from a given column of the result
     * set.
     *
     * @param[in] column Column to get value.
     *
     * @return Value of the column.
     * @throw std::invalid_argument in case of invalid column.
     * @throw std::logic_error in case of failure to get value.
     */
    uint64_t uint64(column_t column) const override;

    /**
     * @brief Get signed integer (8-bits).
     *
     * Get signed integer (8-bits) value from a given column of the result
     * set.
     *
     * @param[in] column Column to get value.
     *
     * @return Value of the column.
     * @throw std::invalid_argument in case of invalid column.
     * @throw std::logic_error in case of failure to get value.
     */
    int8_t int8(column_t column) const override;

    /**
     * @brief Get signed integer (16-bits).
     *
     * Get signed integer (16-bits) value from a given column of the result
     * set.
     *
     * @param[in] column Column to get value.
     *
     * @return Value of the column.
     * @throw std::invalid_argument in case of invalid column.
     * @throw std::logic_error in case of failure to get value.
     */
    int16_t int16(column_t column) const override;

    /**
     * @brief Get signed integer (32-bits).
     *
     * Get signed integer (32-bits) value from a given column of the result
     * set.
     *
     * @param[in] column Column to get value.
     *
     * @return Value of the column.
     * @throw std::invalid_argument in case of invalid column.
     * @throw std::logic_error in case of failure to get value.
     */
    int32_t int32(column_t column) const override;

    /**
     * @brief Get signed integer (64-bits).
     *
     * Get signed integer (64-bits) value from a given column of the result
     * set.
     *
     * @param[in] column Column to get value.
     *
     * @return Value of the column.
     * @throw std::invalid_argument in case of invalid column.
     * @throw std::logic_error in case of failure to get value.
     */
    int64_t int64(column_t column) const override;

    /**
     * @brief Get float.
     *
     * Get float value from a given column of the result set.
     *
     * @param[in] column Column to get value.
     *
     * @return Value of the column.
     * @throw std::invalid_argument in case of invalid column.
     * @throw std::logic_error in case of failure to get value.
     */
    float flt(column_t column) const override;

    /**
     * @brief Get double.
     *
     * Get double value from a given column of the result set.
     *
     * @param[in] column Column to get value.
     *
     * @return Value of the column.
     * @throw std::invalid_argument in case of invalid column.
     * @throw std::logic_error in case of failure to get value.
     */
    double dbl(column_t column) const override;

    /**
     * @brief Get string.
     *
     * Get string value from a given column of the result set.
     *
     * @param[in] column Column to get value.
     *
     * @return String got from column.
     * @throw std::invalid_argument in case of invalid column.
     * @throw std::logic_error in case of failure to get string.
     */
    std::string str(column_t column) const override;

    /**
     * @brief Get BLOB.
     *
     * Get BLOB value from a given column of the result set.
     *
     * @param[in] column Column to get value.
     * @param[out] size Pointer to store the number of bytes read.
     *
     * @retval Pointer to memory got from column.
     * @retval nullptr when there's no data in the column.
     * @throw std::invalid_argument in case of invalid column or size's pointer.
     * @throw std::logic_error in case of failure to get BLOB.
     */
    std::unique_ptr<const void*> blob(column_t column, size_t* size) const override;

private:
    /**
     * @brief Check data type.
     *
     * Check if a given column has the expected data type. If it doesn't have
     * the expected data type, it throws an exception.
     *
     * @param[in] column Column to check the data type.
     * @param[in] dataType Expected data type to the column.
     *
     * @throw std::invalid_argument in case of the column doesn't have the
     * expected data type.
     */
    void expectDataType(column_t column, DataType type) const;

    /**
     * @brief Indicates if the result set is pending.
     *
     * @note A result set is pending util the last result has been got from
     * the statement.
     */
    bool pending_ = true;

    /**
     * @brief SQLite statement to get the next results sets.
     */
    std::shared_ptr<SQLiteStatement> statement_;
};

} // namespace cppdbc

#endif // SQLITE_RESULT_SET_HPP
