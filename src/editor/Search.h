/**
 * uedit
 * Search.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef UEDIT_SEARCH_H
#define UEDIT_SEARCH_H

#include <string>
#include <fstream>
#include <list>
#include <deque>
#include <vector>
#include <cpphelpers.h>
#include <sstream>
#include <functional>
using namespace std;

struct SearchResult {
public:
	explicit SearchResult(unsigned long line)
	{
		this->line = line;
	}
	long line;
	std::list<std::string> rowsBuffer;
};

class Search {
public:
	typedef std::function<bool(const bool icase, const string&, const string&)> SearchComparator;

	static SearchComparator CMP_SUBSTRING;

	explicit Search(std::string& file, const int padding)
			:
			file(std::move(file))
	{
		setComparator(Search::CMP_SUBSTRING);
		if (padding<=0) {
			bufferPadding = 0;
		}
		else {
			bufferPadding = padding;
		}
	}

	Search(const Search&& from) noexcept
			:
			file(from.file),
			bufferPadding(from.bufferPadding)
	{

		if (this==&from) {
			return;
		}

		input.open(file);

		for (auto& r: results) {
			delete r;
		}

		results.clear();
		for (auto& r: from.results) {
			results.push_back(r);
		}

		for (auto& r: from.results) {
			delete r;
		}
	}

	Search(const Search& from) noexcept
			:
			file(from.file),
			bufferPadding(from.bufferPadding)
	{

		input.open(file);
		for (auto& r: results) {
			delete r;
		}
		results.clear();
		for (auto& r: from.results) {
			results.push_back(r);
		}
	}

	~Search()
	{
		for (auto& r: results) {
			delete r;
		}

		results.clear();
	}

	void setIgnoreCase(bool i)
	{
		ignoreCase = i;
	}

	void setClipLongLines(bool c)
	{
		clipLongLines = c;
	}

	bool compare(const string& first, const string& second)
	{
		return comparator(ignoreCase, first, second);
	}

	void setComparator(Search::SearchComparator& searchComparator)
	{
		comparator = searchComparator;
	}

	/**
	 *
	 * @param query
	 * @param result
	 * @return Size of found items or -1 if error occurred
	 */
	long find(const std::string& query, std::string& result, std::string* error = nullptr);
	const std::vector<SearchResult*> getResults();
	const long getResultsCount();
	const long getResultLine();

private:
	Search::SearchComparator comparator;
	int getBufferSize();
	bool ignoreCase = false;
	bool clipLongLines = false;
	std::string file;
	std::ifstream input;
	int bufferPadding = 3;
	std::vector<SearchResult*> results;
	long resultLine = 0;
};

#endif //UEDIT_SEARCH_H
