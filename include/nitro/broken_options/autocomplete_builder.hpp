#pragma once

#include <fstream>
#include <nitro/broken_options/argument.hpp>
#include <nitro/broken_options/option/base.hpp>
#include <nitro/broken_options/parser.hpp>
#include <sstream>
#include <sys/stat.h>

namespace nitro
{
namespace broken_options
{
    namespace autocompletion
    {
        class option
        {
        private:
            const nitro::broken_options::base& option_;

        public:
            bool force_long_name = false;

            option(const nitro::broken_options::base& option) : option_(option)
            {
            }

            std::string name() const
            {
                return option_.name();
            }

            void format(std::stringstream& stream, bool disable_space = false) const
            {
                if (!disable_space)
                    stream << " ";
                if (option_.has_short_name() && !force_long_name)
                    stream << "-" << option_.short_name() << " --" << option_.name();
                else
                    stream << "--" << option_.name();
            }
        };

        class builder
        {
        private:
            std::deque<option> opts_;

        public:
            builder(nitro::broken_options::parser& parser)
            {
                parser.for_each_option([this](const nitro::broken_options::base& opt) {
                    this->opts_.emplace_back(opt);
                });
            }

            std::string get() const
            {
                auto output = std::stringstream("complete -W \"");

                bool first = true;
                for (auto& opt : opts_)
                {
                    if (first)
                    {
                        opt.format(output, true);
                        continue;
                    }
                    opt.format(output);
                }

                return output.str() + "\"";
            }
        };

        class installer
        {
        private:
            std::string install_path;
            std::string script;

        public:
            installer(builder& args, std::string programm_name)
            {
                install_path = locate_rc_location();
                script = prepare_script(args, programm_name);
            }

            bool install() const
            {
                if (install_path.empty())
                    return false;

                std::ofstream fs(install_path, std::ofstream::app);
                if (fs.is_open())
                {
                    fs << script << std::endl;
                    fs.close();

                    std::cout << "[nitro] Autocompletion script successfully installed"
                              << std::endl;
                }
            }

        private:
            static bool file_exists(const std::string& name)
            {
                struct stat buffer;
                return (stat(name.c_str(), &buffer) == 0);
            }

            std::string locate_rc_location()
            {
                if (file_exists("~/.bashrc"))
                {
                    return "~/.bashrc";
                }
                if (file_exists("~/.zshrc"))
                {
                    return "~/.zshrc";
                }
            }

            std::string prepare_script(const builder& builder, std::string programm_name)
            {
                return builder.get() + " " + programm_name;
            }
        };

    } // namespace autocompletion
} // namespace broken_options
} // namespace nitro