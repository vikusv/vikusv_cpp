#pragma once

#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>
#include <cmath>

namespace caches
{
    template <typename T, typename KeyT = int> struct cache_LRU
    {
        size_t sz_;
        std::list<std::pair<KeyT, T >> cache_;

        using ListIt = typename std::list<std::pair<KeyT, T >> ::iterator;
        std::unordered_map<KeyT, ListIt> hash_;

        cache_LRU(size_t sz) : sz_(sz) {

        }

        bool full() const
        {
            return (cache_.size() == sz_);
        }

        template <typename F> bool lookup_update(KeyT key, F slow_get_page)
        {
            auto hit = hash_.find(key);
            if (hit == hash_.end())
            {
                if (full())
                {
                    hash_.erase(cache_.back().first);
                    cache_.pop_back();
                }
                cache_.emplace_front(key, slow_get_page(key));
                hash_.emplace(key, cache_.begin());
                return false;
            }

            auto eltit = hit->second;
            if (eltit != cache_.begin())
                cache_.splice(cache_.begin(), cache_, eltit, std::next(eltit));
            return true;
        }
    };

    template <typename T, typename KeyT = int> struct cache_2Q
    {
        size_t sz_;
        std::list<std::pair<KeyT, T >> queue_in;
        std::list<std::pair<KeyT, T >> queue_main;
        std::list<std::pair<KeyT, T >> queue_out;

        using ListIt = typename std::list<std::pair<KeyT, T >> ::iterator;
        std::unordered_map<KeyT, ListIt> hash_2Q;

        cache_2Q(size_t sz) : sz_(sz) {

        }

        size_t sz_qm = ceil(sz_ * 0.2);
        size_t sz_qin = sz_ - sz_qm; 
        size_t sz_qout = sz_; 

        bool full(const std::list < std::pair < KeyT, T >> & queue, size_t size) const
        {
            return (queue.size() == size);
        }

        template <typename F> bool lookup_update(KeyT key, F slow_get_page)
        {
            auto inf = slow_get_page(key);
            auto hit = hash_2Q.find(key);
            if (hit == hash_2Q.end())
            {
                if (full(queue_in, sz_qin))
                {
                    if (full(queue_out, sz_qout))
                    {
                        hash_2Q.erase(queue_out.back().first);
                        queue_out.pop_back();
                    }
                    queue_out.emplace_front(queue_in.back().first, queue_in.back().second);
                    queue_in.pop_back();
                }
                queue_in.emplace_front(key, inf);
                hash_2Q.emplace(key, queue_in.begin());
                return false;
            }

            
            auto eltit = hit->second;
            auto eltit_num = hit->first;
            if (find_iter(queue_in, eltit_num))
            {
                if (eltit != queue_in.begin() && sz_qin != 1)
                {
                    erase_num(queue_in, eltit_num);
                    queue_in.emplace_front(key, inf);
                }
                return true;
            }
            if (find_iter(queue_main, eltit_num))
            {
                if (eltit != queue_main.begin() && sz_qm != 1)
                {
                    erase_num(queue_main, eltit_num);
                    queue_main.emplace_front(key, inf);
                }
                return true;
            }
            if (find_iter(queue_out, eltit_num))
            {
                if (full(queue_main, sz_qm))
                {
                    hash_2Q.erase(queue_main.back().first);
                    queue_main.pop_back();
                }
                queue_main.emplace_front(key, inf);
                return false;
            }
            return false;
        }

        bool find_iter(const std::list < std::pair < KeyT, T >> & list, int key) const
        {
            for (auto it = list.begin(); it != list.end(); it = std::next(it))
            {
                if (it->first == key)
                {
                    return true;
                }
            }
            return false;
        }

        void erase_num(std::list < std::pair < KeyT, T >> & list, int key)
        {
            auto it = list.begin();
            while (it->first != key) 
            {
                it = std::next(it);
            }
            list.erase(it);
        }
    };
}