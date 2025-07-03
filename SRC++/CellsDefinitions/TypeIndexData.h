#pragma once

#include <vector>
#include <unordered_set>

struct TypeIndexData 
{
    std::vector<uint_fast16_t> list;           
    std::unordered_set<uint_fast16_t> set;     
};