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
                std::stringstream output;
                output << "complete -W \"";

                bool first = true;
                for (auto& opt : opts_)
                {
                    if (first)
                    {
                        opt.format(output, true);
                        first = false;
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
            std::vector<std::string> install_path;
            std::string script;

        public:
            installer(const builder& args, const std::string& programm_name)
            {
                install_path = locate_rc_location();
                script = prepare_script(args, programm_name);
            }

            void install() const
            {
                if (install_path.empty())
                    return;

                for (const auto& path : install_path)
                {
                    std::ofstream fs;
                    fs.open(path, std::ofstream::app);
                    if (fs.is_open())
                    {
                        fs << script << std::endl;
                        fs.close();

                        std::cout << "[nitro] Autocompletion script successfully installed"
                                  << std::endl;
                    }
                    else
                    {
                        std::cout << "[nitro] Unable to opten file: " << path << std::endl;
                    }
                }
            }

            void uninstall() const
            {
                /// TODO: implement
            }

        private:
            int locate_completion_script() const
            {
                /// TODO: implement
                return -1;
            }

            static bool file_exists(const std::string& name)
            {
                struct stat buffer;
                return (stat(name.c_str(), &buffer) == 0);
            }

            std::vector<std::string> locate_rc_location()
            {
                std::vector<std::string> results;
                std::string home = getenv("HOME");

                std::cout << "[nitro] Search for .bashrc in " << home << std::endl;
                if (file_exists(home + "/.bashrc"))
                {
                    std::cout << "[nitro] .bashrc located." << std::endl;
                    results.emplace_back(home + "/.bashrc");
                }

                std::cout << "[nitro] Search for .zshrc in " << home << std::endl;
                if (file_exists(home + "/.zshrc"))
                {
                    std::cout << "[nitro] .zshrc located." << std::endl;
                    results.emplace_back(home + "/.zshrc");
                }

                if (results.empty())
                    std::cout << "[nitro] .zshrc / .bashrc not found." << std::endl;

                return results;
            }

            std::string prepare_script(const builder& builder,
                                       const std::string& programm_name) const
            {
                return builder.get() + " " + programm_name;
            }
        };

    } // namespace autocompletion
} // namespace broken_options
} // namespace nitro