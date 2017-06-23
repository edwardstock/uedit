/**
 * uedit
 * LineByLineParser.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef UEDIT_LINEBYLINEPARSER_H
#define UEDIT_LINEBYLINEPARSER_H

#include "FileParser.h"
class LineParser : public FileParser {
  public:
	explicit LineParser(std::string &file);
	~LineParser();
	void set(const std::string &key, const std::string &value) override;
	void save() override;
};

#endif //UEDIT_LINEBYLINEPARSER_H
