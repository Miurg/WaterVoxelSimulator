#pragma once

#include "Chunk.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/vector3i.hpp>
#include "Int3.h"
#include "SimulationDefinitions/ChunkMap.h"
#include <random>

using namespace godot;

class World : public Node
{
    GDCLASS(World, Node);
private:
    ChunkMap chunks;
    int _worldSize = 10;
    std::random_device rd;

    uint8_t _randomOffset = 0; //For simulation

    int simulateStepCount = 0;

    std::mutex _visualMutex;
    bool _haveUpdateForVisual;
    bool forward = true;
public:    
    int GlobalSeed;
    double physicIteration = 0;
    static void _bind_methods();

    virtual void _notification(int p_what);
    virtual void _ready() override;
    virtual void _process(double delta) override;
    virtual void _physics_process(double delta) override;
    float GetPhysicIteration();
    Chunk* GetChunk(const int3& index);
    Chunk* CreateChunk(Vector3i globalPos, int3 chunkPos);
    void FillArea(
        const Vector3i& start,
        const Vector3i& end,
        const int type);
};
