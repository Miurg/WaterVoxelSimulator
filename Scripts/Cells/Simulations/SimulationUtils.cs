using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    internal static class SimulationUtils
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsIndexInBounds(int index)
        {
            return index >= 0 && index < SimulatorConst.ChunkSize3;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void SwapCells(int index, int targetIndex, SimulationContext ctx)
        {
            CellType tempType = ctx.CurrentTypes[targetIndex];
            CellFlags tempFlags = ctx.CurrentFlags[targetIndex];
            ctx.NextTypes[targetIndex] = ctx.CurrentTypes[index];
            ctx.NextFlags[targetIndex] = ctx.CurrentFlags[index];
            ctx.NextTypes[index] = tempType;
            ctx.NextFlags[index] = tempFlags;
        }
    }
}
