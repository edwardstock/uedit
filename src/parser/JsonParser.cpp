/**
 * uedit
 * JsonParser.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#include "JsonParser.h"

using namespace std;
JsonParser::JsonParser(std::string &file) : FileParser(file) {
	std::ifstream in(file);
	in >> object;
}

JsonParser::~JsonParser() {

}

void JsonParser::set(const std::string &key, const std::string &value) {
	vector<string> path = Strings::split(key, '.');

	if (path.size() == 1) {
		inferredValue(object[key], value);
		return;
	}

	vector<json> elements = vector<json>();

	unsigned long idx = 0;

	for (auto it = path.begin(); it != path.end(); it++) {
		string subKey = (*it);
		bool isLast = idx == path.size() - 1;

		if (idx == 0) {
			elements.push_back(object);
			json &prev = elements.back();
			// check first element exists with first segment of key, if not exists or null, setting it to new object
			if (prev[subKey].is_null()) {
				prev[subKey] = json::object();
			}
		} else {
			// moving to next segment
			elements.push_back(elements.back());
			// getting prev segment of path, where we will write our current segment
			// for example key: sub.key, where "sub" is a first segment, "key" - second (sub segment)
			// we taking previous segment object {"sub":{}} and putting to sub:{"key": {}} - sub element with current segment path
			string prevKey = path[idx - 1];
			json &prev = elements.back();

			// than checking, if not the last segment, jus create new object, otherwise setting value to current
			if (not isLast and prev[prevKey][subKey].is_null()) {
				prev[prevKey][subKey] = json::object();
			} else {
				inferredValue(prev[prevKey][subKey], value);
			}
		}

		idx++;
	}

	object = elements.back();
}

void JsonParser::save() {
	std::ofstream of(getFilePath());
	of << std::setw(4) << object << std::endl;
}

void JsonParser::inferredValue(json &element, std::string value) {
	try {
		if (value == "null") {
			element = nullptr;
		} else if (Numbers::isInteger(value)) {
			element = std::stoi(value);
		} else if (Numbers::isReal(value)) {
			element = std::stof(value);
		} else {
			element = value;
		}
	} catch (std::out_of_range const &e) {
		cerr << "Number is out of range. Writing string instead" << endl;
		element = value;
	}
}

