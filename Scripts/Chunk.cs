using Godot;
using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using VoxelParticleSimulator.Scripts.Cells;
using VoxelParticleSimulator.Scripts.Cells.Behavior;

public partial class Chunk : Node3D
{
    public const int Size = 64;
    private Cell[,,] _cellsCurrent = new Cell[Size, Size, Size];
    private Cell[,,] _cellsNext = new Cell[Size, Size, Size];
    private Cell[,,] _cellsStatic = new Cell[Size, Size, Size];
    private HashSet<Vector3I> _activeCells = new();
    private HashSet<Vector3I> _nextActiveCells = new();
    private List<(Vector3 Position, Color Color)> _visualInstances = new();
    private List<(Vector3 Position, Color Color)> _visualBuffer = new();
    private object _visualLock = new();
    private int _cells = 0;

    private MultiMeshInstance3D _multimeshInstance;
    private MultiMesh _multimesh;
    [Export]
    private Mesh _cellMesh;
    private Action<Chunk, Vector3I>[] _simulateDelegates;


    private static readonly Vector3I[] _neighborOffsets =
    [
        Vector3I.Up, Vector3I.Down,
        Vector3I.Left, Vector3I.Right,
        Vector3I.Forward, Vector3I.Back
    ];

    public override void _Ready()
    {
        InitializeMultimesh();
        for (int x = 0; x < Size; x++)
            for (int y = 0; y < Size; y++)
                for (int z = 0; z < Size; z++)
                    _cellsStatic[x, y, z] = DefaultCells.Air;
        _simulateDelegates = new Action<Chunk, Vector3I>[Enum.GetValues<CellType>().Length];

        foreach (CellType type in Enum.GetValues<CellType>())
        {
            _simulateDelegates[(int)type] = CellBehaviorRegistry.Behaviors[type].Simulate;
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
       
        (_activeCells, _nextActiveCells) = (_nextActiveCells, _activeCells);
        _nextActiveCells.Clear();
        _cellsNext = (Cell[,,])_cellsStatic.Clone();
        var stopwatch = System.Diagnostics.Stopwatch.StartNew();
        foreach (var pos in _activeCells)
        {
            var cell = GetCell(pos);
            if (cell.IsAir) continue;
            _simulateDelegates[(int)cell.Type](this, pos);
        }
        stopwatch.Stop();
        GD.Print("Simulate time: ", stopwatch.ElapsedMilliseconds, "ms");
        GD.Print("Активных клеток:", _activeCells.Count, ", в следующем кадре:", _nextActiveCells.Count, ", всего клеток:", _cells);
        (_cellsCurrent, _cellsNext) = (_cellsNext, _cellsCurrent);
        _visualBuffer.Clear();
        for (int x = 0; x < Size; x++)
            for (int y = 0; y < Size; y++)
                for (int z = 0; z < Size; z++)
                {
                    if (_cellsCurrent[x, y, z].IsAir) continue;
                    _visualBuffer.Add((new Vector3(x, y, z), _cellsCurrent[x, y, z].Color));
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
                _multimesh.SetInstanceCustomData(i, instance.Color);
            }
        }
    }
    public void ReservedCell(Vector3I pos)
    {
        _cellsCurrent[pos.X, pos.Y, pos.Z].Reserved = true;
    }
    public void DeleteStaticCell(Vector3I pos)
    {
        _cellsStatic[pos.X, pos.Y, pos.Z] = DefaultCells.Air;
    }
    public void SetStaticCell(Vector3I pos, Cell cell)
    {
        _cellsStatic[pos.X, pos.Y, pos.Z] = cell;
        _cellsNext[pos.X, pos.Y, pos.Z] = cell;
    }
    public void SetCell(Vector3I pos, Cell cell)
    {
        _cellsNext[pos.X, pos.Y, pos.Z] = cell;
        if (CellBehaviorRegistry.IsActive(cell.Type))
            MarkActive(pos);
    }
    public Cell GetCell(Vector3I pos) => _cellsCurrent[pos.X, pos.Y, pos.Z];
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
            if (!IsInBounds(new Vector3I(x, y, z))) return;
            _cellsCurrent[x, y, z] = cell;
            _cells = _nextActiveCells.Count+1;
            if (CellBehaviorRegistry.IsActive(cell.Type))
                MarkActive(new Vector3I(x, y, z));
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
