#include "Paser.h"
#include "JObject.h"
#include <iostream>
using namespace Json;

/// @brief constructor
/// @param src 
Parser::Parser(string& src) {
	m_index = 0;
	m_str = src;
	trim_right();
}

/// @brief deserialization function used to convert the string version of JSON into JSON object.
/// @return JObject
JObject Parser::parse()
{
	char cur = next_token();
	if (cur == 'n') {
		return parse_null();
	}
	if (cur == 't' || cur == 'f') {
		return parse_bool();
	}
	if (cur == '-' || std::isdigit(cur)) {
		return parse_number();
	}
	if (cur == '\"'){
		return parse_string();
	}
	if (cur == '[') {
		return parse_list();
	}
	if (cur == '{') {
		return parse_dict();
	}
	throw std::logic_error("unexpected character while parsing json");
}

/// @brief check the validity of the current token(not empty, not comment)and update the m_index accordingly.
/// @return the next valid token.
char Parser::next_token()
{	
	while (std::isspace(m_str[m_index])) m_index++;
	if (m_index >= m_str.size())
		throw std::logic_error("unexpected character while parsing json");
	skip_comment();
	return m_str[m_index];
}

/// @brief remove extra empty charactors.
void Parser::trim_right()
{
	m_str.erase(std::find_if(m_str.rbegin(), m_str.rend(), [](char ch)
		{
			return !std::isspace(ch);
		}).base(), m_str.end());
}

/// @brief check if the current token is a comment and skip the comment.
void Parser::skip_comment()
{
	if (m_str.compare(m_index, 2, R"(//)") == 0) {
		while (true) {
			auto next_pos = m_str.find('\n', m_index);
			if (next_pos == string::npos) {
				throw std::logic_error("invalid comment area");
			}
			m_index = next_pos + 1;
			while (std::isspace(m_str[m_index])) {
				m_index ++;
			}
			if (m_str.compare(m_index, 2, R"(//)") != 0) {
				return;
			}
		}
	}

}

/// @brief parser of null type.
/// @return JObject
JObject Parser::parse_null()
{
	if (m_str.compare(m_index, 4, "null") == 0) {
		m_index += 4;
		return {};
	}
	throw std::logic_error("parse null error");
}

/// @brief parser of int and double type.
/// @return JObject
JObject Parser::parse_number()
{
	size_t pos = m_index;
	if (m_str[m_index] == '-') ++m_index;
	if (std::isdigit(m_str[m_index])) {
		m_index += 1;
		while (std::isdigit(m_str[m_index])) {
			m_index += 1;
		}
	}
	else {
		throw std::logic_error("unexpected character while parsing number");
	}
	if (m_str[m_index] != '.')
		return JObject(strtol(m_str.c_str() + pos, nullptr, 10));
	m_index++;
	if (std::isdigit(m_str[m_index])) {
		m_index += 1;
		while (std::isdigit(m_str[m_index])) {
			m_index += 1;
		}
		return JObject(strtof(m_str.c_str() + pos, nullptr));
	}
	else {
		throw std::logic_error("unexpected character while parsing number");
	}
}

/// @brief paser of bool type.
/// @return JObject
JObject Parser::parse_bool()
{	
	if (m_str.compare(m_index, 4, "true") == 0) {
		m_index += 4;
		return JObject(true);
	}
	else if (m_str.compare(m_index, 5, "false") == 0) {
		m_index += 5;
		return JObject(false);
	}
	else {
		throw std::logic_error("unexpected character while parsing bool");
	}
}

/// @brief check whether the character at pos is a escape character.
/// @param pos 
/// @return JObject 
bool Parser::is_esc_consume(size_t pos)
{
	size_t end_pos = pos;
	while (m_str[pos] == '\\') pos--;
	auto cnt = end_pos - pos;
	return cnt % 2 == 0;
}

/// @brief parser of string type.
/// @return JObject
JObject Parser::parse_string()
{
	auto pre_pos = ++m_index;
	auto pos = m_str.find('"', m_index);
	if (pos != string::npos)
	{
		while (true)
		{
			if (m_str[pos - 1] != '\\')
			{
				break;
			}
			if (is_esc_consume(pos - 1))
			{
				break;
			}
			pos = m_str.find('"', pos + 1);
			if (pos == string::npos)
			{
				throw std::logic_error(R"(expected left '"' in parse string)");
			}
		}
		m_index = pos + 1;
		return m_str.substr(pre_pos, pos - pre_pos);
	}
	throw std::logic_error("parse string error");
}

/// @brief parser of vector type.
/// @return JObject
JObject Parser::parse_list()
{	
	vector<JObject> arr;
	m_index++;
	char cur = next_token();
	if (cur == ']') {
		return JObject(arr);
	}

	while (true) {
		arr.push_back(parse());
		cur = next_token();
		if (cur == ']') {
			m_index++;
			break;
		}
		if (cur != ',') {
			throw std::logic_error("unexpected character while parsing list");
		}
		m_index++;
	}
	return JObject(arr);
}

/// @brief parser of map type.
/// @return JObject
JObject Parser::parse_dict()
{
	map<string, JObject> dict;
	m_index++;
	char cur = next_token();
	if (cur == '}'){
		m_index++;
		return JObject(dict);
	}
	while (true)
	{
		string key = std::move(parse().Value<string>());
		cur = next_token();
		if (cur != ':'){
			throw std::logic_error("expected ':' in parse dict");
		}
		m_index++;

		dict[key] = parse();
		cur = next_token();
		if (cur == '}'){
			m_index++;
			break;
		}
		if (cur != ','){
			throw std::logic_error("expected ',' in parse dict");
		}

		m_index++;
	}
	return JObject(dict);
}


