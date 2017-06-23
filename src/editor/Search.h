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

class Search {
  public:
	struct SearchResult {
	  explicit SearchResult(unsigned long line) {
		  this->line = line;
	  }
	  unsigned long line;
	  std::list<std::string> rowsBuffer;
	};

	explicit Search(std::string &file, const unsigned int padding) : file(file),
	                                                                 bufferPadding(padding) {

	}

	Search(const Search &&from) noexcept:
		file(from.file),
		input(from.input),
		bufferPadding(from.bufferPadding) {

		if (this == &from) {
			return;
		}

		for (auto &r: results) {
			delete r;
		}

		results.clear();
		for (auto &r: from.results) {
			results.push_back(r);
		}

		for (auto &r: from.results) {
			delete r;
		}

		from.results.clear();
	}

	Search(const Search &from) noexcept:
		file(from.file),
		input(from.input),
		bufferPadding(from.bufferPadding) {
		for (auto &r: results) {
			delete r;
		}
		results.clear();
		for (auto &r: from.results) {
			results.push_back(r);
		}
	}

	~Search() {
		for (auto &r: results) {
			delete r;
		}

		results.clear();
	}

	/**
	 *
	 * @param query
	 * @param result
	 * @return Size of found items or -1 if error occurred
	 */
	long find(const std::string &query, std::string &result, std::string *error = nullptr);
	const std::vector<SearchResult *> getResults();
	const long getResultsCount();

  private:
	unsigned int getBufferSize();

	std::string file;
	std::ifstream input;
	unsigned int bufferPadding = 3;
	std::vector<SearchResult *> results;
};

#endif //UEDIT_SEARCH_H
