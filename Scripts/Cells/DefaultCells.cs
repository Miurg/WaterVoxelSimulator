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
        public static readonly Cell Air = new (CellType.Air, CellFlags.None);
        public static readonly Cell Sand = new (CellType.Sand, CellFlags.Active);
        public static readonly Cell Water = new (CellType.Water, CellFlags.Active);
    }
}
