using Godot;
using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using VoxelParticleSimulator.Scripts.Cells;
using VoxelParticleSimulator.Scripts.Cells.Behavior;
using VoxelParticleSimulator.Scripts.Chunks;

public partial class Chunk : Node3D
{
    public const int Size = 64;
    private Cell[] _cellsCurrent = new Cell[Size * Size * Size];
    private Cell[] _cellsNext = new Cell[Size * Size * Size];
    private Cell[] _cellsStatic = new Cell[Size * Size * Size];
    private ActiveCellsBuffer _activeCells;
    private ActiveCellsBuffer _nextActiveCells;
    private List<CellVisual> _visualInstances = new();
    private List<CellVisual> _visualBuffer = new();
    private object _visualLock = new();
    private int _cells = 0;

    private MultiMeshInstance3D _multimeshInstance;
    private MultiMesh _multimesh;
    [Export]
    private Mesh _cellMesh;


    private int[] _neighborOffsetsByIndex;
    private void InitializeOffsets()
    {
        int dx = 1;
        int dy = Size;
        int dz = Size * Size;

        _neighborOffsetsByIndex = new int[]
        {
        dx, -dx,
        dy, -dy,
        dz, -dz
        };
    }
    public override void _Ready()
    {
        _activeCells = new ActiveCellsBuffer(Size);
        _nextActiveCells = new ActiveCellsBuffer(Size);
        InitializeMultimesh();
        InitializeOffsets();
        for (int i = 0; i < Size*Size*Size; i++)
        {
            _cellsStatic[i] = DefaultCells.Air;
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
        _multimesh.InstanceCount = Size * Size * Size;
    }
    public void Simulate()
    {
        var stopwatchSimulation = System.Diagnostics.Stopwatch.StartNew();
        _activeCells.Swap(_nextActiveCells);
        _nextActiveCells.Clear();
        Array.Copy(_cellsStatic, _cellsNext, _cellsStatic.Length);
        foreach (var index in _activeCells.GetActiveIndices())
        {
            var cell = _cellsCurrent[index];
            if (cell.IsAir) continue;
            CellBehaviorRegistry.GetBehavior(cell.Type).Simulate(this, index);
        }
        (_cellsCurrent, _cellsNext) = (_cellsNext, _cellsCurrent);
        _visualBuffer.Clear();
        for (int x = 0; x < Size; x++)
            for (int y = 0; y < Size; y++)
                for (int z = 0; z < Size; z++)
                {
                    Vector3I pos = new Vector3I(x, y, z);
                    if (_cellsCurrent[ToIndex(pos)].IsAir) continue;
                    _visualBuffer.Add(new CellVisual(new Vector3(x, y, z), _cellsCurrent[ToIndex(pos)].Type));
                }
        stopwatchSimulation.Stop();
        GD.Print("Simulate time all cells: ", stopwatchSimulation.ElapsedMilliseconds, "ms");
        GD.Print("Active cells:", _activeCells.Count, ", in next frame:", _nextActiveCells.Count, ", all cells:", _cells);
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
                _multimesh.SetInstanceTransform(i, new Transform3D(Basis.Identity, instance.Position));
                _multimesh.SetInstanceCustomData(i, CellsVisualPropertyes.GetColorForCellType(instance.Type));
            }
        }
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public int ToIndex(Vector3I pos) => pos.X + (pos.Y << 6) + (pos.Z << 12);
    //[MethodImpl(MethodImplOptions.AggressiveInlining)]
    //public Vector3I FromIndex(int index) => new Vector3I(index & 63,(index >>> 6) & 63,(index >>> 12) & 63);
    
    public void ReservedCell(int index)
    {
        _cellsCurrent[index].Reserved = true;
    }
    public void DeleteStaticCell(int index)
    {
        _cellsStatic[index] = DefaultCells.Air;
    }

    //We add the cell to the _cellsNext array as well - so that it can be known already in the current iteration. 
    //At the beginning of the next iteration, it will already be rewritten itself in _cellsNext from _cellsStatic.
    public void SetStaticCell(int index, Cell cell)
    {
        _cellsStatic[index] = cell;
        _cellsNext[index] = cell;
    }
    public void SetCell(int index, Cell cell)
    {
        _cellsNext[index] = cell;
        if (CellBehaviorRegistry.IsActive(cell.Type))
            MarkActive(index);
    }
    public Cell GetCell(int index) => _cellsCurrent[index];
    public bool IsInBounds(Vector3I pos)
    {
        return (uint)pos.X < Size && (uint)pos.Y < Size && (uint)pos.Z < Size;
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public bool IsIndexInBounds(int index)
    {
        return index >= 0 && index < Size * Size * Size;
    }

    public void SwapCells(int a, int b)
    {
        var temp = GetCell(a);
        SetCell(a, GetCell(b));
        SetCell(b, temp);
    }

    public void FillColumn(int x, int z, Cell cell)
    {
        if(!IsInBounds(new Vector3I(x, Size-1, z)))
            return;

        int half = Size / 2;
        for (int y = Size - 1; y >= Size - 25; y--)
        {
            Vector3I pos = new Vector3I(x, y, z);
            if (!IsInBounds(pos) || !IsIndexInBounds(ToIndex(pos))) return;
            _cellsCurrent[ToIndex(pos)] = cell;
            _cells = _nextActiveCells.Count+1;

            if (CellBehaviorRegistry.IsActive(cell.Type))
                MarkActive(ToIndex(pos));
        }
    }
    public void MarkActive(int index)
    {
        if (!IsIndexInBounds(index)) return;
        _nextActiveCells.Add(index);
        DeleteStaticCell(index);
    }
    public void MarkNeighborsActive(int index)
    {
        foreach (var offset in _neighborOffsetsByIndex)
        {
            MarkActive(index + offset);
        }
    }

}
