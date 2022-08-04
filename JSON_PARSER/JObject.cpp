#include "JObject.h"

using namespace Json;

#define GET_VALUE(type, val) *((type*) val)

string JObject::to_string()
{	
	void* val = JObject::value();
	std::ostringstream os;
	switch (m_type) {
	case T_NULL:
		os << GET_VALUE(str_t, val);
		break;
	case T_INT:
		os << GET_VALUE(int_t, val);
		break;
	case T_STR:
		os << '\"' << GET_VALUE(str_t, val) << '\"';
		break;
	case T_BOOL:
		if (GET_VALUE(bool_t, val))
			os << "true";
		else
			os << "false";
		break;
	case T_DOUBLE:
		os << GET_VALUE(double_t, val);
		break;
	case T_LIST:
		list_t& list = GET_VALUE(list_t, val);
		os << '[';
		for (auto& i : list) {
			os << i.to_string();
		}
		os << ']';
		break;
	case T_DICT:
		dict_t& dict = GET_VALUE(dict_t, val);
		os << '{';
		for (auto i = dict.begin(); i != dict.end(); ++i) {
			if (i != dict.begin()) os << ',';
			os << '\"' << i->first << '\"' << i->second.to_string();
		}

	}
	return string();
}

void* JObject::value()
{
	switch (m_type) {
		case T_NULL:
			return get_if<str_t>(&m_value);
			break;
		case T_INT:
			return get_if<int_t>(&m_value);
			break;
		case T_DOUBLE:
			return get_if<double_t>(&m_value);
			break;
		case T_STR:
			return get_if<str_t>(&m_value);
			break;
		case T_BOOL:
			return get_if<bool_t>(&m_value);
			break;
		case T_LIST:
			return get_if<list_t>(&m_value);
			break;
		case T_DICT:
			return get_if<dict_t>(&m_value);
			break;
		default:
			return nullptr;
	}
}





