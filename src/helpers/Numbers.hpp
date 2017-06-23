/**
 * uedit
 * NumberUtils.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef UEDIT_NUMBERUTILS_H
#define UEDIT_NUMBERUTILS_H

#include <string>
#include <regex>
class Numbers {
  public:
	static bool isInteger(const std::string &input) {
		return std::regex_match(input, std::regex(R"(^[-]?[0-9eE+]+?$)"));
	}

	static bool isReal(const std::string &input) {
		return std::regex_match(input, std::regex(R"(^[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$)"));
	}
};

#endif //UEDIT_NUMBERUTILS_H
