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
 * @brief Transaction interface.
 * @file
 */

#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

namespace cppdbc {

/**
 * @brief SQL transaction.
 *
 * A transaction objects manages a SQL transaction.
 */
class Transaction {
public:
    /**
     * @brief Destroy transaction.
     *
     * Destructor of the transaction.
     */
    virtual ~Transaction() = default;

    /**
     * @brief Check if transaction is pending.
     *
     * Check if the transaction is pending. When the transaction is created
     * it shall be pending util it is committed.
     *
     * @retvar true - transaction is pending.
     * @retval false - transaction is not pending.
     */
    [[nodiscard]] virtual bool pending() const noexcept = 0;

    /**
     * @brief Commit transaction.
     *
     * Commit the transaction persisting all write operations.
     *
     * @throw std::logic_error in case of failure to commit the transaction.
     */
    virtual void commit() = 0;

    /**
     * @brief Rollback transaction.
     *
     * Rollback the transaction discarding all write operations.
     *
     * @throw std::logic_error in case of failure to rollback the transaction.
     */
    virtual void rollback() = 0;
};

} // namespace cppdbc

#endif // TRANSACTION_HPP
