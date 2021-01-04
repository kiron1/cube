//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <numeric>

namespace cube
{
template <typename T, std::size_t M, std::size_t N>
class basic_matrix
{
public:
    typedef T value_type;
    typedef T& reference_type;

    typedef std::array<std::size_t, 2> index_type;

    std::array<std::size_t, 2> shape() const { return {M, N}; }

    value_type operator[](index_type const& index) const;
    reference_type operator[](index_type const& index);

    static basic_matrix eye();

private:
    std::array<T, M * N> data_;
};

template <typename T, std::size_t M, std::size_t N>
typename basic_matrix<T, M, N>::value_type basic_matrix<T, M, N>::operator[](index_type const& index) const
{
    const auto k = N * index[0] + index[1];
    assert(k < data_.size());
    return data_[k];
}

template <typename T, std::size_t M, std::size_t N>
typename basic_matrix<T, M, N>::reference_type basic_matrix<T, M, N>::operator[](index_type const& index)
{
    const auto k = N * index[0] + index[1];
    assert(k < data_.size());
    return data_[k];
}

template <typename T, std::size_t M, std::size_t N>
basic_matrix<T, M, N> basic_matrix<T, M, N>::eye()
{
    basic_matrix result;
    std::size_t k = 0;
    for (std::size_t i = 0; i != M; ++i)
    {
        for (std::size_t j = 0; j != N; ++j)
        {
            result.data_[k] = i == j ? T(1) : T(0);
            ++k;
        }
    }
    return result;
}

using mat4 = basic_matrix<float, 4, 4>;

} // namespace cube
