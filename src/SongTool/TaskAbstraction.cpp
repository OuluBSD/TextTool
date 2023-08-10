#include "SongTool.h"

String TaskTitledList::GetTreeString(int indent) const {
	String s;
	String in;
	in.Cat('\t', indent);
	
	s << in << "title_format_str: " << title_format_str << "\n";
	if (title_value.GetCount()) s << in << "title_value: " << title_value << "\n";
	if (title_args.GetCount()) {
		s << in << "title_args: ";
		String a;
		for (const Value& v : title_args) {
			if (!a.IsEmpty()) a << ", ";
			a << v.ToString();
		}
		s << a << "\n";
	}
	if (separator.GetCount()) s << in << "separator: " << separator << "\n";
	if (line_str.GetCount()) s << in << "line_str: " << line_str << "\n";
	if (line_in_alpha) s << in << "line_in_alpha: true\n";
	if (no_colon) s << in << "no_colon: true\n";
	if (no_listchar) s << in << "no_listchar: true\n";
	if (title_in_quotes) s << in << "title_in_quotes: true\n";
	if (value_in_quotes) s << in << "value_in_quotes: true\n";
	if (count_lines) s << in << "count_lines: true\n";
	if (inline_list) s << in << "inline_list: true\n";
	if (no_separator) s << in << "no_separator: true\n";
	if (combination_string) s << in << "combination_string: true\n";
	if (empty_line) s << in << "empty_line: true\n";
	if (capitalize) s << in << "capitalize: true\n";
	if (line_begin) s << in << "line_begin: " << line_begin << "\n";
	if (scores.GetCount()) {
		s << in << "scores: ";
		for(int i = 0; i < scores.GetCount(); i++) {
			if (i) s << ", ";
			s << scores[i];
		}
		s << "\n";
	}
	
	for(int i = 0; i < items.GetCount(); i++) {
		String key = items.GetKey(i);
		String value = items[i];
		if (key.GetCount() && items.GetCount())
			s << in << "\t[" << i << "] " << key << ": " << items[i] << "\n";
		else if (key.GetCount())
			s << in << "\t[" << i << "] " << key << "\n";
		else if (value.GetCount())
			s << in << "\t[" << i << "] " << value << "\n";
		else
			s << in << "\t[" << i << "] <empty>\n";
	}
	
	int sub_i = 0;
	for (const auto& list : sub) {
		s << in << "sub[" << sub_i++ << "] {\n";
		s << list.GetTreeString(indent+1);
		s << in << "}\n";
	}
	
	return s;
}

String TaskContent::GetTreeString() const {
	String s;
	
	for (const TaskTitledList& list : titled_lists) {
		s << "{\n";
		s << list.GetTreeString(1);
		s << "}\n";
	}
	
	if (pre_answer) {
		s << "{\n";
		s << pre_answer->GetTreeString(1);
		s << "}\n";
	}
	
	s << "response_length: " << response_length << "\n";
	
	return s;
}

String TaskTitledList::AsString() const {
	String s;
	
	if (title_in_quotes)
		s << "\"";
	if (title_format_str.GetCount()) {
		String title = Format(title_format_str, title_args);
		if (capitalize)
			title = ::Capitalize(title);
		s << title;
	}
	if (title_in_quotes)
		s << "\"";
	
	if (title_value.GetCount()) {
		s << ": ";
		if (value_in_quotes)
			s << "\"";
		if (capitalize)
			s << ::Capitalize(title_value);
		else
			s << title_value;
		if (value_in_quotes)
			s << "\"";
	}
	if (!no_colon)
		s << ":";
	
	s << "\n";
	
	
	if (!items.IsEmpty()) {
		for(int i = 0; i < items.GetCount(); i++) {
			if (count_lines) {
				if (!no_listchar)
					s << "-";
				if (line_in_alpha) {
					int limit = 'z' - 'a' + 1;
					int chr = i % limit;
					int mul = i / limit;
					s.Cat('a' + chr);
					if (mul > 0) s << mul;
					s.Cat(' ');
				}
				else {
					s.Cat(' ');
					s << "Line " << line_begin + i << ", ";
				}
				if (!no_separator) {
					if (separator.GetCount())
						s << separator;
					else
						s << ", ";
				}
			}
			else {
				if (!no_listchar)
					s << "- ";
			}
			
			String key = items.GetKey(i);
			String value = items[i];
			if (capitalize) {
				key = ::Capitalize(key);
				value = ::Capitalize(value);
			}
			if (key.GetCount())
				s << key << ": ";
			if (value_in_quotes)
				s.Cat('\"');
			s << value;
			if (value_in_quotes)
				s.Cat('\"');
			
			if (inline_list) {
				if (no_separator)
					s << " ";
				else if (separator.IsEmpty())
					s << ", ";
				else
					s << separator;
			}
			else
				s << "\n";
		}
	}
	
	for (const TaskTitledList& list : sub) {
		s << list.AsString();
		s << "\n";
	}
	
	if (combination_string) {
		TODO
	}
	
	if (empty_line) {
		s << "-";
	}
	
	return s;
}

String TaskContent::AsString() const {
	String s;
	
	LOG(GetTreeString());
	
	for (const TaskTitledList& list : titled_lists) {
		s << list.AsString();
		s << "\n\n";
	}
	
	if (pre_answer) {
		s << pre_answer->AsString();
	}
	
	return s;
}
