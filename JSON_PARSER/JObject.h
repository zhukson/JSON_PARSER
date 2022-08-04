#pragma once

#include <stdexcept>
#include <utility>
#include <variant>
#include <string>
#include <vector>
#include <map>
#include <string_view>
#include <sstream>

namespace Json{
	using std::variant;
	using std::vector;
	using std::string;
	using std::string_view;
	using std::stringstream;
	using std::get_if;
	using std::map;
	using std::is_same;

	enum TYPE {
		T_NULL,
		T_BOOL,
		T_INT,
		T_DOUBLE,
		T_STR,
		T_LIST,
		T_DICT
	};

	class JObject;
	using null_t = string;
	using int_t = int;
	using bool_t = bool;
	using double_t = double;
	using str_t = string;
	using list_t = vector<JObject>;
	using dict_t = map<string, JObject>;

#define	IS_TYPE(type_a, type_b) is_same<type_a, type_b>::value;

	template<class T>
	constexpr bool is_basic_type() {
		if constexpr (IS_TYPE(T, str_t) ||
			IS_TYPE(T, int_t) ||
			IS_TYPE(T, double_t) ||
			IS_TYPE(T, bool_t)) {
			return true;
		}
		return false;
	}

	class JObject {
	public:
		using value_t = variant<int_t, double_t, bool_t, str_t, list_t, dict_t>;
	//construct function
		JObject() {
			Null();
		}
		JObject(int_t val) {
			Int(val);
		}
		JObject(double_t val) {
			Double(val);
		}
		JObject(bool_t val) {
			Bool(val);
		}
		JObject(str_t val) {
			Str(val);
		}
		JObject(list_t val) {
			List(val);
		}
		JObject(dict_t val) {
			Dict(val);
		}

		void Null() {
			m_type = T_NULL;
			m_value = "null";
		}

		void Str(str_t val) {
			m_type = T_STR;
			m_value = val;
		}

		void Int(int_t val) {
			m_type = T_INT;
			m_value = val;
		}
		
		void Double(double_t val) {
			m_type = T_DOUBLE;
			m_value = val;
		}
		
		void Bool(bool_t val) {
			m_type = T_BOOL;
			m_value = val;
		}
		
		void List(list_t val) {
			m_type = T_STR;
			m_value = std::move(val);
		}

		void Dict(dict_t val) {
			m_type = T_DICT;
			m_value = std::move(val);
		}

#define THROW_GET_ERROR(erron) throw std::logic_error("type error in getting "#erron" value!");

		template<class V>
		V& Value()
		{
			if constexpr (IS_TYPE(V, str_t))
			{
				if (m_type != T_STR)
					THROW_GET_ERROR(string);
			}
			else if constexpr (IS_TYPE(V, bool_t))
			{
				if (m_type != T_BOOL)
					THROW_GET_ERROR(BOOL);
			}
			else if constexpr (IS_TYPE(V, int_t))
			{
				if (m_type != T_INT)
					THROW_GET_ERROR(INT);
			}
			else if constexpr (IS_TYPE(V, double_t))
			{
				if (m_type != T_DOUBLE)
					THROW_GET_ERROR(DOUBLE);
			}
			else if constexpr (IS_TYPE(V, list_t))
			{
				if (m_type != T_LIST)
					THROW_GET_ERROR(LIST);
			}
			else if constexpr (IS_TYPE(V, dict_t))
			{
				if (m_type != T_DICT)
					THROW_GET_ERROR(DICT);
			}

			void* v = value();
			if (v == nullptr)
				throw std::logic_error("unknown type in JObject::Value()");
			return *((V*)v);
		}
	
		string to_string();
	private:
		void* value();

	private:
		TYPE m_type;
		value_t m_value;
	};
}