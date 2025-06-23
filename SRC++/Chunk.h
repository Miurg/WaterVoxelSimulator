#pragma once

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
#include "CellsDefinitions/TypeIndexData.h"

using namespace godot;

class Chunk : public Node3D 
{
    GDCLASS(Chunk, Node3D);

private:
    CellBuffer _currentCellBuffer;
    CellBuffer _nextCellBuffer;

    CellBuffer* _ptrCurrentCellBuffer = &_currentCellBuffer;
    CellBuffer* _ptrNextCellBuffer = &_nextCellBuffer;

    std::unordered_map<CellTypes, TypeIndexData> _indicesByTypeCurrent;
    std::unordered_map<CellTypes, TypeIndexData> _indicesByTypeNext;


    CellTypes _visualInstances[CHUNK_SIZE3];
    CellTypes _visualBuffer[CHUNK_SIZE3];
    CellTypes _visualCurrentState[CHUNK_SIZE3];

    CellTypes* _ptrVisualInstances = _visualInstances;
    CellTypes* _ptrVisualBuffer = _visualBuffer;
    CellTypes* _ptrVisualCurrentState = _visualCurrentState;

    uint_fast16_t NumberOfActiveCells;

    uint8_t _randomOffset = 0; //For simulation

    godot::Color randomColorOffset;

    Ref<MultiMesh> _multiMesh;
    MultiMeshInstance3D* _multiMeshInstance;
    Ref<Mesh> _cellMesh;

    void SetCell(const Vector3i pos, const CellTypes type);
public:
    void Initialize(int3 position, int seed);
    int ChunkSeed;
    int3 PositionAmongChunks;
    static void _bind_methods();
    void SetCellMesh(const Ref<Mesh>& mesh);
    Ref<Mesh> GetCellMesh() const;
    virtual void _notification(int p_what);
    virtual void _ready() override;
    void InitializeMultiMesh();

    void UpdateDeadFromChunk(Chunk* chunk, int direction);
    void UpdateBorderFromChunk(Chunk* chunk, int direction);
    void StepSimulation();
    void CommitStep();
    void UpdateVisuals();

    uint_fast16_t GetNumberActiveCells();
    Cell GetCellFromCurrentBuffer(uint_fast16_t Index);
    Cell GetCellFromNextBuffer(uint_fast16_t Index);
    void FillArea(
        const Vector3i& start,
        const Vector3i& end,
        const int type);
};
