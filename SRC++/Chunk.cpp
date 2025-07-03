#include "Chunk.h"
#include "SimulationDefinitions/SimulationConst.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>    
#include <CellsDefinitions/CellsVisualPropertyes.h>
#include <utility>
#include "CellsDefinitions/ECellTypes.h"
#include "SimulationDefinitions/SimulationContext.h"
#include "SimulationDefinitions/CellSimulationRegistry.h"
#include <algorithm>
#include <random>
#include <SimulationDefinitions/SimulationUtils.h>

using namespace godot;

void Chunk::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("FillArea", "start", "end", "CellTypeId"), &Chunk::FillArea);
    ClassDB::bind_method(D_METHOD("SimulationStep"), &Chunk::SimulationStep);
    ClassDB::bind_method(D_METHOD("UpdateVisuals"), &Chunk::UpdateVisuals);
    ClassDB::bind_method(D_METHOD("SetCellMesh", "mesh"), &Chunk::SetCellMesh);
    ClassDB::bind_method(D_METHOD("GetCellMesh"), &Chunk::GetCellMesh);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "cellMesh", PROPERTY_HINT_RESOURCE_TYPE, "Mesh"),
        "SetCellMesh", "GetCellMesh");

}

void Chunk::SetCellMesh(const Ref<Mesh>& mesh) 
{
    _cellMesh = mesh;
}

Ref<Mesh> Chunk::GetCellMesh() const {
    return _cellMesh;
}

void Chunk::_notification(int p_what) {
    if (p_what == NOTIFICATION_READY)
    {
        UpdateVisuals();
    }
}

void Chunk::Initialize(int3 position, int seed)
{
    PositionAmongChunks = position;
    ChunkSeed = seed;
}

void Chunk::_ready() 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.2, 0.2);
    double randomValue = dis(gen);
    randomColorOffset = godot::Color(randomValue, randomValue, randomValue);
    InitializeMultiMesh();
    CellSimulationRegistry::Init();
}

void Chunk::InitializeMultiMesh() 
{
    _multiMesh.instantiate();
    _multiMesh->set_mesh(_cellMesh);
    _multiMesh->set_transform_format(MultiMesh::TRANSFORM_3D);
    _multiMesh->set_use_custom_data(true);

    _multiMeshInstance = memnew(MultiMeshInstance3D);
    _multiMeshInstance->set_multimesh(_multiMesh);
    add_child(_multiMeshInstance);

    _multiMesh->set_instance_count(CHUNK_SIZE3);
    static const Basis basis;
    static const godot::Transform3D baseTransform(basis, Vector3());
    for (int i = 0; i < CHUNK_SIZE3; i++)
    {
        Vector3 position = SIZECellFastIndexToVector(i);
        Transform3D transform = baseTransform.translated(position);
        _multiMesh->set_instance_transform(i, transform);
    }
}

void Chunk::UpdateDeadCellsFromChunk(Chunk* chunk, int direction)
{
    const auto& myDeadAndItsBorder = boundaryPairs[direction];

    for (size_t j = 0; j < myDeadAndItsBorder.boundaryCells.size(); ++j)
    {
        uint_fast16_t deadIndex = myDeadAndItsBorder.deadCells[j];
        Cell theirCell = chunk->GetCellFromNextBuffer(myDeadAndItsBorder.boundaryCells[j]);
        if (_ptrNextCellBuffer->Cells[deadIndex].Type == theirCell.Type) continue;
        _ptrNextCellBuffer->Cells[deadIndex] = theirCell;
        if (!theirCell.IsAir())
            _ptrCurrentCellBuffer->Cells[deadIndex].SetReserved(true);
    }
}

void Chunk::UpdateBorderCellsFromChunk(Chunk* chunk, int direction)
{
    const auto& theirDeadAndMyBorder = boundaryPairs[InverseForChunkDirection(direction)];

    for (size_t j = 0; j < theirDeadAndMyBorder.deadCells.size(); ++j)
    {
        uint_fast16_t boundryIndex = theirDeadAndMyBorder.boundaryCells[j];
        Cell theirCell = chunk->GetCellFromNextBuffer(theirDeadAndMyBorder.deadCells[j]);
        Cell ourCell = _ptrNextCellBuffer->Cells[boundryIndex];

        if (ourCell.Type != theirCell.Type)
        {
            if (CellSimulationRegistry::IsActive(ourCell.Type))
            {
                auto& vec = _indicesByTypeCurrent[ourCell.Type].list;
                auto& set = _indicesByTypeCurrent[ourCell.Type].set;
                vec.erase(std::remove(vec.begin(), vec.end(), boundryIndex), vec.end());
                set.erase(boundryIndex);

                auto& vec2 = _indicesByTypeNext[ourCell.Type].list;
                auto& set2 = _indicesByTypeNext[ourCell.Type].set;
                vec2.erase(std::remove(vec2.begin(), vec2.end(), boundryIndex), vec2.end());
                set2.erase(boundryIndex);
            }

            if (CellSimulationRegistry::IsActive(theirCell.Type))
            {
                TypeIndexData& data2 = _indicesByTypeNext[theirCell.Type];
                if (data2.set.insert(boundryIndex).second)
                {
                    data2.list.push_back(boundryIndex);
                }
            }
            _ptrCurrentCellBuffer->Cells[boundryIndex].SetReserved(true);
            _ptrNextCellBuffer->Cells[boundryIndex] = theirCell;
        }
        else if(theirCell.IsActive() && !ourCell.IsActive())
        {
            _ptrNextCellBuffer->Cells[boundryIndex].SetActive(true);
        }
    }
}

void Chunk::SimulationStep()
{
    SimulationContext ctx = SimulationContext();
    ctx._currentCellBuffer = _ptrCurrentCellBuffer;
    ctx._nextCellBuffer = _ptrNextCellBuffer;
               
    std::mt19937 gen(ChunkSeed);
    std::uniform_int_distribution<> dist(0, 4); 
    int random_index = dist(gen);
    ctx.randomOffset = random_index;

    for (auto &[type, indices] : _indicesByTypeCurrent)
    {
        ctx._indicesCurrent = &indices;
        auto itNext = _indicesByTypeNext.find(type);
        if (itNext != _indicesByTypeNext.end()) 
        {
            ctx._indicesNext = &(itNext->second);
        }
        CellSimulationRegistry::Simulate(type, ctx);
        std::reverse(itNext->second.list.begin(), itNext->second.list.end());
    }
}

void Chunk::CommitStep()
{
    _ptrNextCellBuffer->CopyTo((*_ptrCurrentCellBuffer)); //Move every cells physically cuz we dont simulate non active cells. That just cheaper

    _indicesByTypeCurrent = _indicesByTypeNext;
}

static const Basis basis;
static const godot::Transform3D baseTransform(basis, Vector3());
void Chunk::UpdateVisuals()
{
    for (uint_fast16_t z = 1; z <= CHUNK_EXT - 2; ++z) //From 1 to CHUNK_EXT-1 for only access cells in the current chunk and not the dead layer
    {
        uint_fast16_t zOffset = z * CHUNK_EXT2;
        for (uint_fast16_t y = 1; y <= CHUNK_EXT - 2; ++y)
        {
            uint_fast16_t yOffset = y * CHUNK_EXT;
            for (uint_fast16_t x = 1; x <= CHUNK_EXT - 2; ++x)
            {
                uint_fast16_t srcIndex = x + yOffset + zOffset;
                uint_fast16_t dstX = x - 1;
                uint_fast16_t dstY = y - 1;
                uint_fast16_t dstZ = z - 1;

                uint_fast32_t dstIndex = dstZ * CHUNK_SIZE * CHUNK_SIZE +
                    dstY * CHUNK_SIZE +
                    dstX;

                //if (!IsBorderLayerCell(srcIndex)) continue;
                if (_ptrNextCellBuffer->Cells[srcIndex].Type == _visualCurrentState[dstIndex]) continue;

                _visualCurrentState[dstIndex] = _ptrNextCellBuffer->Cells[srcIndex].Type;//FROM CHUNK_EXT TO CHUNK_SIZE

                Color color = CellsVisualPropertyes::GetColorForCellType(_visualCurrentState[dstIndex]);
                _multiMesh->set_instance_custom_data(dstIndex, color);
            }
        }
    }
}

uint_fast16_t Chunk::GetNumberActiveCells()
{
    NumberOfActiveCells = 0;
    for (auto& [type, indices] : _indicesByTypeCurrent)
    {
        for (int i = 0; i < indices.list.size(); i++)
        {
            if (_ptrCurrentCellBuffer->Cells[indices.list[i]].IsActive())
            {
                NumberOfActiveCells++;
            }
        }
        
    }
    return NumberOfActiveCells;
}

Cell Chunk::GetCellFromCurrentBuffer(uint_fast16_t Index)
{
    return _ptrCurrentCellBuffer->Cells[Index];
}

Cell Chunk::GetCellFromNextBuffer(uint_fast16_t Index)
{
    return _ptrNextCellBuffer->Cells[Index];
}

void Chunk::SetCell(const Vector3i pos, const ECellTypes type)
{
    //We do not take into account the layer of dead cells to install new cells, so +1 for start
    uint_fast16_t index = EXTCellVectorToIndex(Vector3i(pos.x+1,pos.y+1,pos.z+1)); 

    Cell currentCell = _ptrCurrentCellBuffer->Cells[index];

    _ptrCurrentCellBuffer->Cells[index].SetActive(true);
    _ptrCurrentCellBuffer->Cells[index].SetType(type);
    _ptrNextCellBuffer->Cells[index].SetActive(true);
    _ptrNextCellBuffer->Cells[index].SetType(type); //Store everything in next buffer too, cus we dont want to lost unactive cells

    if (currentCell.IsAir() || !CellSimulationRegistry::IsActive(currentCell.Type))
    {
        if (CellSimulationRegistry::IsActive(type))
        {
            TypeIndexData& data = _indicesByTypeCurrent[type];
            if (data.set.insert(index).second)
            {
                data.list.push_back(index);
            }
            TypeIndexData& data2 = _indicesByTypeNext[type];
            if (data2.set.insert(index).second)
            {
                data2.list.push_back(index);
            }
        }
    }
    else
    {
        auto it = _indicesByTypeCurrent.find(currentCell.Type);
        if (it != _indicesByTypeCurrent.end())
        {
            auto& data = it->second;
            if (data.set.erase(index))
                data.list.erase(std::remove(data.list.begin(), data.list.end(), index), data.list.end());
        }

        it = _indicesByTypeNext.find(currentCell.Type);
        if (it != _indicesByTypeNext.end())
        {
            auto& data = it->second;
            if (data.set.erase(index))
                data.list.erase(std::remove(data.list.begin(), data.list.end(), index), data.list.end());
        }

        if (CellSimulationRegistry::IsActive(type))
        {
            auto& currentData = _indicesByTypeCurrent[type];
            if (currentData.set.insert(index).second)
                currentData.list.push_back(index);

            auto& nextData = _indicesByTypeNext[type];
            if (nextData.set.insert(index).second)
                nextData.list.push_back(index);
        }
    }
}
void Chunk::FillArea(
    const Vector3i& start,
    const Vector3i& end,
    const int CellTypeId)
{
    ECellTypes type = static_cast<ECellTypes>(CellTypeId);
    if ((start.x < 0) || (start.x > CHUNK_SIZE - 1) ||
        (start.y < 0) || (start.y > CHUNK_SIZE - 1) ||
        (start.z < 0) || (start.z > CHUNK_SIZE - 1))
    {
        UtilityFunctions::printerr("Invalid start position");
        return;
    }
    else if ((end.x < 0) || (end.x > CHUNK_SIZE - 1) ||
        (end.y < 0) || (end.y > CHUNK_SIZE - 1) ||
        (end.z < 0) || (end.z > CHUNK_SIZE - 1))
    {
        UtilityFunctions::printerr("Invalid end position");
        return;
    }
    for (uint_fast16_t x = start.x; x <= end.x; ++x) 
        for (uint_fast16_t y = start.y; y <= end.y; ++y) 
            for (uint_fast16_t z = start.z; z <= end.z; ++z) 
            {
                Vector3i pos(x, y, z);
                SetCell(pos, type);
            }
}
