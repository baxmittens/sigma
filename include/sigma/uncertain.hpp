#pragma once
#include "independent_variable.hpp"
#include <cmath>
#include <iostream>
#include <map>
#include <memory>

namespace sigma {

/** @brief Models an unceratin variable.
 *
 *  In this context, an uncertain variable is one that is dependent on one or
 *  more independent variables. It has a mean value and standard deviation. This
 *  class also keeps track of the variables that each instance is dependent
 *  upon and the contribution of that variable to the uncertainty of this
 *  instance.
 *
 *  @tparam ValueType The type of the value and standard deviation
 *
 */
template<typename ValueType>
class Uncertain {
public:
    /// Type of the instance
    using my_t = Uncertain<ValueType>;

    /// The numeric type of the variable
    using value_t = ValueType;

    /// The type of a variable that this instance depends on
    using ind_var_t = IndependentVariable<value_t>;

    /// A pointer to a dependency of this variable
    using ind_var_ptr = std::shared_ptr<ind_var_t>;

    /// A map of dependencies and their contributions to the uncertainty
    using deps_map_t = std::map<ind_var_ptr, value_t>;

    /// @brief Default ctor
    Uncertain() noexcept = default;

    /** @brief Construct an uncertain value from mean and standard deviation
     *
     *  Effectively, this creates a value that is a function of a single
     *  independent variable.
     *
     *  @param mean The average value of the variable
     *  @param std The standard deviation of the variable
     *
     *  @throw none No throw guarantee
     */
    Uncertain(value_t mean, value_t std);

    /** @brief Get the mean value of the variable
     *
     *  @return The value of the mean
     *
     *  @throw none No throw guarantee
     */
    value_t mean() const { return m_mean_; }

    /** @brief Get the standard deviation of the variable
     *
     *  @return The value of the standard deviation
     *
     *  @throw none No throw guarantee
     */
    value_t std() const { return m_std_; }

    /** @brief Get the dependencies of the variable
     *
     *  @return The dependencies map
     *
     *  @throw none No throw guarantee
     */
    const deps_map_t& deps() const { return m_deps_; }

    /** @brief Negation Operation
     *
     *  @return A copy of this variable, but with the sign of the mean reversed
     *
     *  @throw none No throw guarantee
     */
    my_t operator-() const;

    /** @brief Addition Operation
     *
     *  @param b The variable being added to *this
     *
     *  @return A variable that is the sum of *this and @p b
     *
     *  @throw none No throw guarantee
     */
    my_t operator+(const my_t& b) const;

    /** @brief Subtraction Operation
     *
     *  @param b The variable being subtract from *this
     *
     *  @return A variable that is the difference of *this and @p b
     *
     *  @throw none No throw guarantee
     */
    my_t operator-(const my_t& b) const;

    /** @brief Multiplication Operation
     *
     *  @param b The variable *this is being multiplied by
     *
     *  @return A variable that is the product of *this and @p b
     *
     *  @throw none No throw guarantee
     */
    my_t operator*(const my_t& b) const;

    /** @brief Division Operation
     *
     *  @param b The variable *this is being divided by
     *
     *  @return A variable that is the quotient of *this and @p b
     *
     *  @throw none No throw guarantee
     */
    my_t operator/(const my_t& b) const;

    /** @brief Scale Operation
     *
     *  @tparam NumericType The type of the number @p v
     *  @param v The number by which to scale the variable
     *
     *  @return A variable that is *this scaled by @p v
     *
     *  @throw none No throw guarantee
     */
    template<typename NumericType>
    my_t operator*(NumericType v) const;

    /** @brief Exponentiation of this variable
     *
     *  @tparam NumericType The type of the number @p exp
     *  @param exp The exponent to raise *this by
     *
     *  @return A variable that is *this scaled by @p exp
     *
     *  @throw none No throw guarantee
     */
    template<typename NumericType>
    my_t pow(NumericType exp) const;

private:
    /// Mean value of the variable
    value_t m_mean_;

    /// Standard deviation of the variable
    value_t m_std_;

    /** Map of the variables this value is dependent on to their partial
     *  derivatives with respect to this value
     */
    deps_map_t m_deps_ = {};

    /** @brief Update the partial derivative of a dependency
     *
     *  Updates this variables list of dependencies with the provided values. If
     *  @p var is already in the list of dependencies, the value of its partial
     *  derivative is updated with the addition of @p deriv. Otherwise, @p var
     *  is added to the list of dependencies with @p deriv as its partial
     *  derivative.
     *
     *  @param var The dependency to update
     *  @param deriv The partial derivative of this variable with respect to
     *               the dependency
     *
     *  @throw none No throw guarantee
     */
    void update_dependency(ind_var_ptr var, value_t deriv);

    /** @brief Update the partial derivatives of a set of dependencies
     *
     *  Given a map @p deps of dependencies and their partial derivatives,
     *  updates each dependency of this variable with the partial derivatives
     *  scaled by @p dydx.
     *
     *  @param deps The dependencies to update
     *  @param dydx The partial derivative of this variable with respect to
     *              the dependency
     *
     *  @throw none No throw guarantee
     */
    void update_dependencies(const deps_map_t& deps, value_t dydx);

    /** @brief Calculate the standatd deviation of this variables based on the
     *         uncertainty of its dependencies.
     *
     *  @throw none No throw guarantee
     */
    void calculate_std();

}; // class Uncertain

#define UNCERTAIN Uncertain<ValueType>

// -- Out of line definitions --------------------------------------------------

template<typename ValueType>
template<typename NumericType>
typename UNCERTAIN::my_t UNCERTAIN::operator*(NumericType v) const {
    my_t c(*this);
    c.m_mean_ = v * mean();
    for(const auto& [dep, deriv] : c.deps()) { c.m_deps_[dep] *= v; }
    c.calculate_std();
    return c;
}

template<typename ValueType>
template<typename NumericType>
typename UNCERTAIN::my_t UNCERTAIN::pow(NumericType exp) const {
    my_t c(*this);
    c.m_mean_ = std::pow(mean(), exp);
    auto dydx = exp * std::pow(mean(), exp - 1);
    for(const auto& [dep, deriv] : c.deps()) { c.m_deps_[dep] *= dydx; }
    c.calculate_std();
    return c;
}

// -- Utility functions --------------------------------------------------------

/** @relates Uncertain
 *  @brief Scale Operation
 *
 *  Handles the case where @p v precedes the variable @p u.
 *
 *  @return A variable that is *this scaled by @p v
 *
 *  @throw none No throw guarantee
 */
template<typename ValueType>
UNCERTAIN operator*(ValueType v, const UNCERTAIN& u) {
    return u * v;
}

/** @relates Uncertain
 *  @brief Overload stream insertion to print uncertain variable
 *
 *  @tparam ValueType The numerical type of the variable
 *  @param os The ostream to write to
 *  @param u The uncertain variable to write
 *
 *  @return The modified ostream instance
 *
 *  @throws std::ios_base::failure if anything goes wrong while writing.
 *          Weak throw guarantee.
 */
template<typename ValueType>
std::ostream& operator<<(std::ostream& os, const UNCERTAIN& u) {
    os << u.mean() << "+/-" << u.std();
    return os;
}

/** @relates Uncertain
 *  @brief Compare two variables
 *
 *  @tparam ValueType The numerical type of the variable
 *  @param lhs The first variable
 *  @param rhs The second variable
 *
 *  @return Whether the instances are equivalent
 *
 */
template<typename ValueType>
bool operator==(const UNCERTAIN& lhs, const UNCERTAIN& rhs) {
    if(lhs.mean() != rhs.mean()) return false;
    if(lhs.std() != rhs.std()) return false;
    if(lhs.deps() != rhs.deps()) return false;
    return true;
}

/** @relates Uncertain
 *  @brief Compare two variables
 *
 *  @tparam ValueType The numerical type of the variable
 *  @param lhs The first variable
 *  @param rhs The second variable
 *
 *  @return Whether the instances are not equivalent
 *
 */
template<typename ValueType>
bool operator!=(const UNCERTAIN& lhs, const UNCERTAIN& rhs) {
    return !(lhs == rhs);
}

#undef UNCERTAIN

extern template class Uncertain<double>;

/// Typedef for an uncertain double
using UDouble = Uncertain<double>;

} // namespace sigma