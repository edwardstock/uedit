/**
 * uedit
 * JsonParser.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef UEDIT_JSONPARSER_H
#define UEDIT_JSONPARSER_H

#include "FileParser.h"
#include "json.hpp"
#include <fstream>
#include <regex>

using json = nlohmann::json;

class JsonParser : public FileParser {
  public:
	explicit JsonParser(std::string &file);
	~JsonParser();
	void set(const std::string &key, const std::string &value) override;
	void save() override;

  private:
	json object;

	void inferredValue(json &element, std::string value);

};

#endif //UEDIT_JSONPARSER_H
