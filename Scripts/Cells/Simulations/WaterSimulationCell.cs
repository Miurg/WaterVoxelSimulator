using Godot;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    internal class WaterSimulationCell : BaseSimulation
    {
        private readonly static int[] _offsets;
        private readonly static int _offsetDown;

        static WaterSimulationCell()
        {
            _offsets = new[] { dx, -dx, dz, -dz };
            _offsetDown = -dy;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void Simulate(int index, ref SimulationContext ctx)
        {
            //if (!ctx.IsCurrentCellActive(index))
            //{
            //    SetNextStaticCellOnIndex(index, ref ctx);
            //    return;
            //}
            //int belowIndex = index + _offsetDown;
            //if (!(((index / SimulatorConst.ChunkSize) % SimulatorConst.ChunkSize) == 0))
            //{
            //    if ((ctx.CurrentCellsTypes[belowIndex] == CellType.Air) && !ctx.IsCurrentCellReserved(belowIndex))
            //    {
            //        MarkNeighborsActive(index, ref ctx);
            //        SwapCells(index, belowIndex, ref ctx);
            //        ctx.SetCurrentCellReserved(belowIndex, true);
            //        return;
            //    }
            //}
            ////In theory its better than cycle
            //int startIndex = (int)(Stopwatch.GetTimestamp() & 0x3) + 1;
            //if (TryMove(index, _offsets[(startIndex + 0) & 3], ref ctx)) return;
            //if (TryMove(index, _offsets[(startIndex + 1) & 3], ref ctx)) return;
            //if (TryMove(index, _offsets[(startIndex + 2) & 3], ref ctx)) return;
            //if (TryMove(index, _offsets[(startIndex + 3) & 3], ref ctx)) return;
            //ctx.SetCurrentCellActive(index, false);
            //SetNextStaticCellOnIndex(index, ref ctx);
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static bool TryMove(int index, int offset, ref SimulationContext ctx)
        {
            return true;
            //int targetIndex = index + offset;
            //if (IsMoveValid(index, offset, SimulatorConst.ChunkSize))
            //{
            //    if ((ctx.CurrentCellsTypes[targetIndex] == CellType.Air) && !ctx.IsCurrentCellReserved(targetIndex))
            //    {
            //        MarkNeighborsActive(index, ref ctx);
            //        SwapCells(index,targetIndex,ref ctx);
            //        ctx.SetCurrentCellReserved(targetIndex, true);
            //        return true;
            //    }
            //    return false;
            //}
            //return false;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        //It is my personal hell.
        private static bool IsMoveValid(int index, int offset, int size)
        {
            int x = index % size;
            int y = (index / size) % size;
            int z = index / (size * size);

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
