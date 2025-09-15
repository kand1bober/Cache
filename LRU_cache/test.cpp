#include <iostream>
#include <fstream>
#include "cache.hpp"
#include <string>
#include <vector> 
#include <sstream>

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
        std::vector<int> numbers; 
        if (line.empty())
            continue;

        std::istringstream iss_line(line);
        int number;
        while (iss_line >> number)
        {
            // std::cout << "" << number << " "; 
            numbers.push_back(number);
        }

        LRU_cache_t<page_t> cache{numbers[0]};
        
        int i, hits = 0;
        for (i = 0; i < numbers[1]; i++)
        {
            if (cache.lookup_update(numbers[i + 2], slow_get_page)) hits += 1;

        }
        std::cout << "Test " << test_count << ": ";
        if (hits == numbers[i + 2])
            std::cout << "Test passed\n";
        else 
            std::cout << "Test failed <-- !\n";

        test_count += 1;
    }

    tests_file.close();
    return 0;
}

