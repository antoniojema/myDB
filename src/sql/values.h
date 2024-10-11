#pragma once

#include "common/utils.h"
#include "common/typedef.h"

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <variant>
#include <type_traits>
#include <chrono>


namespace SQL {
namespace Value {

enum class Type {
    Null,
    Bool,
    Integer,
    Float,
    String,
    Date,
    Money
};

enum class Currency {
    EUR, USD, GBP, JPY
};

using Day   = std::chrono::day;
using Month = std::chrono::month;
using Year  = std::chrono::year;

using Null = struct {};
using Bool = bool;
using Integer = long long int;
using Float = double;
using String = std::string;
class Date {
public:
    Date(Year y, Month m, Day d) : _date(y, m, d) {}

    void set(Year y, Month m, Day d) {this->_date = std::chrono::year_month_day(y, m, d);}
    void set_day  (Day   d) {this->set(this->_date.year(), this->_date.month(), d);}
    void set_month(Month m) {this->set(this->_date.year(), m, this->_date.day());}
    void set_year (Year  y) {this->set(y, this->_date.month(), this->_date.day());}

    Day   get_day  () {return this->_date.day  ();}
    Month get_month() {return this->_date.month();}
    Year  get_year () {return this->_date.year ();}

    template <typename R, typename P>
    Date operator+(std::chrono::duration<R,P> dur) {return Date(std::chrono::sys_days(this->_date) + dur);}
    
    template <typename R, typename P>
    Date& operator+=(std::chrono::duration<R,P> dur) {(*this) = (*this) + dur; return this;}

private:
    std::chrono::year_month_day _date;
};

struct Money {
public:
    Money(Currency currency, unsigned char cents_figures = 2) :
        _currency(currency),
        _cents_figures(cents_figures),
        _cents_limit(static_pow(10, cents_figures))
    {}
    Money(ssize_t amount, unsigned int cents, Currency currency, unsigned char cents_figures = 2) :
        _amount(amount),
        _cents(cents),
        _currency(currency),
        _cents_figures(cents_figures),
        _cents_limit(static_pow(10, cents_figures))
    {
        if (this->_cents >= _cents_limit) {
            this->_amount += this->_cents / this->_cents_limit;
            this->_cents = this->_cents % this->_cents_limit;
        }
    }

    void set_amount(ssize_t _amount) {this->_amount = _amount;}
    void set_cents (ssize_t _cents ) {this->_cents  = _cents; }
    ssize_t       get_amount() const {return this->_amount;}
    unsigned char get_cents () const {return (this->is_positive()) ? this->_cents : (this->_cents_limit - this->_cents); }
    unsigned char get_cents_figures() const {return this->_cents_figures; }

    double get_float() const {return (double)this->_amount + (double)this->_cents/(this->_cents_limit);}

    bool is_positive() const {return this->_amount >= 0;}
    bool is_negative() const {return this->_amount < 0;}

    Money operator+(const Money& rhs) const {
        if (this->_currency != rhs._currency) {
            throw std::runtime_error("Error: Attempted to perform summation of different currencies.");
        }

        auto amount = this->_amount + rhs._amount;

        if (this->_cents_figures == rhs._cents_figures) {
            return Money{
                amount,
                this->_cents + rhs._cents,
                this->_currency,
                this->_cents_figures
            };
        }
        else if (this->_cents_figures > rhs._cents_figures) {
            return Money{
                amount,
                this->_cents + rhs._cents * static_pow(10, this->_cents_figures - rhs._cents_figures),
                this->_currency,
                this->_cents_figures
            };
        }
        else {
            return Money{
                amount,
                this->_cents * static_pow(10, rhs._cents_figures - this->_cents_figures) + rhs._cents,
                this->_currency,
                rhs._cents_figures
            };
        }
    }

    Money& operator+=(const Money& rhs) {
        return (*this) = (*this) + rhs;
    }

    Currency _currency;
private:
    ssize_t _amount {0};
    unsigned int _cents {0};

    unsigned char _cents_figures {2};
    unsigned int _cents_limit {10};

};

constexpr Null null {};

enum class ValueType {
    Null = 0, Bool, Integer, Float, String, Date, Money
};

template <typename T>
concept ValidValue = ::IsAnyOf<T,
    Null,
    Bool,
    Integer,
    Float,
    String,
    Date,
    Money
>;

class Value {
public:
    Value() : _value(null) {}
    Value(Null _null) : _value(_null) {}
    Value(bool val) : _value(val) {}
    template<::Integral T> Value(T val) : _value((Integer)val)) {}
    template<::Floating T> Value(T val) : _value((Float)val)) {}
    Value(const std::string_view& val) : _value(String(val)) {}
    Value(std::string&& val) noexcept : _value(String(std::move(val))) {}
    Value(const char * const val) : _value(String(val)) {}
    Value(const Date& val) : _value(Date(val)) {}
    Value(const Money& val) : _value(Money(val)) {}

    template<ValidValue T> Value(const T& val) : _value(val) {}
    template<ValidValue T> Value(T&& val) noexcept : _value(std::move(val)) {}

    ValueType type() const {
        return (ValueType)this->_value.index();
    }
    bool is_null() const { return this->type() == ValueType::Null; }

    template<ValidValue T> T& get() {
        return std::get<T>(this->_value);
    }
    template<ValidValue T> const T& get() const {
        return std::get<T>(this->_value);
    }

private:
    std::variant<Null, Bool, Integer, Float, String, Date, Money> _value;
};

} // namespace Value
} // namespace SQL
