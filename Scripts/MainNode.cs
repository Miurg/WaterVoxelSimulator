using Godot;
using System;
using System.Diagnostics;
using VoxelParticleSimulator.Scripts.Cells;

public partial class MainNode : Node
{
    public Chunk chunk;

    public override void _Ready()
    {
        chunk = GD.Load<PackedScene>("res://Chunk.tscn").Instantiate<Chunk>();
        Debug.Write("Chunk Ready");
        AddChild(chunk);
        for (int i = 0; i < 1000; i++)
            chunk.FillColumn(Random.Shared.Next(1, 63), Random.Shared.Next(1, 63), DefaultCells.Water); 
        chunk.UpdateVisuals();
    }

    public override void _PhysicsProcess(double delta)
    {
        chunk.Simulate();
    }
    public override void _Process(double delta)
    {
        chunk.UpdateVisuals();
    }
}
