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
 * uedit [mode: edit, search(s), comment(c)] !filename! [options]
 *
 * global options:
 *  -f,--force    force, overwrite all, be careful!
 *  -h,-?         this help
 *
 *
 * edit or empty mode:
 *  Example:
 *      ./uedit /path/to/config.ini -k post_max_size -v 1G
 *      ./uedit edit /path/to/config.ini -l 4 -v Off
 *      ./uedit /path/to/custom.config -s post_max_size -v "post_max_size=1G" (example how to overwrite whole found line)
 *
 *  -v,--value    value to set
 *  -k,--key      key
 *  -s,--search   search specified string in file and set to found line value from -v
 *  -l,--line     line where set value (-v)
 *  if not set any of option: -k,-s or -l the file will clear and value set to first line like command: echo "value" > /path/to/file.cfg
 *
 * search (shortcut: s):
 *  Example:
 *      ./uedit search /path/to/config.ini -s stringToFind
 *  -b,--buffer    Number of lines to show around found row
 *
 * comment (shortcut: c):
 *  Example:
 *      ./uedit comment /path/to/config.ini -l 45
 *      ./uedit comment /path/to/config.ini -s post_max_size
 *      ./uedit comment /path/to/custom.superconfig -s my_custom_line -c "//"
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
#include "cmdline.hpp"
#include "json.hpp"
#include "Strings.hpp"
#include "src/editor/Editor.h"

using namespace cmdline;
using namespace std;
using arg =

using json = nlohmann::json;

int edit(int argc, char **argv);
int search(int argc, char **argv);
int comment(int argc, char **argv);

int main(int argc, char **argv) {
	auto *args = new cmdline::parser();
	args->set_program_name("UEDIT 1.0");

	delete args;
	return 0;
}

int edit(cmdline::parser *args, int argc, char **argv) {

	int line = -1;
	bool overwrite = true;

	args->add<string>("input", 'i', "Input file", true);
	args->add<string>("key", 'k', "Key - config key", false);
	args->add<string>("value", 'v', "Value", true);
	args->add<int>("line", 'l', "Line number (use instead key, if you know exact line)", false, line);
	args->add<bool>("force", 'f', "Overwrite existed value", false, overwrite);

	args->parse_check(argc, argv);

	if (!args->exist("input")) {
		return 1;
	}

	string input = args->get<string>("input");
	string val = args->get<string>("value");

	auto *editor = new Editor(input);

	if (args->exist("key")) {
		string key = args->get<string>("key");
		editor->set(key, val);
	} else if (args->exist("line")) {
		editor->set(args->get<int>("line"), val);
	} else {
		editor->set(val);
	}

	delete args, editor;
}
int search(int argc, char **argv) {
	string s = "post";
	string filename = "/etc/php.ini.default";
	ifstream f(filename);
	if (!f.is_open() || f.bad()) {
		perror("error while opening file");
		return 1;
	}

	string row;
	const int padding = 3;
	const int bufferSize = padding * 2 + 1; // top lines + bottom lines + found line

	unsigned long line = 1;
	vector<SearchResult *> results;
	int unbufferedResults = 0;

	deque<string> buffer;

	while (std::getline(f, row)) {

		if (buffer.size() > padding + 1) {
			// remove top element, to prevent buffer overflowing
			buffer.pop_front();
		}
		// add bottom element
		buffer.push_back(row);


		// if in results list has a rows, where buffered lines not filled enought (see bufferSize)
		if (unbufferedResults > 0) {
			for (auto &r: results) {
				if (r->rowsBuffer.size() < bufferSize) {
					r->rowsBuffer.push_back(row);
					// if after adding row buffer size filled enought, decrement count of unbuffered items
					if (r->rowsBuffer.size() == bufferSize) {
						unbufferedResults--;
					}
				}
			}
		}

		if (Strings::hasSubstring(row, s)) {
			results.push_back(new SearchResult(line));
			unbufferedResults++;

			// fill result's buffered rows, put last (padding) lines + 1 (found row)
			size_t idx = buffer.size() - 1;
			while (results.back()->rowsBuffer.size() < padding + 1) {
				if (idx == 0) break;
				results.back()->rowsBuffer.push_front(buffer[idx]);
				idx--;
			}
		}

		line++;
	}

	cout << "Searching for \"" << s << "\" in \"" << filename << "\"" << endl;
	for (auto &r: results) {
		cout << "Found line: " << r->line << endl;
		int i = (-padding);
		while (!r->rowsBuffer.empty()) {
			cout << (r->line + i) << "\t";
			if ((r->line + i) == r->line) {
				cout << "--> " << r->rowsBuffer.front() << endl;
			} else {
				cout << r->rowsBuffer.front() << endl;
			}
			r->rowsBuffer.pop_front();
			i++;

			if ((r->line + i) >= line) {
				cout << "--EOF--" << endl;
			}
		}

		cout << endl;

		delete r;
	}
}
int comment(int argc, char **argv) {

}
