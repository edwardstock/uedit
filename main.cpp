/**
 * uedit
 * main.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 *
 *
 *
 * Usage:
 *
 * uedit [command: edit, search(s), comment(c)] !filename! [options]
 *
 * global options:
 *  -f,--force    force, overwrite all, be careful!
 *  -h,-?         this help
 *
 *
 * edit or empty command:
 *  Example:
 *      ./uedit /path/to/config.ini -k post_max_size -v 1G
 *      ./uedit edit /path/to/config.ini -l 4 -v Off
 *      ./uedit edit /path/to/custom.config post_max_size -v "post_max_size=1G" (example how to overwrite whole found line)
 *
 *  -v,--value    value to set
 *  -k,--key      key
 *  -s,--search   search specified string in file and set to found line value from -v
 *  -l,--line     line where set value (-v)
 *  if not set any of option: -k,-s or -l the file will clear and value set to first line like command: echo "value" > /path/to/file.cfg
 *
 * search (shortcut: s):
 *  Example:
 *      ./uedit search /path/to/config.ini stringToFind
 *  -b,--buffer    Number of lines to show around found row
 *
 * comment (shortcut: c):
 *  Example:
 *      ./uedit comment /path/to/config.ini -l 45
 *      ./uedit comment /path/to/config.ini post_max_size
 *      ./uedit comment /path/to/custom.superconfig my_custom_line -c "//"
 *
 *  -l,--line      Line to comment
 *  -s,--search    Search row and comment it
 *  -c,--char      Character for commenting. By default, "uedit" try to determine how to comment file, if not, prompt will ask you for required character
 *
 *
 *  If specified line already commented, "uedit" will try to uncomment it
 *
 */

#include <list>
#include <cmdline.hpp>
#include "json.hpp"
#include "cpphelpers.h"
#include "src/editor/Search.h"
#include "src/editor/Editor.h"

using namespace std;

using json = nlohmann::json;

int edit(int argc, char** argv, cmdline::parser* parser, std::string& file, std::string& searchQuery);
int search(int argc, char** argv, cmdline::parser* parser, std::string& file, std::string& searchQuery);
int comment(int argc, char** argv, cmdline::parser* parser, std::string& file);

enum Command {
	EDIT,
	SEARCH,
	COMMENT
};

const map<string, Command> commands = {
		make_pair("edit", Command::EDIT),
		make_pair("e", Command::EDIT),
		make_pair("search", Command::SEARCH),
		make_pair("s", Command::SEARCH),
		make_pair("comment", Command::COMMENT),
		make_pair("c", Command::COMMENT),
};

bool getCommand(const vector<string>& vars, Command& found, int* position)
{
	int i = 0;
	for (auto& r: vars) {
		for (auto& m: commands) {
			if (m.first==r) {
				*position = i;
				found = m.second;
				return true;
			}
		}
		i++;
	}

	found = Command::EDIT;
	return false;
}

const string commandUsage()
{
	std::ostringstream str;
	str << "\n\ncommands:\n";
	str << "  edit    (default)     - can be empty, editing config\n";
	str << "  search  (shortcut: s) - to search certain line (Indistinct search)\n";
	str << "  comment (shortcut: c) - to comment/uncomment config line\n";

	return str.str();
}

const string topLine(const char* bin)
{
	std::ostringstream str;
	str << "[optional:command...] [filePath...] [optional:search...] [options]\n\n";
	str << "usage example:\n";
	str << "Edit:\n";
	str <<
	    "    " << bin << " /path/to/config.ini -k post_max_size -v 1G\n"
			    "    " << bin << " edit /path/to/config.ini -l 4 -v Off\n"
			    "    " << bin
	    << " /path/to/custom.config post_max_size -v \"post_max_size=1G\" (example how to overwrite whole found line)\n";

	str << "Search:\n";
	str << "    " << bin << " search /path/to/config.ini stringToFind\n";
	str << "    " << bin << " search /path/to/config.ini \"escaped string\"\n";

	str << "Comment:\n";
	str <<
	    "    " << bin << " comment /path/to/config.ini -l 45\n"
			    "    " << bin << " comment /path/to/config.ini post_max_size\n"
			    "    " << bin << " comment /path/to/custom.superconfig my_search_line -c \"//\"";

	str << endl;
	return str.str();
}

int main(int argc, char **argv) {

	cmdline::parser parser;

	parser.setUsageTopLine(topLine(*argv));
	parser.setOptionsPrefix("global");
	parser.footer(commandUsage());

	parser.add("help", 'h', "Use: uedit [command] -h[--help] to see command options");

	//command

	parser.parse(argc, argv);

	unsigned long optionsSize = parser.rest().size();
	if (optionsSize<1) {
		cout << parser.usage() << endl;
		return 0;
	}

	Command command;
	string file;
	string searchQuery;

	int commandPosition = 0;
	bool hasModeArg = getCommand(parser.rest(), command, &commandPosition);

	int optionPositionOffset = static_cast<int>(hasModeArg);
	if (optionsSize-1>=optionPositionOffset) {
		file = parser.rest()[(int) hasModeArg];
	}
	optionPositionOffset++;

	if (optionsSize-1>=optionPositionOffset) {
		searchQuery = parser.rest()[optionPositionOffset];
		if (!searchQuery.empty() && searchQuery.c_str()[0]=='-') {
			searchQuery.clear();
		}
	}

	int retval = 0;
	switch (command) {
	case EDIT: retval = edit(argc, argv, &parser, file, searchQuery);
		break;
	case SEARCH: retval = search(argc, argv, &parser, file, searchQuery);
		break;
	case COMMENT: retval = comment(argc, argv, &parser, file);
		break;
	default: cerr << "Unknown command" << endl;
		cerr << commandUsage() << endl;
		retval = 1;

	}

	return retval;
}

int edit(int argc, char** argv, cmdline::parser* parser, std::string& file, std::string& searchQuery)
{
	parser->setOptionsPrefix("edit");
	parser->add<string>("key", 'k', "Key", false);
	parser->add<string>("value", 'v', "String value", true);
	parser->add<int>("line", 'l', "Line number (use instead key, if you know exact line)", false);
	parser->add<bool>("force", 'f', "Overwrite existed value", false, false);

	if (parser->exist("help")) {
		cout << parser->usage() << endl;
		return 0;
	}

	parser->parse_check(argc, argv);

	string value = parser->get<string>("value");
	bool force = parser->exist("force") && parser->get<bool>("force");
	long line = -1;

	string searchResult;
	if (!parser->exist("key") && !searchQuery.empty()) {
		Search s(file, 0);

		if (s.find(searchQuery, searchResult, nullptr)<1) {
			cerr << "Can't find \"" << searchQuery << "\" in " << "\"" << file << "\"" << endl;
			return 0;
		}

		line = s.getResultLine();
	}

	Editor editor(file);

	if (!parser->exist("key") && !searchResult.empty()) {
		cout << searchResult << endl;
	}

	stringstream ss;
	ss << "Set value \"";
	ss << value;
	ss << "\"";
	if (parser->exist("key")) {
		ss << " to key " << parser->get<string>("key");
	}
	else if (parser->exist("line") || line>0) {
		ss << " to line \"" << line << "\"";
	} else {
		ss << " to whole file";
	}
	ss << "?";

	if (!force) {
		if (!cpphelpers::console::confirm(cin, cout, ss.str())) {
			cout << "\nEdit canceled" << endl;
			return 0;
		}
	}

	if (parser->exist("key")) {
		string key = parser->get<string>("key");
		editor.set(key, value);
	}
	else if (parser->exist("line") || line>0) {
		editor.set(line, value);
	}
	else {
		editor.set(value);
	}

	return 0;
}

int search(int argc, char** argv, cmdline::parser* parser, std::string& file, std::string& searchQuery)
{
	parser->setOptionsPrefix("search");

	parser->add<int>("buffer",
			'b',
			"Buffer size for search - Number of lines to show around found config line",
			false,
			3);

	parser->add<bool>("ignore-case", 'i', "Search ignore case", false, false);
	parser->add<bool>("no-long-lines", 'c', "Cut long lines to 120 characters", false, false);

	if (parser->exist("help")) {
		cout << parser->usage() << endl;
		return 0;
	}

	if (file.empty()) {
		cerr << "File path is empty!" << endl;
		return 0xFF;
	}

	if (searchQuery.empty()) {
		cerr << "Search query is empty!" << endl;
		return 0xFF;
	}

	Search search(file, parser->get<int>("buffer"));
	search.setIgnoreCase(parser->exist("ignore-case") || parser->get<bool>("ignore-case")!=0);
	search.setClipLongLines(parser->exist("no-long-lines") || parser->get<bool>("no-long-lines")!=0);

	parser->parse(argc, argv);

	bool ne = parser->exist("no-long-lines");
	bool nb = parser->get<bool>("no-long-lines")!=0;

	std::string res;
	long found = search.find(searchQuery, res, nullptr);

	if (found<0) {
		cout << "Nothings found" << endl;
		return 1;
	}

	cout << res << endl;

	return 0;
}
int comment(int argc, char** argv, cmdline::parser* parser, std::string& file)
{
	parser->setOptionsPrefix("comment");
	parser->add<string>("char",
			'c',
			"Character for commenting. By default, \"uedit\" try to determine how to comment file, if not, prompt will ask you for required character",
			false);
	return 0;
}
