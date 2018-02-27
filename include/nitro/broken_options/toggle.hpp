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

#ifndef INCLUDE_NITRO_BROKEN_OPTIONS_TOGGLE_HPP
#define INCLUDE_NITRO_BROKEN_OPTIONS_TOGGLE_HPP

#include <nitro/broken_options/exception.hpp>
#include <nitro/broken_options/fwd.hpp>

#include <nitro/lang/optional.hpp>

#include <memory>
#include <sstream>
#include <string>

namespace nitro
{
namespace broken_options
{
    class toggle
    {
    public:
        toggle(const std::string& name, const std::string& description)
        : name_(name), description_(description), ref_(nullptr), given_(false)
        {
        }

    public:
        toggle& short_name(const std::string& short_name)
        {
            if (short_ && *short_ != short_name)
            {
                raise<parser_error>("Trying to redefine short name");
            }

            short_ = short_name;

            return *this;
        }

        toggle& ref(bool& target)
        {
            ref_ = &target;

            return *this;
        }

    public:
        const std::string& short_name() const
        {
            return *short_;
        }

        bool has_short_name() const
        {
            return static_cast<bool>(short_);
        }

        bool given() const
        {
            return given_;
        }

        const std::string& name() const
        {
            return name_;
        }

        const std::string& description() const
        {
            return description_;
        }

    private:
        void update_value(const std::string&)
        {
            if (ref_ != nullptr)
            {
                *ref_ = true;
            }

            given_ = true;
        }

        void update()
        {
        }

        void check()
        {
        }

        bool matches(const std::string& arg)
        {
            if (short_)
            {
                return (arg == std::string("-") + *short_) || arg == std::string("--") + name_;
            }
            return arg == std::string("--") + name_;
        }

        friend class parser;

    private:
        std::string name_;
        std::string description_;
        nitro::lang::optional<std::string> short_;
        bool* ref_;
        bool given_;
    };
} // namespace broken_options
} // namespace nitro

#endif // INCLUDE_NITRO_BROKEN_OPTIONS_TOGGLE_HPP
