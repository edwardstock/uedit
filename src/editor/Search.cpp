/**
 * uedit
 * Search.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#include "Search.h"
#include <Strings.hpp>

long Search::find(const std::string &query, std::string &result, std::string *error = nullptr) {
	input.open(file);
	if (!input.is_open() || input.bad()) {
		*error = "error while opening file";
		return -1;
	}

	unsigned long line = 1;
	int unbufferedResults = 0;
	std::string row;

	std::stringstream resultStream;

	deque<string> buffer;

	while (std::getline(input, row)) {
		if (buffer.size() > bufferPadding + 1) {
			// remove top element, to prevent buffer overflowing
			buffer.pop_front();
		}
		// add bottom element
		buffer.push_back(row);


		// if in results list has a rows, where buffered lines not filled enought (see bufferSize)
		if (unbufferedResults > 0) {
			for (auto &r: results) {
				if (r->rowsBuffer.size() < getBufferSize()) {
					r->rowsBuffer.push_back(row);
					// if after adding row buffer size filled enought, decrement count of unbuffered items
					if (r->rowsBuffer.size() == getBufferSize()) {
						unbufferedResults--;
					}
				}
			}
		}

		if (Strings::hasSubstring(row, query)) {
			results.push_back(new SearchResult(line));
			unbufferedResults++;

			// fill result's buffered rows, put last (padding) lines + 1 (found row)
			size_t idx = buffer.size() - 1;
			while (results.back()->rowsBuffer.size() < bufferPadding + 1) {
				if (idx == 0) break;
				results.back()->rowsBuffer.push_front(buffer[idx]);
				idx--;
			}
		}

		line++;
	}

	resultStream << "Searching for \"" << query << "\" in \"" << file << "\"" << endl;
	for (auto &r: results) {
		resultStream << "Found line: " << r->line << endl;
		int i = (-bufferPadding);
		while (!r->rowsBuffer.empty()) {
			resultStream << (r->line + i) << "\t";
			if ((r->line + i) == r->line) {
				resultStream << "--> " << r->rowsBuffer.front() << endl;
			} else {
				resultStream << r->rowsBuffer.front() << endl;
			}
			r->rowsBuffer.pop_front();
			i++;

			if ((r->line + i) >= line) {
				resultStream << "--EOF--" << endl;
			}
		}

		resultStream << endl;
	}

	result = resultStream.str();

	return results.size();
}
unsigned int Search::getBufferSize() {
	return bufferPadding * 2 + 1;
}

const std::vector<SearchResult *> Search::getResults() {
	return results;
}

const long Search::getResultsCount() {
	return results.size();
}

