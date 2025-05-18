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
        public Vector3 Position;
        public CellType Type;

        public CellVisual(Vector3 position, CellType type)
        {
            Position = position;
            Type = type;
        }
    }
}
