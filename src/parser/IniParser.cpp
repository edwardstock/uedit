/**
 * uedit
 * IniParser.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#include "IniParser.h"
IniParser::IniParser(std::string &file) : FileParser(file) {
	parse();
}
IniParser::~IniParser() {
}
void IniParser::set(const std::string &key, const std::string &value) {

}
void IniParser::save() {

}
void IniParser::parse()
{
	std::regex sectionReg(R"(\[{1,1}(.+)\]{1,1})"); // group 0: full match, 1: section
	/*
	 * (?:\;|\#)          - optional: comment before line (; or #)
	 * ([a-z0-9_\-\+\.]+) - required: key
	 * ?(?:\[\])?         - optional: square braces
	 * (?:[ ]*)           - optional: space char[0,] before "="
	 * \=                 - required: equal mark
	 * (?:[ ]*)           - optional: space char[0,] after "="
	 * (.*)               - required: value
	 *
	 * Matches:
	 * group 0: full match, 1: key, 2: value
	 */
	std::regex rowReg(R"((?:\;|\#)([a-z0-9_\-\+\.]+)?(?:\[\])?(?:[ ]*)\=(?:[ ]*)(.*))"); //

	ifstream input(getFilePath());
	string row;
	u_long line = 1;

	string lastSection;
	while (std::getline(input, row)) {
		if (row.empty()) {
			line++;
			continue;
		}

		// searching for section pattern in row
		string mSection = Strings::matchRegexpFirst(sectionReg, row);

		if (!mSection.empty() && row.c_str()[0]==';') {
			line++;
			continue;
		}

		// searching for key([])=value pattern in row
		smatch mRow = Strings::matchRegexp(rowReg, row);

		// if is not a section and key=value detected and size of matches is correct, writing value to section
		if (mSection.empty() && !mRow.empty() && mRow.size()==3) {
			string key, value;
			key = mRow[1];
			value = mRow[2];

			// if no one section were found, we create hidden __default__ section for consistency
			if (lastSection.empty()) {
				lastSection = "__default__";
				sections[lastSection] = new IniSection(lastSection);
			}

			IniRow iniRow(key, IniValue(value), line);
			iniRow.setIsCommented(Strings::hasRegex(R"((?:\;|\#))", row));
			rows.push_back(&iniRow);
			if (sections[lastSection]->hasRowKey(iniRow) && Strings::hasRegex(R"((\[\]))", row)) {
				// We found array. Why? In current section already added a value with the same key
				sections[lastSection]
						->getRow(iniRow)
						->addValue(IniValue(value));
			}
			else {
				sections[lastSection]
						->addRow(std::move(iniRow));
			}

		}
		else if (!mSection.empty()) {
			lastSection = mSection.empty() ? "__default__" : mSection;
			sections[lastSection] = new IniSection(lastSection);
		}

		line++;
	}
}

