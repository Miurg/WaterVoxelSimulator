using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VoxelParticleSimulator.Scripts.Cells
{
    [Flags]
    public enum CellFlags : byte
    {
        None = 0,
        Reserved = 1 << 0, // 00000001
        Active = 1 << 1    // 00000010
    }

}
