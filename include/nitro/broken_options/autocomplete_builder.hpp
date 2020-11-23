#pragma once

#if __cplusplus >= 201703L

#include <filesystem>
#include <fstream>
#include <iostream>
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
            std::string program_name;

        public:
            installer(const builder& args, const std::string& program_name)
            : program_name(program_name)
            {
                script = prepare_script(args, program_name);
            }

            void install_zsh() const
            {
                std::cout << "[autocompletion] Search for installation path of zsh..." << std::endl;
                try
                {

                    std::string location = getenv("ZSH");
                    if (!location.empty())
                    {
                        std::cout << "[autocompletion] found!" << std::endl;
                        std::cout << "[autocompletion] installing zsh files..." << std::endl;

                        std::filesystem::create_directories(location + "/plugins/" + program_name);
                        std::ofstream file;
                        file.open(
                            (location + "/plugins/" + program_name + "/" + program_name + ".zsh"));

                        if (file.is_open())
                        {
                            file << script << std::endl;
                            file.close();
                            std::cout << "[autocompletion] zsh instalation successful!"
                                      << std::endl;
                            return;
                        }
                    }
                }
                catch (const std::logic_error&)
                {
                }
                std::cout << "[autocompletion] zsh instalation failed!" << std::endl;
            }

            void install_bash() const
            {
                /*if (std::filesystem::is_directory("/usr/share/doc/bash-completion"))
                {
                    std::cout << "[autocompletion] bash-completion: installing files..." << std::endl;
                    std::ofstream file("/usr/share/doc/bash-completion/" + program_name);
                    if (file.is_open())
                    {
                        file << script << std::endl;
                        file.close();
                        std::cout << "[autocompletion] bash-completion: instalation successful!" << std::endl;
                        return;
                    }
                } else {*/
                    std::cout << "[autocompletion] bash: installing files..." << std::endl;
                    std::ofstream file("/etc/profile.d/" + program_name + ".sh");
                    if (file.is_open())
                    {
                        file << script << std::endl;
                        file.close();

                        std::system(std::string("source /etc/profile.d/" + program_name + ".sh").c_str());
                        std::cout << "[autocompletion] bash: instalation successful!" << std::endl;
                        return;
                    }
                //}
                std::cout << "[autocompletion] bash: instalation failed!" << std::endl;
            }

            void uninstall() const
            {
                /// TODO: implement
            }

        private:
            /*
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
            */

            std::string prepare_script(const builder& builder,
                                       const std::string& programm_name) const
            {
                return builder.get() + " " + programm_name;
            }
        };

    } // namespace autocompletion
} // namespace broken_options
} // namespace nitro

#endif // CPP 17