#include "Chunk.h"
#include "SimulationDefinitions/SimulationConst.h"
#include "World.h"
#include <algorithm>
#include <chrono>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>    
#include <godot_cpp/variant/vector3i.hpp>
#include <SimulationDefinitions/SimulationUtils.h>
#include <utility>
#include <omp.h>
using namespace godot;

void World::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("FillArea", "start", "end", "typeId"), &World::FillArea);
    ClassDB::bind_method(D_METHOD("GetPhysicIteration"), &World::GetPhysicIteration);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "physicIteration"), "", "GetPhysicIteration");
}

void World::_notification(int p_what)
{
}

void World::_ready()
{ 
    if (godot::Engine::get_singleton()->is_editor_hint()) 
    {
        return; 
    }
    //=== Temp solution for world generation ==
    int temp = 0;
    for (int i = 0; i < _worldSize; ++i)
    {
        for (int j = 0; j < _worldSize; ++j)
        {
            Vector3i vecPos = Vector3i(CHUNK_SIZE * i, 0, CHUNK_SIZE * j);
            CreateChunk(vecPos, int3(i,0,j));
        }
    }
    chunks.sortByYZX();
    FillArea(Vector3i(0, 0, 0), Vector3i(_worldSize * CHUNK_SIZE, _worldSize * CHUNK_SIZE, _worldSize * CHUNK_SIZE), 3);
    FillArea(Vector3i(1, 1, 1), Vector3i((_worldSize * CHUNK_SIZE)-2, _worldSize * CHUNK_SIZE, (_worldSize * CHUNK_SIZE)-2), 0);

    FillArea(Vector3i((_worldSize * CHUNK_SIZE) /4, 9, (_worldSize * CHUNK_SIZE) / 4), Vector3i((_worldSize * CHUNK_SIZE) / 4+ (_worldSize * CHUNK_SIZE) / 2, 18, (_worldSize * CHUNK_SIZE) / 4+ (_worldSize * CHUNK_SIZE) / 2), 2);
    GlobalSeed = 1;
    // === End of temp solution ===
}
int NumberOfAllCells;
void World::_process(double delta)
{
    if (godot::Engine::get_singleton()->is_editor_hint()) 
    {
        return;
    }
    std::scoped_lock lock(_visualMutex);
    if (!_haveUpdateForVisual) return;

    for (auto& chunk : chunks)
    {
        chunk->second->UpdateVisuals();
    }

    _haveUpdateForVisual = false;
}

void World::_physics_process(double delta)
{
    if (godot::Engine::get_singleton()->is_editor_hint()) 
    {
        return;
    }
    NumberOfAllCells = 0;
    auto start = std::chrono::high_resolution_clock::now();

    const auto numberOfNeighbors = 27; //Including ourselves
    int color[numberOfNeighbors]; //Colors for a checkerboard-like processing
    for (int temp = 0; temp < numberOfNeighbors-1; ++temp)
    {
        color[temp] = temp;
    }
    std::mt19937 rng(rd());
    std::shuffle(color, color + (sizeof(color) / sizeof(color[0])), rng);
    for (int i = 0; i < numberOfNeighbors-1; ++i)
    {
        int colorTemp = color[i];
#pragma omp parallel for
        for (int j = 0; j < chunks.size(); ++j)
        {
            auto& chunk = chunks.sorted[j];
            auto pos = chunk->first;
            if (((pos.x % 3) + (pos.y % 3) * 9 + (pos.z % 3) * 3) == colorTemp) //1. Update our dead cells
            {
                for (int direction = 0; direction < numberOfNeighbors-1; ++direction)
                {
                    if (direction == 13) continue; //Exclude the direction towards ourselves
                    auto [dx, dy, dz] = ChunkIndexToDirection(direction);
                    auto it = chunks.get(int3(pos.x + dx, pos.y + dy, pos.z + dz));
                    if (it != nullptr)
                    {
                        chunk->second->UpdateDeadCellsFromChunk((*it), direction);
                    }
                }

                chunk->second->SimulationStep(); //2. Simulate our cells

                for (int direction = 0; direction < numberOfNeighbors-1; ++direction) //3. Update border cells in neighbors chunks
                {
                    if (direction == 13) continue; //Exclude the direction towards ourselves
                    auto [dx, dy, dz] = ChunkIndexToDirection(direction);
                    auto it = chunks.get(int3(pos.x + dx, pos.y + dy, pos.z + dz));
                    if (it != nullptr)
                    {
                        (*it)->UpdateBorderCellsFromChunk(chunk->second, InverseForChunkDirection(direction));
                    }
                }
            }
        }
    }
    forward = !forward;

#pragma omp parallel for
    for (int i = 0; i < chunks.size(); ++i)
    {
        auto& chunk = chunks.sorted[i];
        chunk->second->CommitStep();
        NumberOfAllCells += chunk->second->GetNumberActiveCells();
        chunk->second->ChunkSeed++;
    }
    std::scoped_lock lock(_visualMutex);
    _haveUpdateForVisual = true;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    physicIteration = duration.count();
    simulateStepCount++;

    UtilityFunctions::print("Chunk Simulation time: ", physicIteration);
    UtilityFunctions::print("Number of all cells: ", NumberOfAllCells);
    UtilityFunctions::print("Simulate step: ", simulateStepCount);
}
float World::GetPhysicIteration() 
{
    return physicIteration;
}
Chunk* World::GetChunk(const int3& index)
{
    return *chunks.get(index);
}

std::size_t HashChunkPos(int x, int y, int z) 
{
    std::size_t h = 2166136261u;
    h = (h * 16777619) ^ x;
    h = (h * 16777619) ^ y;
    h = (h * 16777619) ^ z;
    return h;
}
Chunk* World::CreateChunk(Vector3i globalPos, int3 chunkPos)
{
    Ref<PackedScene> chunkScene = ResourceLoader::get_singleton()->load("res://Chunk.tscn");
    if (!chunkScene.is_valid())
    {
        UtilityFunctions::printerr("Failed to load scene");
        return nullptr;
    }
    Node* chunkInstance = chunkScene->instantiate();
    Chunk* chunkPtr = Object::cast_to<Chunk>(chunkInstance);
    if (!chunkPtr)
    {
        UtilityFunctions::printerr("Failed to cast Node* to Chunk*");
        return nullptr;
    }
    chunks.insert(chunkPos,chunkPtr);
    chunkPtr->Initialize(chunkPos, HashChunkPos(chunkPos.x, chunkPos.y, chunkPos.z) ^ GlobalSeed);

    this->add_child(chunkPtr);
    chunkPtr->set_global_position(globalPos);
    return chunkPtr;
}



void World::FillArea(
    const Vector3i& world_start,
    const Vector3i& world_end,
    const int type)
{
    Vector3i min_pos = Vector3i
    (
        std::min(world_start.x, world_end.x),
        std::min(world_start.y, world_end.y),
        std::min(world_start.z, world_end.z)
    );

    Vector3i max_pos = Vector3i
    (
        std::max(world_start.x, world_end.x),
        std::max(world_start.y, world_end.y),
        std::max(world_start.z, world_end.z)
    );

    Vector3i chunk_min
    (
        FloorDiv(min_pos.x, CHUNK_SIZE),
        FloorDiv(min_pos.y, CHUNK_SIZE),
        FloorDiv(min_pos.z, CHUNK_SIZE)
    );

    Vector3i chunk_max
    (
        FloorDiv(max_pos.x, CHUNK_SIZE),
        FloorDiv(max_pos.y, CHUNK_SIZE),
        FloorDiv(max_pos.z, CHUNK_SIZE)
    );
    for (int cx = chunk_min.x; cx <= chunk_max.x; ++cx) 
    {
        for (int cy = chunk_min.y; cy <= chunk_max.y; ++cy) 
        {
            for (int cz = chunk_min.z; cz <= chunk_max.z; ++cz) 
            {
                Vector3i chunk_pos(cx, cy, cz);
                auto it = chunks.get(int3(chunk_pos));
                if (it == nullptr) continue;
                Chunk* chunk = (*it);

                Vector3i chunk_world_start = chunk_pos * CHUNK_SIZE;
                Vector3i chunk_world_end = chunk_world_start + Vector3i(CHUNK_SIZE - 1, CHUNK_SIZE - 1, CHUNK_SIZE - 1);
                Vector3i local_start = Vector3i
                (
                    std::max(min_pos.x, chunk_world_start.x) - chunk_world_start.x,
                    std::max(min_pos.y, chunk_world_start.y) - chunk_world_start.y,
                    std::max(min_pos.z, chunk_world_start.z) - chunk_world_start.z
                );

                Vector3i local_end = Vector3i
                (
                    std::min(max_pos.x, chunk_world_end.x) - chunk_world_start.x,
                    std::min(max_pos.y, chunk_world_end.y) - chunk_world_start.y,
                    std::min(max_pos.z, chunk_world_end.z) - chunk_world_start.z
                );
                chunk->FillArea(local_start, local_end, type);
            }
        }
    }
}
