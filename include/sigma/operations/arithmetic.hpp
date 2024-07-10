#pragma once
#include "sigma/uncertain.hpp"

namespace sigma {

/** @relates Uncertain
 *  @brief Negation Operation
 *
 *  @tparam UncertainType The type of the variable
 *  @param u The variable being negated
 *
 *  @return A copy of @p u, but with the sign of the mean reversed
 *
 *  @throw none No throw guarantee
 */
template<typename UncertainType>
UncertainType operator-(const UncertainType& u);

/** @relates Uncertain
 *  @brief Addition Operation
 *
 *  @tparam UncertainType The type of the variables
 *  @param lhs The left-hand variable
 *  @param rhs The right-hand variable
 *
 *  @return A variable that is the sum of @p lhs and @p rhs
 *
 *  @throw none No throw guarantee
 */
template<typename UncertainType>
UncertainType operator+(const UncertainType& lhs, const UncertainType& rhs);

/** @relates Uncertain
 *  @brief Subtraction Operation
 *
 *  @tparam UncertainType The type of the variables
 *  @param lhs The left-hand variable
 *  @param rhs The right-hand variable
 *
 *  @return A variable that is the difference of @p lhs and @p rhs
 *
 *  @throw none No throw guarantee
 */
template<typename UncertainType>
UncertainType operator-(const UncertainType& lhs, const UncertainType& rhs);

/** @relates Uncertain
 *  @brief Multiplication Operation
 *
 *  @tparam UncertainType The type of the variables
 *  @param lhs The left-hand variable
 *  @param rhs The right-hand variable
 *
 *  @return A variable that is the product of @p lhs and @p rhs
 *
 *  @throw none No throw guarantee
 */
template<typename UncertainType>
UncertainType operator*(const UncertainType& lhs, const UncertainType& rhs);

/** @relates Uncertain
 *  @brief Scale Operation
 *
 *  @tparam UncertainType The type of the uncertain variable
 *  @param lhs The uncertain variable
 *  @param rhs The scaling variable
 *
 *  @return A variable that is @p lhs scaled by @p rhs
 *
 *  @throw none No throw guarantee
 */
template<typename UncertainType>
UncertainType operator*(const UncertainType& lhs, double rhs);

/** @relates Uncertain
 *  @brief Scale Operation
 *
 *  @tparam UncertainType The type of the uncertain variable
 *  @param lhs The scaling variable
 *  @param rhs The uncertain variable
 *
 *  @return A variable that is @p rhs scaled by @p lhs
 *
 *  @throw none No throw guarantee
 */
template<typename UncertainType>
UncertainType operator*(double lhs, const UncertainType& rhs);

/** @relates Uncertain
 *  @brief Division Operation
 *
 *  @tparam UncertainType The type of the variables
 *  @param lhs The left-hand variable
 *  @param rhs The right-hand variable
 *
 *  @return A variable that is the quotient of @p lhs and @p rhs
 *
 *  @throw none No throw guarantee
 */
template<typename UncertainType>
UncertainType operator/(const UncertainType& lhs, const UncertainType& rhs);

} // namespace sigma