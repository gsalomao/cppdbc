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
 * @brief Statement interface.
 * @file
 */

#ifndef STATEMENT_HPP
#define STATEMENT_HPP

namespace cppdbc {

// Forward declarations
class ResultSet;

/**
 * @brief SQL statement.
 *
 * A statement object manages a single SQL query.
 */
class Statement {
public:
    /**
     * @brief Destroy statement.
     *
     * Destructor of the statement.
     */
    virtual ~Statement() = default;

    /**
     * @brief Check if statement is pending.
     *
     * Check if the statement is pending. When a statement is created it
     * shall be pending and can be executed only once. After that, it shall
     * not be pending anymore.
     *
     * @retval true - statement pending.
     * @retval false - statement is not pending.
     */
    virtual bool pending() = 0;

    /**
     * @brief Execute statement.
     *
     * Once the statement has been created, it can be executed in the
     * database. Statements, when executed, it generates a result set or not.
     *
     * @retval Pointer to the result set.
     * @retval nullptr when there's no result set.
     * @throw std::logic_error in case of failure to execute the statement.
     */
    virtual std::shared_ptr<ResultSet> execute() = 0;

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
    virtual void bind(uint8_t value, uint16_t index) = 0;

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
    virtual void bind(uint16_t value, uint16_t index) = 0;

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
    virtual void bind(uint32_t value, uint16_t index) = 0;

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
    virtual void bind(uint64_t value, uint16_t index) = 0;

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
    virtual void bind(int8_t value, uint16_t index) = 0;

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
    virtual void bind(int16_t value, uint16_t index) = 0;

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
    virtual void bind(int32_t value, uint16_t index) = 0;

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
    virtual void bind(int64_t value, uint16_t index) = 0;

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
    virtual void bind(std::string value, uint16_t index) = 0;

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
    virtual void bind(float value, uint16_t index) = 0;

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
    virtual void bind(double value, uint16_t index) = 0;

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
    virtual void bind(const void* value, size_t size, uint16_t index) = 0;
};

} // namespace cppdbc

#endif // STATEMENT_HPP
