using Godot;
using System;
using System.Diagnostics;
using System.Threading.Tasks;
using VoxelParticleSimulator.Scripts.Cells;

public partial class MainNode : Node
{
    public Chunk[] chunk = new Chunk[16];
    public int cells = 0;
    public static int FillSize = 7;
    public override void _Ready()
    {
        for (short i = 0; i < 4; i++)
        {
            for (short r = 0; r < 4; r++)
            {
                chunk[(i*4)+r] = GD.Load<PackedScene>("res://Chunk.tscn").Instantiate<Chunk>();
                AddChild(chunk[(i * 4) + r]);
                chunk[(i * 4) + r].Position = new Vector3(33*r, 0, 33 * i);
                for (ushort k = 0; k < 30; k++)
                    for (ushort j = 0; j < 30; j++)
                    {
                        chunk[(i * 4) + r].FillColumn(k, j, CellType.Water);
                        cells+= FillSize;
                    }

                Debug.Write("Chunk Ready");
                chunk[(i * 4) + r].UpdateVisuals();
            }
        }
        //Task task1 = Task.Run(async () => await RunTasks());
    }
    //private async Task RunTasks()
    //{
    //    while (true) 
    //        chunk.Simulate();
    //}

    public override void _PhysicsProcess(double delta)
    {
        var stopwatchSimulation = System.Diagnostics.Stopwatch.StartNew();
        for (short i = 0; i < 16; i++)
        {
            chunk[i].Simulate();
        }
        stopwatchSimulation.Stop();
        GD.Print($"Simulate time all cells: {stopwatchSimulation.Elapsed.TotalMilliseconds:F6} ms");
        GD.Print("All cells:", cells);

    }
    public override void _Process(double delta)
    {
        for (short i = 0; i < 16; i++)
        {
            chunk[i].UpdateVisuals();
        }
    }
}
