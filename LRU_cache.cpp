#include <list>
#include <unordered_map>
#include <iostream>
#include <cassert>

struct page_t 
{
    int id;
    size_t size;
};

page_t slow_get_page(int id)
{
    page_t page = {id, 0};
    return page;
}

template <typename T, typename KeyT = int>
struct cache_t 
{
    size_t sz_;
    std::list<T> cache_;

    using ListIt = typename std::list<T>::iterator;
    std::unordered_map<KeyT, ListIt> hash_;

    bool full() const
    {
        return (cache_.size() >= sz_);
    }
    

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page)
    {
        auto hit = hash_.find(key);
        if (hit == hash_.end()) //not found 
        {
            if (full())
            {
                hash_.erase(cache_.back().id);
                cache_.pop_back();
            }

            cache_.push_front(slow_get_page(key));
            hash_[key] = cache_.begin();
            return false;
        }
    }
};


int main()
{
    cache_t<page_t> cache;

    auto cache_sz = 0, input_sz = 0;
    std::cin >> cache_sz >> input_sz;
    cache.sz_ = cache_sz;

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
