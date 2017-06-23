/**
 * series_media_server
 * StringHelper.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef UEDDIT_STRINGS_HPP
#define UEDDIT_STRINGS_HPP

#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include "json.hpp"

using namespace std;

class Strings {
  public:
	struct StringHelperException : public std::exception {
	  std::string s;
	  StringHelperException(const std::string ss) : s(ss) {};
	  ~StringHelperException() throw() {};
	  const char *what() const throw() { return s.c_str(); };
	};

	static bool hasSubstring(const string &source, const string &substring) {
		return source.find(substring) != string::npos;
	}

	static bool hasSubstring(const string &source, const char substring) {
		return source.find(substring) != string::npos;
	}

	static pair<string, string> splitPair(const string &source, const string &delimiter) {
		if (delimiter.size() < 1) {
			throw StringHelperException("Delimiter is empty");
		}
		return splitPair(source, delimiter.c_str()[0]);
	}

	static pair<string, string> splitPair(const string &source, const char &delimiter) {
		vector<string> elems = split(source, delimiter);

		if (elems.size() == 0) {
			return pair<string, string>(string(), string());
		} else if (elems.size() == 1) {
			return pair<string, string>(elems.at(0), string());
		}

		return pair<string, string>(elems.at(0), elems.at(1));
	}

	static vector<string> split(const string &source, const string &delimiter) {
		if (delimiter.size() < 1) {
			throw StringHelperException("Delimiter is empty");
		}

		return split(source, delimiter.c_str()[0]);
	}
	static vector<string> split(const string &source, const char &delimiter) {
		stringstream ss;
		ss.str(source);
		std::string item;
		vector<string> elements;
		while (std::getline(ss, item, delimiter)) {
			elements.push_back(item);
		}

		return elements;
	}

	static void removeSubstrings(string &s, const string &p) {
		std::string::size_type n = p.length();

		for (string::size_type i = s.find(p); i != string::npos; i = s.find(p)) {
			s.erase(i, n);
		}
	}

	static void removeSubstrings(string &s, vector<string> replaces) {
		for (auto it = replaces.begin(); it != replaces.end(); ++it) {
			removeSubstrings(s, *it);
		}
	}

	static std::string toString(int32_t in) {
		std::stringstream ss;
		ss << in;
		return ss.str();
	}

	static std::string toString(int64_t in) {
		std::stringstream ss;
		ss << in;
		return ss.str();
	}

	static std::string toString(nlohmann::json &jsonObj) {
		return jsonObj.dump();
	}

	static std::string toString(ifstream &inputStream) {
		return std::string((istreambuf_iterator<char>(inputStream)), istreambuf_iterator<char>());
	}

	static vector<vector<string>> matchAllRegexp(const string &s, const regex &rx) {
		vector<vector<string>> capturedGroups;
		vector<string> capturedSubgroups;
		const sregex_token_iterator endIterator;
		for (sregex_token_iterator it(s.cbegin(), s.cend(), rx); it != endIterator; ++it) {
			capturedSubgroups.clear();
			string group = *it;
			smatch res;
			if (regex_search(group, res, rx)) {
				for (auto r : res) {
					capturedSubgroups.push_back(move(r));
				}

				if (!capturedSubgroups.empty()) {
					capturedGroups.push_back(capturedSubgroups);
				}
			}

		}
		capturedGroups.push_back(capturedSubgroups);
		return capturedGroups;
	}

	static vector<vector<string>> matchAllRegexp(const string &s, const string &rxPattern) {
		return matchAllRegexp(s, regex(rxPattern, regex_constants::icase));
	}

};

#endif //UEDDIT_STRINGS_HPP
