#ifndef __CACHE_H__
#define __CACHE_H__
#include <cstddef>
#include <list>
#include <unordered_map>
#include <iostream>

namespace cache {
	template <typename T, typename KeyT = int>
	class LFU {
	public:		
		using listIt = typename std::list<KeyT>::iterator;

		size_t size;
		size_t min_freq = 1;

		struct object {
			T value;
			size_t freq;
			listIt it; 
		};

		std::unordered_map<KeyT, object> key_object;
		std::unordered_map<size_t, std::list<KeyT>> freq_key;
	public:
		explicit LFU(size_t size): size(size) {}

		LFU() = delete;
		LFU(const LFU& other) = delete;
		LFU& operator=(const LFU& other) = delete;

	private:
		bool full() const {
			return (key_object.size() == size);
		}

	public:
		std::pair<T, bool> get(KeyT key) {
			auto objectIt = key_object.find(key);
			if (objectIt == key_object.end()) {
				return {T(), false};
			}

			freq_key[objectIt->second.freq].erase(objectIt->second.it);
			++objectIt->second.freq;
						
			freq_key[objectIt->second.freq].push_front(key);
			objectIt->second.it = freq_key[objectIt->second.freq].begin();

			min_freq += freq_key[min_freq].empty();

			return {objectIt->second.value, true};
		}

		void insert(KeyT key, T&& value) {
			if (full()) {
				auto it_min_freq = freq_key.find(min_freq);
				key_object.erase(it_min_freq->second.back());
				it_min_freq->second.pop_back();
			}

			freq_key[1].push_front(key);
			key_object.insert({key, {value, 1, freq_key[1].begin()}});

			min_freq = 1;
		}
	};
}

#endif