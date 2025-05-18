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
        public static readonly Cell Sand = new() { Type = CellType.Sand, Reserved = false };
        public static readonly Cell Water = new() { Type = CellType.Water, Reserved = false };
    }
}
