#include "cache.hpp"


int main()
{
    auto cache_sz = 0, input_sz = 0;
    std::cin >> cache_sz >> input_sz;

    LRU_cache_t<page_t> cache{cache_sz};

    int hits = 0;
    for (int i = 0; i < input_sz; i++)
    {
        int page_id;
        std::cin >> page_id; 
        assert(std::cin.good());
        if (cache.lookup_update(page_id, slow_get_page)) hits += 1;
    }

    std::cout << hits << "\n";

    return 0;
}

