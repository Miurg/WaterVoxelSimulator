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


    private static readonly Vector3I[] _neighborOffsets =
    [
        Vector3I.Up, Vector3I.Down,
        Vector3I.Left, Vector3I.Right,
        Vector3I.Forward, Vector3I.Back
    ];

    public override void _Ready()
    {
        _activeCells = new ActiveCellsBuffer(Size);
        _nextActiveCells = new ActiveCellsBuffer(Size);
        InitializeMultimesh();
        for (int x = 0; x < Size; x++)
            for (int y = 0; y < Size; y++)
                for (int z = 0; z < Size; z++)
                    _cellsStatic[ToIndex(new Vector3I(x, y, z))] = DefaultCells.Air;
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

        _activeCells.Swap(_nextActiveCells);
        _nextActiveCells.Clear();
        Array.Copy(_cellsStatic, _cellsNext, _cellsStatic.Length);
        var stopwatchSimulation = System.Diagnostics.Stopwatch.StartNew();
        foreach (var pos in _activeCells.GetActivePositions())
        {
            var cell = GetCell(pos);
            if (cell.IsAir) continue;
            CellBehaviorRegistry.GetBehavior(cell.Type).Simulate(this, pos);
        }

        stopwatchSimulation.Stop();
        GD.Print("Simulate time all cells: ", stopwatchSimulation.ElapsedMilliseconds, "ms");
        GD.Print("Active cells:", _activeCells.Count, ", in next frame:", _nextActiveCells.Count, ", all cells:", _cells);

        
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
    private int ToIndex(Vector3I pos) => pos.X + (pos.Y << 6) + (pos.Z << 12);
    public void ReservedCell(Vector3I pos)
    {
        _cellsCurrent[ToIndex(pos)].Reserved = true;
    }
    public void DeleteStaticCell(Vector3I pos)
    {
        _cellsStatic[ToIndex(pos)] = DefaultCells.Air;
    }
    public void SetStaticCell(Vector3I pos, Cell cell)
    {
        _cellsStatic[ToIndex(pos)] = cell;
        _cellsNext[ToIndex(pos)] = cell;
    }
    public void SetCell(Vector3I pos, Cell cell)
    {
        _cellsNext[ToIndex(pos)] = cell;
        if (CellBehaviorRegistry.IsActive(cell.Type))
            MarkActive(pos);
    }
    public Cell GetCell(Vector3I pos) => _cellsCurrent[ToIndex(pos)];
    public bool IsInBounds(Vector3I pos)
    {
        return (uint)pos.X < Size && (uint)pos.Y < Size && (uint)pos.Z < Size;
    }

    public void SwapCells(Vector3I a, Vector3I b)
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
            if (!IsInBounds(pos)) return;
            _cellsCurrent[ToIndex(pos)] = cell;
            _cells = _nextActiveCells.Count+1;
            if (CellBehaviorRegistry.IsActive(cell.Type))
                MarkActive(pos);
        }
    }
    public void MarkActive(Vector3I pos)
    {
        if (!IsInBounds(pos)) return;
        _nextActiveCells.Add(pos);
        DeleteStaticCell(pos);
    }
    public void MarkNeighborsActive(Vector3I pos)
    {
        foreach (var offset in _neighborOffsets)
        {
            MarkActive(pos + offset);
        }
    }
}
