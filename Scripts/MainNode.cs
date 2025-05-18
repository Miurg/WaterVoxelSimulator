using Godot;
using System;
using System.Diagnostics;
using System.Threading.Tasks;
using VoxelParticleSimulator.Scripts.Cells;

public partial class MainNode : Node
{
    public Chunk chunk;

    public override void _Ready()
    {
        chunk = GD.Load<PackedScene>("res://Chunk.tscn").Instantiate<Chunk>();
        Debug.Write("Chunk Ready");
        AddChild(chunk);
        for (int i = 0; i < 63; i++)
            for (int j = 0; j < 63; j++)
                chunk.FillColumn(i, j, DefaultCells.Water); 
        chunk.UpdateVisuals();
        Task task1 = Task.Run(async () => await RunTasks());
    }
    private async Task RunTasks()
    {
        while (true) 
            chunk.Simulate();
    }

    public override void _PhysicsProcess(double delta)
    {
        //chunk.Simulate();
    }
    public override void _Process(double delta)
    {
        chunk.UpdateVisuals();
    }
}
