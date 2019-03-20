//------------------------------------------------------------------------------
/*
  This file is part of sunnycoind: https://github.com/sunnycoin/sunnycoind
  Copyright (c) 2012-2015 SUNNYCoin Labs Inc.

  Permission to use, copy, modify, and/or distribute this software for any
  purpose  with  or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
  MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef SUNNYCOIN_TEST_JTX_AMOUNT_H_INCLUDED
#define SUNNYCOIN_TEST_JTX_AMOUNT_H_INCLUDED

#include <test/jtx/Account.h>
#include <test/jtx/amount.h>
#include <test/jtx/tags.h>
#include <sunnycoin/protocol/Issue.h>
#include <sunnycoin/protocol/STAmount.h>
#include <sunnycoin/basics/contract.h>
#include <cstdint>
#include <ostream>
#include <string>
#include <type_traits>

namespace sunnycoin {
namespace test {
namespace jtx {

/*

The decision was made to accept amounts of drops and SUN
using an int type, since the range of SUN is 100 billion
and having both signed and unsigned overloads creates
tricky code leading to overload resolution ambiguities.

*/

struct AnyAmount;

// Represents "no amount" of a currency
// This is distinct from zero or a balance.
// For example, no USD means the trust line
// doesn't even exist. Using this in an
// inappropriate context will generate a
// compile error.
//
struct None
{
    Issue issue;
};

//------------------------------------------------------------------------------

template <class T>
struct dropsPerSUN
{
    static T const value = 1000000;
};

/** Represents an SUN or IOU quantity
    This customizes the string conversion and supports
    SUN conversions from integer and floating point.
*/
struct PrettyAmount
{
private:
    // VFALCO TODO should be Amount
    STAmount amount_;
    std::string name_;

public:
    PrettyAmount() = default;
    PrettyAmount (PrettyAmount const&) = default;
    PrettyAmount& operator=(PrettyAmount const&) = default;

    PrettyAmount (STAmount const& amount,
            std::string const& name)
        : amount_(amount)
        , name_(name)
    {
    }

    /** drops */
    template <class T>
    PrettyAmount (T v, std::enable_if_t<
        sizeof(T) >= sizeof(int) &&
            std::is_integral<T>::value &&
                std::is_signed<T>::value>* = nullptr)
        : amount_((v > 0) ?
            v : -v, v < 0)
    {
    }

    /** drops */
    template <class T>
    PrettyAmount (T v, std::enable_if_t<
        sizeof(T) >= sizeof(int) &&
            std::is_integral<T>::value &&
                std::is_unsigned<T>::value>* = nullptr)
        : amount_(v)
    {
    }

    std::string const&
    name() const
    {
        return name_;
    }

    STAmount const&
    value() const
    {
        return amount_;
    }

    operator STAmount const&() const
    {
        return amount_;
    }

    operator AnyAmount() const;
};

inline
bool
operator== (PrettyAmount const& lhs,
    PrettyAmount const& rhs)
{
    return lhs.value() == rhs.value();
}

std::ostream&
operator<< (std::ostream& os,
    PrettyAmount const& amount);

//------------------------------------------------------------------------------

// Specifies an order book
struct BookSpec
{
    AccountID account;
    sunnycoin::Currency currency;

    BookSpec(AccountID const& account_,
        sunnycoin::Currency const& currency_)
        : account(account_)
        , currency(currency_)
    {
    }
};

//------------------------------------------------------------------------------

struct SUN_t
{
    /** Implicit conversion to Issue.

        This allows passing SUN where
        an Issue is expected.
    */
    operator Issue() const
    {
        return sunIssue();
    }

    /** Returns an amount of SUN as STAmount

        @param v The number of SUN (not drops)
    */
    /** @{ */
    template <class T, class = std::enable_if_t<
        std::is_integral<T>::value>>
    PrettyAmount
    operator()(T v) const
    {
        return { std::conditional_t<
            std::is_signed<T>::value,
                std::int64_t, std::uint64_t>{v} *
                    dropsPerSUN<T>::value };
    }

    PrettyAmount
    operator()(double v) const
    {
        auto const c =
            dropsPerSUN<int>::value;
        if (v >= 0)
        {
            auto const d = std::uint64_t(
                std::round(v * c));
            if (double(d) / c != v)
                Throw<std::domain_error> (
                    "unrepresentable");
            return { d };
        }
        auto const d = std::int64_t(
            std::round(v * c));
        if (double(d) / c != v)
            Throw<std::domain_error> (
                "unrepresentable");
        return { d };
    }
    /** @} */

    /** Returns None-of-SUN */
    None
    operator()(none_t) const
    {
        return { sunIssue() };
    }

    friend
    BookSpec
    operator~ (SUN_t const&)
    {        
        return BookSpec( sunAccount(),
            sunCurrency() );
    }
};

/** Converts to SUN Issue or STAmount.

    Examples:
        SUN         Converts to the SUN Issue
        SUN(10)     Returns STAmount of 10 SUN
*/
extern SUN_t const SUN;

/** Returns an SUN STAmount.

    Example:
        drops(10)   Returns STAmount of 10 drops
*/
template <class Integer,
    class = std::enable_if_t<
        std::is_integral<Integer>::value>>
PrettyAmount
drops (Integer i)
{
    return { i };
}

//------------------------------------------------------------------------------

namespace detail {

struct epsilon_multiple
{
    std::size_t n;
};

} // detail

// The smallest possible IOU STAmount
struct epsilon_t
{
    epsilon_t()
    {
    }

    detail::epsilon_multiple
    operator()(std::size_t n) const
    {
        return { n };
    }
};

static epsilon_t const epsilon;

/** Converts to IOU Issue or STAmount.

    Examples:
        IOU         Converts to the underlying Issue
        IOU(10)     Returns STAmount of 10 of
                        the underlying Issue.
*/
class IOU
{
public:
    Account account;
    sunnycoin::Currency currency;

    IOU(Account const& account_,
            sunnycoin::Currency const& currency_)
        : account(account_)
        , currency(currency_)
    {
    }

    Issue
    issue() const
    {
        return { currency, account.id() };
    }

    /** Implicit conversion to Issue.

        This allows passing an IOU
        value where an Issue is expected.
    */
    operator Issue() const
    {
        return issue();
    }

    template <class T, class = std::enable_if_t<
        sizeof(T) >= sizeof(int) &&
            std::is_arithmetic<T>::value>>
    PrettyAmount operator()(T v) const
    {
        // VFALCO NOTE Should throw if the
        //             representation of v is not exact.
        return { amountFromString(issue(),
            std::to_string(v)), account.name() };
    }

    PrettyAmount operator()(epsilon_t) const;
    PrettyAmount operator()(detail::epsilon_multiple) const;

    // VFALCO TODO
    // STAmount operator()(char const* s) const;

    /** Returns None-of-Issue */
    None operator()(none_t) const
    {
        return { issue() };
    }

    friend
    BookSpec
    operator~ (IOU const& iou)
    {
        return BookSpec(iou.account.id(), iou.currency);
    }
};

std::ostream&
operator<<(std::ostream& os,
    IOU const& iou);

//------------------------------------------------------------------------------

struct any_t
{
    inline
    AnyAmount
    operator()(STAmount const& sta) const;
};

/** Amount specifier with an option for any issuer. */
struct AnyAmount
{
    bool is_any;
    STAmount value;

    AnyAmount() = delete;
    AnyAmount (AnyAmount const&) = default;
    AnyAmount& operator= (AnyAmount const&) = default;

    AnyAmount (STAmount const& amount)
        : is_any(false)
        , value(amount)
    {
    }

    AnyAmount (STAmount const& amount,
            any_t const*)
        : is_any(true)
        , value(amount)
    {
    }

    // Reset the issue to a specific account
    void
    to (AccountID const& id)
    {
        if (! is_any)
            return;
        value.setIssuer(id);
    }
};

inline
AnyAmount
any_t::operator()(STAmount const& sta) const
{
    return AnyAmount(sta, this);
}

/** Returns an amount representing "any issuer"
    @note With respect to what the recipient will accept
*/
extern any_t const any;

} // jtx
} // test
} // sunnycoin

#endif
