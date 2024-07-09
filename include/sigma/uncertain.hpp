#pragma once
#include <cmath>
#include <iostream>
#include <map>
#include <memory>

namespace sigma {

// Forward Declaration
template<typename ValueType>
class IndependentVariable;

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

private:
    /// Mean value of the variable
    value_t m_mean_;

    /// Standard deviation of the variable
    value_t m_std_;

    /** Map of the variables this value is dependent on to their partial
     *  derivatives with respect to this value
     */
    deps_map_t m_deps_ = {};

    /** A friendly class used by functions to manipulate the private members
     *  of a variable that is being updated
     */
    template<typename T>
    friend class Setter;

}; // class Uncertain

// -- Utility functions --------------------------------------------------------

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
std::ostream& operator<<(std::ostream& os, const Uncertain<ValueType>& u) {
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
bool operator==(const Uncertain<ValueType>& lhs,
                const Uncertain<ValueType>& rhs) {
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
bool operator!=(const Uncertain<ValueType>& lhs,
                const Uncertain<ValueType>& rhs) {
    return !(lhs == rhs);
}

// External instantiations
extern template class Uncertain<float>;
extern template class Uncertain<double>;

/// Typedef for an uncertain float
using UFloat = Uncertain<float>;

/// Typedef for an uncertain double
using UDouble = Uncertain<double>;

} // namespace sigma