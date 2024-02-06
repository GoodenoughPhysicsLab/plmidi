#include <regex>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

namespace stdf = std::filesystem;

namespace detail {

template<typename... Args>
void print(Args&&... args) noexcept {
    (..., (::std::cout << ::std::forward<Args>(args)));
}

#ifdef PI_DEBUG
template<typename T>
void print(::std::vector<T> arg) noexcept
    requires requires (T t) {print(t);}
{
    ::std::cout << "[ ";
    for (auto& i : arg) {
        print(i);
        ::std::cout << " ";
    }
    ::std::cout << "]";
}
#endif

template<typename... Args>
void println(Args&&... args) noexcept {
    print(args...);
    ::std::endl(::std::cout);
}

// like `detail/../` should be fixed
::std::string fix_path(::stdf::path path) noexcept {
    ::std::string res{path.string()};
    for (int i{}; i < res.size(); ++i) {
        if (res.at(i) == '\\') {
	    res.at(i) = '/';
	}
    }
    while (res.find("../") != ::std::string::npos) {
        res = ::std::regex_replace(res, ::std::regex("\\w+\\/\\.\\.\\/"), "");
    }
    return res;
}

class Included_files {
    using frame = ::std::vector<::std::string>;
public:
    ::std::vector<frame> data{frame{}};
    Included_files() = default;
    ~Included_files() = default;

    void append(const ::std::string& path) {
        this->data[data.size() - 1].push_back(path);
    }

    void new_frame() {
        this->data.push_back(frame{});
    }

    void del_frame() {
        this->data.pop_back();
    }

    bool in(::stdf::path path) {
        ::std::string fixed_path = fix_path(path);
        for (frame& included_paths : this->data) {
            for (auto& path : included_paths) {
                if (fixed_path == path) {
                    return true;
                }
            }
        }
        this->append(fixed_path);
        return false;
    }
};

Included_files included_files{};

::std::string output{};

bool merge_file(::stdf::path&& path) noexcept {
    if (!::stdf::exists(path)) { // can't merge, usually it is standard library
        return false;
    }
    if (included_files.in(path)) {
        return true;
    }

    ::std::fstream file{path.c_str()};
    ::std::string line;
    while (::std::getline(file, line))
    {
        if (::std::regex_match(line, ::std::regex("# *include.*")))
        {
            bool status = false;
            ::std::string include_path{};
            for (char a_char : line) {
                if (a_char == '\"' || a_char == '<' || a_char == '>') {
                    status = !status;
                    continue;
                }

                if (status) {
                    include_path.push_back(a_char);
                }
            }

            if (!merge_file(path.parent_path() / include_path)) {
                output += line + "\n";
            }
        }
        else if (::std::regex_match(line, ::std::regex("# *if.*"))
                 || ::std::regex_match(line, ::std::regex("# *ifdef.*"))
                 || ::std::regex_match(line, ::std::regex("# *ifndef.*")))
        {
            included_files.new_frame();
            output += line + "\n";
        }
        else if (::std::regex_match(line, ::std::regex("# *else.*"))
                 || ::std::regex_match(line, ::std::regex("# *elif.*"))
                 || ::std::regex_match(line, ::std::regex("# *elifdef.*"))
                 || ::std::regex_match(line, ::std::regex("# *elifndef.*")))
        {
            included_files.data[included_files.data.size() - 1].clear();
            output += line + "\n";
        }
        else if (::std::regex_match(line, ::std::regex("# *endif.*"))) {
            included_files.del_frame();
            output += line + "\n";
        }
        else if (line != "#pragma once") [[likely]] {
            output += line + "\n";
        }
    }
    file.close();
    return true;
}

} // namespace detail

// preInclude /a/file/path
int main(int argc, char** argv) noexcept {
    if (argc < 2) {
        detail::println("preInclude");
        return 0;
    }

    // -o
    ::std::string output_path{"release/release.cpp"};
    for (int i{2}; i < argc; ++i) {
        if (strcmp(argv[i], "-o") == 0 && i != argc - 1) {
            output_path = argv[i + 1];
        }
    }
    {
        auto tmp = ::stdf::path{output_path};
        if (!::stdf::exists(tmp)) {
            ::stdf::create_directories(tmp.parent_path());
        }
    }

    // check input path
    auto path = stdf::path{argv[1]};
    if (!stdf::exists(path) || ::stdf::is_directory(path)) {
        detail::println("file \"", path.string(), "\" don't exist");
        return 0;
    }

    detail::merge_file(::std::move(path));

    ::std::ofstream output{output_path};
    output << detail::output;
    output.close();

    return 0;
}
