using Godot;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VoxelParticleSimulator.Scripts.Cells
{
    public static class DefaultCells
    {
        public static readonly Cell Air = new() { Type = CellType.Air, Reserved = false };
        public static readonly Cell Sand = new() { Type = CellType.Sand, Color = new Color(0.9f, 0.8f, 0.4f), Reserved = false };
        public static readonly Cell Water = new() { Type = CellType.Water, Color = new Color(0.1f, 0.3f, 0.8f), Reserved = false };
    }
}
