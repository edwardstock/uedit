/**
 * uedit
 * Editor.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef UEDIT_EDITOR_H
#define UEDIT_EDITOR_H

#include <string>
#include "../parser/FileParser.h"
class Editor {
  private:
	FileParser *fileParser;

  public:
	explicit Editor(std::string &filePath);
	~Editor();

	void set(int line, std::string &value);
	void set(std::string &key, std::string &value);
	void set(std::string &value);
};

#endif //UEDIT_EDITOR_H
