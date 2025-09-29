#include <list>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <iterator> 
#include <algorithm>
#include <vector>

#include "page.hpp"

struct page_info 
{
    page_t page;
    int count;
};

template<typename T, typename KeyT = int>
struct ideal_cache_t
{
    size_t sz_;
    std::list<T> cache_; //list

    ideal_cache_t(size_t sz) : sz_(sz) {}; //ctor

    using ListIt = typename std::list<T>::iterator; //hash_table
    std::unordered_map<KeyT, ListIt> hash_;

    std::vector<page_info> page_info_arr;
    void fill(std::vector<int>& keys) // vector of pages on input
    {
        page_info_arr.reserve(keys.size());

        for (size_t i = 0; i < keys.size(); i++) 
        {
            auto it = std::find_if(
                page_info_arr.begin(), 
                page_info_arr.end(), 
                [&](const page_info& pi)
                {
                    return (pi.page.id == keys[i]);
                }
            );

            if (it != page_info_arr.end())
            {
                it->count += 1;
            }
            else
            {
                page_info_arr.push_back((page_info){ (page_t){keys[i], 0}, 1});
            }            
        }

        std::sort(page_info_arr.begin(),
            page_info_arr.end(),
            [&](const page_info& p1, const page_info& p2)
            {
                return (p1.count > p2.count); 
            }  
        );
        
        page_info_arr.resize(sz_);
    }

    bool lookup_update(KeyT key)
    {
        auto hit = hash_.find(key);

        if (hit == hash_.end())
        {
            if (cache_.size() < sz_)
            {
                auto it = std::find_if(
                    page_info_arr.begin(),
                    page_info_arr.end(),
                    [&](const page_info& pi)
                    {
                        return (pi.page.id == key);
                    }
                );

                cache_.push_back((page_t){it->page});
                hash_[it->page.id] = cache_.end();
            }

            return false;
        }
        else 
            return true;
    }
};
