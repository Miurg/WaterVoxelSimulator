using Godot;
using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Intrinsics.X86;
using System.Threading.Tasks;
using VoxelParticleSimulator.Scripts;
using VoxelParticleSimulator.Scripts.Cells;
using VoxelParticleSimulator.Scripts.Cells.Behavior;
using VoxelParticleSimulator.Scripts.Cells.Simulations.GeneralMoving;
using VoxelParticleSimulator.Scripts.Chunks;

public partial class Chunk : Node3D
{
    private CellBuffer _cellsCurrent = new CellBuffer(SimulatorConst.ChunkSize3);
    private CellBuffer _cellsNext = new CellBuffer(SimulatorConst.ChunkSize3);
    private List<CellVisual> _visualInstances = new();
    private List<CellVisual> _visualBuffer = new();
    private Dictionary<CellType, List<int>> _indicesByTypeCurrent = new();
    private Dictionary<CellType, List<int>> _indicesByTypeNext = new();
    private object _visualLock = new();
    private int _cells = 0;

    private MultiMeshInstance3D _multimeshInstance;
    private MultiMesh _multimesh;
    [Export]
    private Mesh _cellMesh;


    private int[] _neighborOffsetsByIndex;
    public Chunk()
    {
        int dx = 1;
        int dy = SimulatorConst.ChunkSize;
        int dz = SimulatorConst.ChunkSize * SimulatorConst.ChunkSize;

        _neighborOffsetsByIndex = new int[]
        {
        dx, -dx,
        dy, -dy,
        dz, -dz
        };

    }
    public override void _Ready()
    {
        InitializeMultimesh();
        for (int i = 0; i < SimulatorConst.ChunkSize3; i++)
        {
            _cellsNext.Type[i] = DefaultCells.Air.Type;
            _cellsNext.Flags[i] = DefaultCells.Air.Flags;
        }
        foreach (CellType type in Enum.GetValues<CellType>())
        {
            _indicesByTypeCurrent[type] = new List<int>(1024);
            _indicesByTypeNext[type] = new List<int>(1024);
        }

        GD.Print("Chunk ready.");
    }
    private void InitializeMultimesh()
    {
        _multimesh = new MultiMesh
        {
            Mesh = _cellMesh,
            TransformFormat = MultiMesh.TransformFormatEnum.Transform3D,
            InstanceCount = 0,
            UseCustomData = true,
        };
        _multimeshInstance = new MultiMeshInstance3D
        {
            Multimesh = _multimesh
        };
        AddChild(_multimeshInstance);
        _multimesh.InstanceCount = SimulatorConst.ChunkSize3;
    }
    public unsafe void Simulate()
    {
        var stopwatchSimulation = System.Diagnostics.Stopwatch.StartNew();

  
        foreach (var kvp in _indicesByTypeCurrent)
        {
            var type = kvp.Key; 
            var currentIndicesListValue = kvp.Value;
            var nextIntidicie = _indicesByTypeNext[type].ToArray();
            int[] currentIntindicies = currentIndicesListValue.ToArray();
            if (currentIndicesListValue.Count == 0)
                continue;
            SimulationContext context = new SimulationContext(_cellsCurrent.Type, _cellsCurrent.Flags, _cellsNext.Type, _cellsNext.Flags, currentIntindicies, nextIntidicie);
            CellSimulationsRegistry.Simulate(type, ref context);
            _indicesByTypeCurrent[type].Clear();
            _indicesByTypeCurrent[type].AddRange(nextIntidicie);
        }

        //CellSimulationsRegistry.Simulate(CellType.Water,ref context);
        (_cellsCurrent, _cellsNext) = (_cellsNext, _cellsCurrent);
        _visualBuffer.Clear();
        for (int i = 0; i < SimulatorConst.ChunkSize3; i++)
        {
            _cellsNext.Type[i] = DefaultCells.Air.Type;
            _cellsNext.Flags[i] = DefaultCells.Air.Flags;
            if (_cellsCurrent.IsAirAt(i)) continue;
            _visualBuffer.Add(new CellVisual(i, _cellsCurrent.Type[i]));
        }

        stopwatchSimulation.Stop();
        GD.Print($"Simulate time all cells: {stopwatchSimulation.Elapsed.TotalMilliseconds:F6} ms");
        GD.Print("All cells:", _cells);
        lock (_visualLock)
        {
            (_visualInstances, _visualBuffer) = (_visualBuffer, _visualInstances);
        }

    }
    public void UpdateVisuals()
    {
        int count = 0;
        lock (_visualLock)
        {
            count = _visualInstances.Count;
            _multimesh.InstanceCount = count;

            for (int i = 0; i < count; i++)
            {
                var instance = _visualInstances[i];
                _multimesh.SetInstanceTransform(i, new Transform3D(Basis.Identity, FromIndex(instance.IndexPosition)));
                _multimesh.SetInstanceCustomData(i, CellsVisualPropertyes.GetColorForCellType(instance.Type));
            }
        }
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public int ToIndex(Vector3I pos) => pos.X + (pos.Y << 5) + (pos.Z << 10);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Vector3I FromIndex(int index) =>
        new Vector3I(index & 31, (index >> 5) & 31, (index >> 10) & 31);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public bool IsIndexInBounds(int index)
    {
        return index >= 0 && index < SimulatorConst.ChunkSize3;
    }


    public void FillColumn(int x, int z, CellType cell)
    {

        if (!((uint)x < SimulatorConst.ChunkSize && (uint)z < SimulatorConst.ChunkSize))
            return;

        for (int y = SimulatorConst.ChunkSize - 1; y >= SimulatorConst.ChunkSize - 11; y--)
        {
            Vector3I pos = new Vector3I(x, y, z);
            if (!((uint)x < SimulatorConst.ChunkSize && (uint) y < SimulatorConst.ChunkSize && (uint)z < SimulatorConst.ChunkSize) 
                || !IsIndexInBounds(ToIndex(pos))) return;
            _cellsCurrent.SetActive(ToIndex(pos), true);
            _cellsCurrent.Type[ToIndex(pos)] = cell;
            _indicesByTypeCurrent[cell].Add(ToIndex(pos));
            _indicesByTypeNext[cell].Add(ToIndex(pos));
            _cells++;
        }
    }

}
