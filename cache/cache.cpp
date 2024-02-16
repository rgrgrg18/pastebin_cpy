#ifndef __CACHE_H__
#define __CACHE_H__
#include <cstddef>
#include <list>
#include <unordered_map>

namespace cache {
	template <typename T, typename KeyT = int>
	class LRU {
	public:
		size_t size;
		std::list<T> cache;

		using ListIt = typename std::list<T>::iterator;
		std::unordered_map<KeyT, ListIt> hash;
	public:
		explicit LRU(size_t size): size(size) {}

		LRU() = delete;
		LRU(const LRU& other) = delete;
		LRU& operator=(const LRU& other) = delete;	
	private:
		bool full() const {
			return (cache.size() == size);
		}

	public:
		template <typename U, typename... Types>
		T get (const KeyT& key, U slow_get, Types... args) {
			auto bucket = hash.find(key);
			if (bucket == hash.end()) {
				if (full()) {
					hash.erase(cache.back());
					cache.pop_back();
				}
				T cached_item = slow_get(args..., key);
				cache.push_front(cached_item);
				hash[key] = cache.begin();
				return cached_item;
			}
			auto cache_it = bucket->second;
			if (cache_it != cache.begin()) 
				cache.splice(cache.begin(), cache, cache_it, std::next(cache_it));
			return *cache_it;
		}
	};		
}
// string and map can throw exceptions


#endif