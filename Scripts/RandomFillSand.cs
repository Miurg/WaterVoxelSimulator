using Godot;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VoxelParticleSimulator.Scripts.Cells;

namespace VoxelParticleSimulator.Scripts
{

    public partial class RandomFillSand : Button
    {
        [Export]
        public MainNode mainnode;
        public override void _Ready()
        {
            this.Pressed += OnButtonPressed;
        }

        private void OnButtonPressed()
        {
            mainnode.chunk.FillColumn(Random.Shared.Next(0, 63), Random.Shared.Next(0, 63), CellType.Sand);
            GD.Print("Pressed sand");
        }
    }
}
