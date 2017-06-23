/**
 * uedit
 * FileParser.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef UEDIT_FILEPARSER_H
#define UEDIT_FILEPARSER_H

#include <string>
class FileParser {
  public:
	explicit FileParser(std::string &file) :
		filePath(file) {
	}

	FileParser(const FileParser &another) = default;
	FileParser(FileParser &&another) = default;

	virtual ~FileParser() = default;
	virtual void set(const std::string &key, const std::string &value) = 0;
	virtual void save() = 0;

	std::string find(std::string &key) {
		return nullptr;
	}

  protected:
	const std::string &getFilePath() {
		return filePath;
	}

  private:
	std::string &filePath;
};

#endif //UEDIT_FILEPARSER_H
