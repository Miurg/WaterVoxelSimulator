#include <unordered_map>
#include <Int3.h>
#include <algorithm>

struct ChunkMap 
{
public:
    std::unordered_map<int3, Chunk*> map;
    std::vector<typename std::unordered_map<int3, Chunk*>::iterator> sorted;

    bool insert(const int3& key, Chunk* value)
    {
        auto [it, inserted] = map.emplace(key, value);
        if (!inserted) return false;
        sorted.push_back(it);
        return true;
    }

    bool erase(const int3& key) 
    {
        auto it = map.find(key);
        if (it == map.end()) return false;

        sorted.erase(std::remove_if(sorted.begin(), sorted.end(),
            [&](auto& ptr) { return ptr->first == key; }), sorted.end());

        map.erase(it);
        return true;
    }

    Chunk** get(const int3& key)
    {
        auto it = map.find(key);
        return (it != map.end()) ? &it->second : nullptr;
    }

    void sortByYZX() 
    {
        std::sort(sorted.begin(), sorted.end(), [](auto a, auto b) 
            {
            const int3& A = a->first;
            const int3& B = b->first;
            if (A.y != B.y) return A.y < B.y;
            if (A.z != B.z) return A.z < B.z;
            return A.x < B.x;
            });
    }

    auto begin() const { return sorted.begin(); }
    auto end() const { return sorted.end(); }

    size_t size() const { return map.size(); }

    void clear() {
        map.clear();
        sorted.clear();
    }
};