# pragma once
# include <string>
# include <vector>
# include <map>

using std::string;
using std::vector;
using std::map;

namespace Json {
	class JObject;

	class Parser
	{
	public:
		Parser(string& src);
		JObject parse();
		char next_token();
		JObject parse_null();
		JObject parse_number();
		JObject parse_bool();
		bool is_esc_consume(size_t pos);
		JObject parse_list();
		JObject parse_dict();
		JObject parse_string();
		void trim_right();
		void skip_comment();


	private:
		size_t m_index;
		string m_str;
	};
}

