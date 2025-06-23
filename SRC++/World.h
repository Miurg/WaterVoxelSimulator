#pragma once

#include "Chunk.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>
#include <godot_cpp/variant/vector3i.hpp>
#include "CellsDefinitions/CellBuffer.h"
#include <godot_cpp/classes/rendering_server.hpp>
#include "SimulationDefinitions/SimulationConst.h"
#include "Int3.h"
#include "SimulationDefinitions/ChunkMap.h"

using namespace godot;

class World : public Node
{
    GDCLASS(World, Node);
private:
    ChunkMap chunks;
    int _worldSize = 6;


    uint8_t _randomOffset = 0; //For simulation

    int simulateStepCount = 0;

    std::mutex _visualMutex;
    bool _haveUpdateForVisual;

    RenderingServer* rs;
public:    
    int GlobalSeed;
    static void _bind_methods();

    virtual void _notification(int p_what);
    virtual void _ready() override;
    virtual void _process(double delta) override;
    virtual void _physics_process(double delta) override;
    Chunk* GetChunk(const int3& index);
    Chunk* CreateChunk(Vector3i globalPos, int3 chunkPos);
    void FillArea(
        const Vector3i& start,
        const Vector3i& end,
        const int type);
};
