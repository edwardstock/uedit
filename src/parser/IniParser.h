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
#include <vector>
#include <regex>
#include <cpphelpers.h>
#include <map>
#include <unordered_map>

#ifndef u_long
#define u_long unsigned long
#endif

using namespace std;

class IniValue {
public:
	IniValue(string& value)
			:value(std::move(value))
	{
	}

	IniValue(const char* value)
			:value(string(value))
	{

	}

	IniValue()
			:value(string())
	{
	}

	~IniValue()
	{
	}

	string get() const
	{
		return value;
	}

	int getInt() const
	{
		int out;
		try {
			out = std::stoi(get());
		}
		catch (std::invalid_argument& e) {
			out = 0;
		}

		return out;
	}

	long getLong() const
	{
		long out;
		try {
			out = std::stol(get());
		}
		catch (std::invalid_argument& e) {
			out = 0L;
		}

		return out;
	}

	double getReal() const
	{
		double out;
		try {
			out = std::stod(get());
		}
		catch (std::invalid_argument& e) {
			out = 0;
		}

		return out;
	}

	bool getBool() const
	{
		return get()=="1" || get()=="true";
	}

	bool operator==(const IniValue& another)
	{
		return get()==another.get();
	}

	bool operator==(const long another)
	{
		return getLong()==another;
	}

	bool operator==(const double another)
	{
		return getReal()==another;
	}

	bool operator==(const int another)
	{
		return getInt()==another;
	}

	bool operator==(const bool another)
	{
		return getBool()==another;
	}

	friend ostream& operator<<(ostream& os, const IniValue& val)
	{
		os << val.get();
		return os;
	}

private:
	string value;
	string newValue;
};

class IniRow {
public:
	IniRow(const string& key, IniValue&& value, u_long line)
			:
			IniRow(key, line)
	{
		addValue(value);
	}

	IniRow(const string& key, u_long line)
			:key(key), line(line)
	{
	}

	// move
	IniRow(IniRow&& another) noexcept
	{
		*this = std::move(another);
	}

	// copy
	IniRow(const IniRow& another)
	{
		*this = another;
	}

	IniRow& operator=(IniRow&& another) noexcept
	{
		key = std::move(another.key);
		value = std::move(another.value);
		line = another.line;
		commented = another.commented;

		return *this;
	}

	IniRow& operator=(const IniRow& another)
	{
		if (this==&another) {
			return *this;
		}

		key = another.key;
		line = another.line;
		commented = another.commented;

		value.clear();
		value.reserve(another.value.size());
		std::copy(another.value.begin(), another.value.end(), back_inserter(value));

		return *this;
	}

	bool operator==(const IniRow& another) const
	{
		return getKey()==another.getKey()
				&& getValue()==another.getValue()
				&& getLine()==another.getLine();
	}

	bool operator>(const IniRow& another) const
	{
		return line>another.line;
	}

	bool operator<(const IniRow& another) const
	{
		return !(*this>another);
	}

	~IniRow()
	{
		destroy();
	}

	void setIsCommented(bool isCommented)
	{
		commented = isCommented;
	}

	bool isCommented()
	{
		return commented;
	}

	void addValue(IniValue&& val)
	{
		value.push_back(val);
	}

	void addValue(const IniValue& val)
	{
		value.push_back(val);
	}

	bool isArray() const
	{
		return value.size()>1;
	}

	string getKey() const
	{
		return key;
	}

	u_long getLine() const
	{
		return line;
	}

	IniValue getValue() const
	{
		if (value.empty()) {
			return "";
		}

		return value[0];
	}

	vector<IniValue> getValues() const
	{
		return value;
	}

private:
	u_long line;
	string key;
	vector<IniValue> value;
	bool commented = false;

	void destroy()
	{
	}
};

class IniSection {
public:

	IniSection(const string& n)
	{
		name = n;
	}

	~IniSection()
	{
		rows.clear();
	}

	string getName() const
	{
		return name;
	}

	vector<IniRow> getRows() const
	{
		return rows;
	}

	void addRow(IniRow&& row)
	{
		rows.push_back(row);
	}

	bool hasRowKey(const IniRow& row) const
	{
		for (auto& r: rows) {
			if (r.getKey()==row.getKey()) return true;
		}

		return false;
	}

	bool hasRowKey(const string& key) const
	{
		for (auto& r: rows) {
			if (r.getKey()==key) return true;
		}

		return false;
	}

	bool hasRow(const IniRow& row) const
	{
		for (auto& r: rows) {
			if (r==row) return true;
		}

		return false;
	}

	bool hasRow(const string& key)
	{
		for (auto& r: rows) {
			if (r.getKey()==key) {
				return true;
			}
		}

		return false;
	}

	bool operator==(const IniSection& section)
	{
		return getName()==section.getName();
	}

	bool operator==(const string& name)
	{
		return getName()==name;
	}

	IniRow* operator[](const char* key)
	{
		return (*this)[string(key)];
	}

	IniRow* operator[](const string& key)
	{
		return getRow(key);
	}

	IniRow* getRow(const IniRow& anotherRow)
	{
		return getRow(anotherRow.getKey());
	}

	IniRow* getRow(const string& key)
	{
		for (auto& r: rows) {
			if (r.getKey()==key) {
				return &r;
			}
		}

		return nullptr;
	}

private:
	string name;
	vector<IniRow> rows;
};

class IniParser : public FileParser {

public:
	IniParser(std::string& file);
	~IniParser();
	void set(const std::string& key, const std::string& value) override;
	void save() override;
	bool hasSection(const string& name)
	{
		return sections.count(name)>0;
	}
	bool hasSection(const IniSection* section)
	{
		return hasSection(section->getName());
	}

	void dump()
	{
		for (auto& s: sections) {
			cout << "[" << s.second->getName() << "]" << endl;
			for (auto& r: s.second->getRows()) {
				cout << "    ";
				for (auto& v: r.getValues()) {
					if (r.isCommented()) cout << ";";
					cout << r.getKey();
					if (r.isArray()) {
						cout << "[]=";
					}
					else {
						cout << "=";
					}

					cout << v.get() << endl;
				}
			}
		}
	}
private:
	std::unordered_map<string, IniSection*> sections;
	vector<IniRow*> rows;
	void parse();

};

#endif //UEDIT_INIPARSER_H
