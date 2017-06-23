/**
 * uedit
 * IniParser.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef UEDIT_INIPARSER_H
#define UEDIT_INIPARSER_H

#include "FileParser.h"
class IniParser : public FileParser {

  public:
	explicit IniParser(std::string &file);
	~IniParser();
	void set(const std::string &key, const std::string &value) override;
	void save() override;
};

#endif //UEDIT_INIPARSER_H
