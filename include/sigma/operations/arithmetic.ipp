#pragma once

#include "sigma/detail_/setter.hpp"

namespace sigma {

template<typename UncertainType>
UncertainType operator-(const UncertainType& u) {
    UncertainType c(u);
    Setter<UncertainType> c_setter(c);
    c_setter.update_mean(-u.mean());
    c_setter.update_derivatives(-1.0);
    return c;
}

template<typename UncertainType>
UncertainType operator+(const UncertainType& lhs, const UncertainType& rhs) {
    UncertainType c(lhs);
    c += rhs;
    return c;
}

template<typename UncertainType>
UncertainType& operator+=(UncertainType& lhs, const UncertainType& rhs) {
    Setter<UncertainType> lhs_setter(lhs);
    lhs_setter.update_mean(lhs.mean() + rhs.mean());
    lhs_setter.update_derivatives(rhs.deps(), 1.0);
    return lhs;
}

template<typename UncertainType>
UncertainType operator-(const UncertainType& lhs, const UncertainType& rhs) {
    UncertainType c(lhs);
    c -= rhs;
    return c;
}

template<typename UncertainType>
UncertainType operator*(const UncertainType& lhs, const UncertainType& rhs) {
    UncertainType c(lhs);
    c *= rhs;
    return c;
}

template<typename UncertainType>
UncertainType operator*(const UncertainType& lhs, double rhs) {
    UncertainType c(lhs);
    c *= rhs;
    return c;
}

template<typename UncertainType>
UncertainType operator*(double lhs, const UncertainType& rhs) {
    return rhs * lhs;
}

template<typename UncertainType>
UncertainType& operator*=(UncertainType& lhs, const UncertainType& rhs) {
    auto dcda = rhs.mean();
    auto dcdb = lhs.mean();
    Setter<UncertainType> lhs_setter(lhs);
    lhs_setter.update_mean(lhs.mean() * rhs.mean());
    lhs_setter.update_derivatives(dcda, false);
    lhs_setter.update_derivatives(rhs.deps(), dcdb);
    return lhs;
}

template<typename UncertainType>
UncertainType& operator*=(UncertainType& lhs, double rhs) {
    Setter<UncertainType> lhs_setter(lhs);
    lhs_setter.update_mean(lhs.mean() * rhs);
    lhs_setter.update_derivatives(rhs);
    return lhs;
}

template<typename UncertainType>
UncertainType& operator-=(UncertainType& lhs, const UncertainType& rhs) {
    Setter<UncertainType> lhs_setter(lhs);
    lhs_setter.update_mean(lhs.mean() - rhs.mean());
    lhs_setter.update_derivatives(rhs.deps(), -1.0);
    return lhs;
}

template<typename UncertainType>
UncertainType operator/(const UncertainType& lhs, const UncertainType& rhs) {
    UncertainType c(lhs);
    c /= rhs;
    return c;
}

template<typename UncertainType>
UncertainType operator/(const UncertainType& lhs, double rhs) {
    UncertainType c(lhs);
    c /= rhs;
    return c;
}

template<typename UncertainType>
UncertainType operator/(double lhs, const UncertainType& rhs) {
    UncertainType c(rhs);
    Setter<UncertainType> c_setter(c);
    c_setter.update_mean(lhs / rhs.mean());
    c_setter.update_derivatives(-lhs / std::pow(rhs.mean(), 2.0));
    return c;
}

template<typename UncertainType>
UncertainType& operator/=(UncertainType& lhs, const UncertainType& rhs) {
    auto dcda = 1.0 / rhs.mean();
    auto dcdb = -lhs.mean() / std::pow(rhs.mean(), 2.0);
    Setter<UncertainType> lhs_setter(lhs);
    lhs_setter.update_mean(lhs.mean() / rhs.mean());
    lhs_setter.update_derivatives(dcda, false);
    lhs_setter.update_derivatives(rhs.deps(), dcdb);
    return lhs;
}

template<typename UncertainType>
UncertainType& operator/=(UncertainType& lhs, double rhs) {
    Setter<UncertainType> lhs_setter(lhs);
    lhs_setter.update_mean(lhs.mean() / rhs);
    lhs_setter.update_derivatives(1.0 / rhs);
    return lhs;
}

} // namespace sigma
