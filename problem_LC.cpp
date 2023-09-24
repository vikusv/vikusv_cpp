#include <cassert>
#include <iostream>

#include <iterator>
#include <list>
#include <unordered_map>
#include "cache.hpp"

int slow_get_page_int(int key)
{
    return key;
}

int main()
{
    int hits = 0;
    int n = 0;
    size_t m = 0;

    std::cin >> m >> n;
    assert(std::cin.good());
    caches::cache_2Q<int> c(m);

    for (int i = 0; i < n; ++i)
    {
        int q = 0;
        std::cin >> q;
        assert(std::cin.good());
        if (c.lookup_update(q, slow_get_page_int))
            hits += 1;
        //printf("hits = %d\n", hits);
    }

    std::cout << hits << std::endl;
}