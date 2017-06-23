/**
 * uedit
 * ConfParser.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef UEDIT_CONFPARSER_H
#define UEDIT_CONFPARSER_H

#include "FileParser.h"

class ConfParser : public FileParser {
  public:
	explicit ConfParser(std::string &file);
	~ConfParser();
	void set(const std::string &key, const std::string &value) override;
	void save() override;

};

#endif //UEDIT_CONFPARSER_H
