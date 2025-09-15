#include <list>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <iterator> 


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


/*
* requires the type T to have field named id, 
* to differ one page from another
*/
template <typename T, typename KeyT = int>
struct LRU_cache_t 
{
    size_t sz_;
    std::list<T> cache_;

    LRU_cache_t(size_t sz) : sz_(sz) {}; //ctor

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
        else 
        {
            auto found = hit->second;
            if (found != cache_.begin())
            {
                cache_.splice(cache_.begin(), cache_, found, std::next(found));
            }

            return true;
        }
    }
};
