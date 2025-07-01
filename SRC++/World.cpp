#include "World.h"
#include "Chunk.h"
#include "SimulationDefinitions/SimulationConst.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>    
#include <CellsDefinitions/CellsVisualPropertyes.h>
#include <godot_cpp/classes/plane_mesh.hpp>
#include <utility>
#include "CellsDefinitions/CellTypes.h"
#include "SimulationDefinitions/SimulationContext.h"
#include "SimulationDefinitions/CellSimulationRegistry.h"
#include <iostream>
#include <chrono>
#include <mmintrin.h> 
#include <xmmintrin.h>
#include <algorithm>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/vector3i.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <SimulationDefinitions/SimulationUtils.h>

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

    //for (auto& chunk : chunks)
    //{
    //    auto pos = chunk->first;
    //    for (int i = 0; i < 27; ++i)
    //    {
    //        if (i == 13) continue;
    //        auto [dx, dy, dz] = ChunkIndexToDirection(i);
    //        auto it = chunks.get(int3(pos.x + dx, pos.y + dy, pos.z + dz));
    //        if (it != nullptr)
    //        {
    //            chunk->second->UpdateDeadFromChunk((*it), i);
    //        }
    //    }
    //}
    FillArea(Vector3i(4, 11, 4), Vector3i(26 + (CHUNK_SIZE * 3), 21, 26 + (CHUNK_SIZE * 3)), 2);
    GlobalSeed = 1;
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
    simulateStepCount++;
    NumberOfAllCells = 0;
    auto start = std::chrono::high_resolution_clock::now(); 
    
    for (auto& chunk : chunks)
    {
        auto pos = chunk->first;
        for (int i = 0; i < 27; ++i)
        {
            if (i == 13) continue;
            auto [dx, dy, dz] = ChunkIndexToDirection(i);
            auto it = chunks.get(int3(pos.x + dx, pos.y + dy, pos.z + dz));
            if (it != nullptr)
            {
                chunk->second->UpdateDeadCellsFromChunk((*it), i);
            }
        }

        chunk->second->SimulationStep();
        
        for (int i = 0; i < 27; ++i)
        {
            if (i == 13) continue;
            auto [dx, dy, dz] = ChunkIndexToDirection(i);
            auto it = chunks.get(int3(pos.x + dx, pos.y + dy, pos.z + dz));
            if (it != nullptr)
            {
                (*it)->UpdateBorderCellsFromChunk(chunk->second, InverseForChunkDirection(i));
            }
        }
    }
    for (auto& chunk : chunks)
    {
        chunk->second->CommitStep();
        NumberOfAllCells += chunk->second->GetNumberActiveCells();
    }

    std::scoped_lock lock(_visualMutex);
    _haveUpdateForVisual = true;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    physicIteration = duration.count();

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
