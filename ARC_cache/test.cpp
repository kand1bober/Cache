#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <sstream>

#include "cache.hpp"
#include "ideal_cache.hpp"

int main()
{    
    //open file
    std::ifstream tests_file("test.txt");
    
    if (!tests_file.is_open())
    {
        std::cerr << "Cannot open open file with tests for reading" << std::endl;
        return -1;
    }
    
    std::string line;
    int test_count = 0;
    while (std::getline(tests_file, line)) 
    {
        std::vector<int> num_arr; 
        if (line.empty())
            continue;

        std::istringstream iss_line(line);
        int number;
        while (iss_line >> number)
        {
            // std::cout << number << " "; 
            num_arr.push_back(number);
        }
        num_arr.shrink_to_fit();

        ideal_cache_t<page_t> ideal_cache{(size_t)num_arr[0]};
        ideal_cache.fill(num_arr);        

        ARC_cache_t<page_t> arc_cache{(size_t)num_arr[0]};

        int i = 0, arc_hits = 0, ideal_hits = 0;
        for (; i < num_arr[1]; i++)
        {
            if (arc_cache.lookup_update(num_arr[i + 2], slow_get_page)) arc_hits += 1;
            if (ideal_cache.lookup_update(num_arr[i + 2])) ideal_hits += 1;
        }   

        std::cout << "Test " << test_count << ": ";
        if (arc_hits == num_arr[i + 2])
        {
            std::cout << "passed, hits: " << arc_hits << "\n";
            std::cout << "ideal cache hits: " << ideal_hits << "\n\n";
        }
        else 
            std::cout << "failed, ans: " << num_arr[i + 2] << ", out: " << arc_hits <<"\n";

        test_count += 1;

    }
    tests_file.close();
    
    return 0;
}

