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


    bool B2_lookup_upate(KeyT key)
    {
        auto hit = hash_.find(key);
        if (hit == hash_.end()) //not found 
            return false;
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
        if (T2_cache_.T2_lookup_update(T1_cache_))
        {
            return true;
        } 
        else if (T1_cache_.lookup_upadte(B2_cache_))
        {
            // replace from T1 to T2 with splice
            return true;
        }  
        else if (B2_cache_.lookup_update(B1_cache_))  
        {
            // shift T2 to left (increase T2 and shrink T1) 

            return true;
        }
        else if (B1_cache_.lookup_update(page_id, slow_get_page))
        {
            
            return true;
        }
        else
        {

        }
    }
 
    // evict from T-type cache to corresponding B -type cache 
    template<typename T_type, typename B_type>
    void evict_page(T_type T, B_type B)
    {
    
    }

    //increase T1, shrink T2
    template<typename T_type>
    void shift_right(T_type T1, T_type T2)
    {

    }

    //increase T2, shrink T1
    template<typename T_type>
    void shift_left(T_type T1, T_type T2)
    {

    }
};
