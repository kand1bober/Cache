#include <list>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <iterator> 


struct page_t
{
    int id;
    size_t sz;
};


struct emtpy_page {};

/*
* requires the type T to have field named id, 
* to differ one page from another
*/
template<typename T, typename KeyT = int>
struct custom_cache_t
{
    size_t sz_;
    std::list<T> cache_;

    custom_cache_t(size_t sz) : sz_(sz) {}; //ctor

    using ListIt = typename std::list<T>::iterator;
    std::unordered_map<KeyT, ListIt> hash_;

    bool full() const
    {
        return (cache_.size() >= sz_);
    }
    
    template <typename F>
    bool T1_lookup_update(KeyT key, F slow_get_page)
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

    template <typename F>
    bool T2_lookup_upate(KeyT key, F slow_get_page)
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

            return true; // --> return page_t
        }
    }

    template <typename F>
    bool B1_lookup_upate(KeyT key, F slow_get_page)
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

    template <typename F>
    bool B2_lookup_upate(KeyT key, F slow_get_page)
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


/*
* requires the type T to have field named id, 
* to differ one page from another
*/
template<typename T, typename KeyT = int>
struct ARC_cache_t
{
private:
    size_t sz_;
    LRU_cache_t<T, KeyT> T1_cache_;     
    LRU_cache_t<T, KeyT> T2_cache_;     
    LRU_cache_t<T, empty_page> B1_cache_;     
    LRU_cache_t<T, empty_page> B2_cache_;     

public:
    ARC_cache_t(size_t sz) :
        sz_(sz), 
        T1_cache_(0), 
        T2_cache_(0), 
        B1_cache_(0), 
        B2_cache_(0) 
    {
        if (sz_ < 4) {
            std::cerr << Too little size of cache for ARC << std::endl;
            exit(1);
        }
    };  

    template<typename F>
    bool lookup_update(KeyT key, F slow_get_page)
    {

    }

    bool lookup_update()
    {
        if (is in T2)
            T2_cache_.T2_lookup_update(T1_cache_);    
        else if (is in T1)
            T1_cache_.lookup_upadte(B2_cache_);
        else if (is in B2)  
            B2_cache_.lookup_update(B1_cache_);
        else (is in B1)
            B1_cache_.lookup_update(page_id, slow_get_page);
    }
};
