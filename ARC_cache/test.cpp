#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <sstream>

#include "cache.hpp"

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
            // std::cout << "" << number << " "; 
            num_arr.push_back(number);
        }

        ARC_cache_t<page_t> cache{(size_t)num_arr[0]};
        
        int i = 0, hits = 0;
        for (; i < num_arr[1]; i++)
        {
            if (cache.lookup_update(num_arr[i + 2], slow_get_page)) hits += 1;

        }
        std::cout << "Test " << test_count << ": ";
        if (hits == num_arr[i + 2])
            std::cout << "passed\n\n";
        else 
            std::cout << "failed, ans: " << num_arr[i + 2] << ", out: " << hits <<"\n\n";

        test_count += 1;
    }

    tests_file.close();
    return 0;
}

