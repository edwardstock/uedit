/**
 * uedit
 * Editor.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#include "Editor.h"
#include "../parser/IniParser.h"
#include "../parser/JsonParser.h"
#include "../parser/ConfParser.h"
Editor::Editor(std::string &filePath) {

	if (Strings::hasSubstring(filePath, ".ini")) {
		fileParser = new IniParser(filePath);
		((IniParser*) fileParser)->dump();
	} else if (Strings::hasSubstring(filePath, ".json")) {
		fileParser = new JsonParser(filePath);
	} else {
		fileParser = new ConfParser(filePath);
	}
}

Editor::~Editor() {
	delete fileParser;
}

void Editor::set(long line, std::string& value)
{

}
void Editor::set(std::string &key, std::string &value) {
	fileParser->set(key, value);
	fileParser->save();
}
void Editor::set(std::string &value) {

}

