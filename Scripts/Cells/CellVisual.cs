using Godot;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VoxelParticleSimulator.Scripts.Cells
{
    struct CellVisual
    {
        public ushort IndexPosition;
        public CellType Type;

        public CellVisual(ushort Position, CellType type)
        {
            IndexPosition = Position;
            Type = type;
        }
    }
}
