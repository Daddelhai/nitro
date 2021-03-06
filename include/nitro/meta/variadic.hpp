/*
 * Copyright (c) 2015-2016, Technische Universität Dresden, Germany
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INCLUDE_NITRO_META_VARIADIC_HPP
#define INCLUDE_NITRO_META_VARIADIC_HPP

#include <type_traits>

namespace nitro
{
namespace meta
{

    /**
     * @brief meta function to check if a variadic type pack contains a given type.
     */
    template <typename...>
    struct is_variadic_member;

    template <typename U, typename first, typename... Attributes>
    struct is_variadic_member<U, first, Attributes...>
    {
        static const bool value =
            std::is_same<U, first>::value || is_variadic_member<U, Attributes...>::value;
    };

    template <typename U>
    struct is_variadic_member<U> : std::false_type
    {
    };

    template <template <typename...> class U, typename... Members>
    struct is_variadic_member_specialization;

    template <template <typename...> class U, typename... Members, typename... Args>
    struct is_variadic_member_specialization<U, U<Args...>, Members...>
    {
        static const bool value = true;
    };

    template <template <typename...> class U, typename First, typename... Members>
    struct is_variadic_member_specialization<U, First, Members...>
    {
        static const bool value = is_variadic_member_specialization<U, Members...>::value;
    };

    template <template <typename...> class U>
    struct is_variadic_member_specialization<U>
    {
        static const bool value = false;
    };
} // namespace meta
} // namespace nitro

#endif // INCLUDE_NITRO_META_VARIADIC_HPP
