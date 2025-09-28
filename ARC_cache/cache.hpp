#include <list>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <iterator> 

#ifndef ARC_CACHE_HEADER
#define ARC_CACHE_HEADER

// #define DEBUG

#ifdef DEBUG
    #define DEBUG_PRINT(expr) expr
#else 
    #define DEBUG_PRINT(expr) 
#endif

struct page_t
{
    int id;
    size_t sz;
};


struct empty_page 
{
    int id;
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
template<typename T, typename KeyT = int>
struct cache_t
{
    size_t sz_;
    std::list<T> cache_; //list

    cache_t(size_t sz) : sz_(sz) {}; //ctor

    using ListIt = typename std::list<T>::iterator; //hash_table
    std::unordered_map<KeyT, ListIt> hash_;

    bool full() const
    {
        return (cache_.size() >= sz_);
    }

    bool shrink()
    {
        if (sz_ > 0)
        {
            sz_ -= 1;
            return true;
        }
        else 
            return false;
    }

    bool lookup(KeyT key) const
    {
        auto hit = hash_.find(key);
        if (hit == hash_.end()) //not found 
            return false;
        else 
            return true;
    }

    void push_front(T page) 
    {
        cache_.push_front(page);
        hash_[page.id] = cache_.begin();
    }

    void pop_back(KeyT key)
    {
        hash_.erase(key);
        cache_.pop_back();
    }

    bool LRU_pop(KeyT key)
    {   
        auto found = hash_.find(key)->second;
        if (found != cache_.begin())
            cache_.splice(cache_.begin(), cache_, found, std::next(found));

        return true;        
    }
};


/*
* requires the type T of page to have field named id, 
* to differ one page from another
*/
template<typename T, typename KeyT = int>
struct ARC_cache_t
{
private:
    size_t sz_; //sz = sz_T1 + sz_T2 = sz_B1 + sz_B2 --> sz_ is const
    cache_t<T, KeyT> T1_;     
    cache_t<T, KeyT> T2_;     
    cache_t<empty_page, KeyT> B1_;     
    cache_t<empty_page, KeyT> B2_;     

    template<typename T_cache, typename B_cache>
    void shift_L_cache(T_cache& TC, B_cache& BC)
    {
        if (TC.full() && TC.cache_.size() > 0)
        {
            if (BC.full())
            {
                //last from T1 --> first in B1
                BC.hash_.erase(BC.cache_.back().id);
                BC.cache_.pop_back();
            }
            auto evicted_id = TC.cache_.back().id;

            BC.cache_.push_front((empty_page){.id = evicted_id});
            BC.hash_[evicted_id] = BC.cache_.begin();

            //erase last from T1
            TC.hash_.erase(evicted_id);
            TC.cache_.pop_back();
        }
    }

    //evicts from A, settles to head of B
    template<typename T_cache>
    void evict_and_settle(T_cache& A, T_cache& B, KeyT key)
    {
        auto it = A.hash_.find(key);
        assert(it != A.hash_.end());
        auto found = it->second;

        A.hash_.erase(key);
        B.cache_.splice(B.cache_.begin(), A.cache_, found, std::next(found));
        B.hash_[key] = found;

        A.sz_ -= 1;
        B.sz_ += 1;
    }

public:
    ARC_cache_t(size_t sz) : //ctor
        sz_(sz),
        T1_((sz%2 == 0) ? (sz/2) : (sz+1)/2), 
        T2_((sz%2 == 0) ? (sz/2) : (sz-1)/2), 
        B1_((sz%2 == 0) ? (sz/2) : (sz+1)/2), 
        B2_((sz%2 == 0) ? (sz/2) : (sz-1)/2)
    {
        if (sz_ < 1) {
            std::cerr << "Too little size of cache" << std::endl;
            exit(0);
        }
    };  

    template<typename F>
    bool lookup_update(KeyT key, F slow_get_page)
    {
        if (T2_.lookup(key)) //взять из T2 и переместить по принципу LRU
        {
            T2_.LRU_pop(key);
            
            DEBUG_PRINT(std::cout << "T2\n";)
            DEBUG_PRINT(std::cout << "T1_sz: " << T1_.sz_ << "; T2_sz: " << T2_.sz_ << "\n";)

            return true;
        } 
        else if (T1_.lookup(key)) //shift L2 + evict from L1 -> add_front L2
        {
            shift_L_cache(T2_, B2_);
            evict_and_settle(T1_, T2_, key);

            DEBUG_PRINT(std::cout << "T1\n";)
            DEBUG_PRINT(std::cout << "T1_sz: " << T1_.sz_ << "; T2_sz: " << T2_.sz_ << "\n";)

            return true;
        }  
        else if (B2_.lookup(key)) // shift L1 + add_front L2 
        {
            if (T1_.shrink()) // there is still a place to grow T2
            {
                shift_L_cache(T1_, B1_);
                T2_.sz_ += 1;
                T2_.cache_.push_front(slow_get_page(key));
            }
            else // there is no more place to grow T1 
            {
                shift_L_cache(T2_, B2_);
                T2_.push_front(slow_get_page(key));
            }

            DEBUG_PRINT(std::cout << "B2\n";)
            DEBUG_PRINT(std::cout << "T1_sz: " << T1_.sz_ << "; T2_sz: " << T2_.sz_ << "\n";)

            return false;
        }
        else if (B1_.lookup(key)) // shift L2 + add_front L1
        { 
            if (T2_.shrink()) // there is still a place to grow T1
            {
                shift_L_cache(T2_, B2_);
                T1_.sz_ += 1;
                T1_.push_front(slow_get_page(key));
            } 
            else // there is no more place to grow T1 
            {
                shift_L_cache(T1_, B1_);
                T1_.push_front(slow_get_page(key));
            }

            DEBUG_PRINT(std::cout << "B1\n";)
            DEBUG_PRINT(std::cout << "T1_sz: " << T1_.sz_ << "; T2_sz: " << T2_.sz_ << "\n";)

            return false;
        }
        else // shift L1 + add_front L1
        {
            shift_L_cache(T1_, B1_);
            T1_.push_front(slow_get_page(key));
            
            DEBUG_PRINT(std::cout << "slow\n";)
            DEBUG_PRINT(std::cout << "T1_sz: " << T1_.sz_ << "; T2_sz: " << T2_.sz_ << "\n";)

            return false;
        }
    }
};

#endif
