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
 * @brief Result set interface.
 * @file
 */

#ifndef RESULT_SET_HPP
#define RESULT_SET_HPP

#include <string>
#include <memory>

namespace cppdbc {

/**
 * @brief Table's column.
 *
 * Column of the database table.
 */
typedef uint16_t column_t;

/**
 * @brief SQL result set.
 *
 * A result set object manages the result from a statement.
 */
class IResultSet {
public:
    /**
     * @brief Data type.
     *
     * Type of the data stored in the column.
     */
    enum class DataType {
        INTEGER,    /*!< Integer (signed or unsigned) */
        FLOAT,      /*!< Float (float or double) */
        TEXT,       /*!< Text (string) */
        BLOB        /*!< BLOB (memory) */
    };

    /**
     * @brief Destroy result set.
     *
     * Destructor of the result set.
     */
    virtual ~IResultSet() = default;

    /**
     * @brief Next result set.
     *
     * Move result set to the next result of the statement.
     *
     * @retval true - result set moved to next result.
     * @retval false - result set finished.
     * @throw std::logic_error in case of failure to move to next result set.
     */
    virtual bool next() = 0;

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
    virtual DataType dataType(column_t column) const = 0;

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
    virtual uint8_t uint8(column_t column) const = 0;

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
    virtual uint16_t uint16(column_t column) const = 0;

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
    virtual uint32_t uint32(column_t column) const = 0;

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
    virtual uint64_t uint64(column_t column) const = 0;

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
    virtual int8_t int8(column_t column) const = 0;

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
    virtual int16_t int16(column_t column) const = 0;

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
    virtual int32_t int32(column_t column) const = 0;

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
    virtual int64_t int64(column_t column) const = 0;

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
    virtual float flt(column_t column) const = 0;

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
    virtual double dbl(column_t column) const = 0;

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
    virtual std::string str(column_t column) const = 0;

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
    virtual std::unique_ptr<const void*> blob(column_t column, size_t* size) const = 0;
};

} // namespace cppdbc

#endif // RESULT_SET_HPP
