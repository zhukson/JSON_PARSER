#pragma once
#include<string>
#include<string_view>
#include<sstream>

using std::string;
using std::string_view;
using std::stringstream;

namespace json
{
    class Parser {
        Parser() = default;
        static string FromString(string_view content);
        private:
            string m_str;
            size_t m_idx{};
        };
    }
}


