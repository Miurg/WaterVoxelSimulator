using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using VoxelParticleSimulator.Scripts.Cells.Behavior;

namespace VoxelParticleSimulator.Scripts.Cells.Simulations.GeneralMoving.Liquid
{
    internal class LiquidMove : BaseSimulation
    {
        private readonly static int[] _offsets;

        static LiquidMove()
        {
            _offsets = new[] { dx, -dx, dz, -dz };
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void Simulate(int index, ref SimulationContext ctx)
        {
            if (!ctx.IsCurrentCellActive(ctx.CurrentIndicies[index]) || ctx.IsCurrentCellHasMoved(ctx.CurrentIndicies[index])) return;
            int startIndex = (int)(Stopwatch.GetTimestamp() & 0x3) + 1;
            if (TryMove(index, _offsets[(startIndex + 0) & 3], ref ctx)) return;
            if (TryMove(index, _offsets[(startIndex + 1) & 3], ref ctx)) return;
            if (TryMove(index, _offsets[(startIndex + 2) & 3], ref ctx)) return;
            if (TryMove(index, _offsets[(startIndex + 3) & 3], ref ctx)) return;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static bool TryMove(int index, int offset, ref SimulationContext ctx)
        {
            int targetIndex = ctx.CurrentIndicies[index] + offset;
            if (IsMoveValid(ctx.CurrentIndicies[index], offset, SimulatorConst.ChunkSize))
            {
                if ((ctx.CurrentCellsTypes[targetIndex] == CellType.Air) && !ctx.IsCurrentCellReserved(targetIndex))
                {
                    MarkNeighborsActive(ctx.CurrentIndicies[index], ref ctx);
                    SwapCells(ctx.CurrentIndicies[index], targetIndex, index, ref ctx);
                    ctx.SetCurrentCellReserved(targetIndex, true);
                    ctx.SetCurrentCellHasMoved(ctx.CurrentIndicies[index], true);
                    return true;
                }
                return false;
            }
            return false;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static bool IsMoveValid(int index, int offset, int size)
        {
            int x = index & 0x1F;              
            int y = (index >> 5) & 0x1F;      
            int z = index >> 10;

            if (offset == -1 && x == 0) return false;                // Can't go left if on the left edge
            if (offset == 1 && x == size - 1) return false;          // Can't go right if on the right edge

            //if (offset == -size && y == 0) return false;             // Can't go down if on the bottom edge
            //if (offset == size && y == size - 1) return false;       // Can't go up if on the top edge

            if (offset == -size * size && z == 0) return false;      // Can't go back if on the back edge
            if (offset == size * size && z == size - 1) return false; // Can't go forward if on the front edge

            return true; // Movement in this direction is possible
        }
    }
}
