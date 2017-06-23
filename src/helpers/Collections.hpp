/**
 * series_media_server
 * CollectionHelper.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef UEDIT_COLLECTIONHELPER_H
#define UEDIT_COLLECTIONHELPER_H
#include <vector>
#include <map>

class Collections {
  public:
	template<typename K, typename V>
	static std::vector<K> mapKeysToVector(const std::map<K, V> &map) {
		std::vector<K> v(map.size());

		for (auto it = map.begin(); it != map.end(); ++it) {
			v.push_back(it->first);
		}

		return v;
	}

	template<typename K, typename V>
	static std::vector<V> mapValuesToVector(const std::map<K, V> &map) {
		std::vector<V> v(map.size());

		for (auto it = map.begin(); it != map.end(); ++it) {
			v.push_back(it->first);
		}

		return v;
	}
};

#endif //UEDIT_COLLECTIONHELPER_H
