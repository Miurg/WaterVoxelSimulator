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
        public static void Simulate(ushort index, ref SimulationContext ctx)
        {
            var currentInd = ctx.CurrentIndicies[index];
            if (!ctx.IsCurrentCellActive(currentInd) || ctx.IsCurrentCellHasMoved(currentInd)) return;
            int startIndex = (int)(Stopwatch.GetTimestamp() & 0x3) + 1;
            if (TryMove(currentInd, _offsets[(startIndex + 0) & 3], index, ref ctx)) return;
            if (TryMove(currentInd, _offsets[(startIndex + 1) & 3], index, ref ctx)) return;
            if (TryMove(currentInd, _offsets[(startIndex + 2) & 3], index, ref ctx)) return;
            if (TryMove(currentInd, _offsets[(startIndex + 3) & 3], index, ref ctx)) return;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static bool TryMove(ushort index, int offset, ushort indexInMassive, ref SimulationContext ctx)
        {
            int targetIndex = index + offset;
            if (IsMoveValid(index, offset, SimulatorConst.ChunkSize))
            {
                if ((ctx.CurrentCellsTypes[targetIndex] == CellType.Air) && !ctx.IsCurrentCellReserved((ushort)targetIndex))
                {
                    //MarkNeighborsActive(index, ref ctx);
                    SwapCells(index, (ushort)targetIndex, indexInMassive, ref ctx);
                    ctx.SetCurrentCellReserved((ushort)targetIndex, true);
                    ctx.SetCurrentCellHasMoved(index, true);
                    return true;
                }
                return false;
            }
            return false;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static bool IsMoveValid(int index, int offset, int size)
        {
            int x = index & 31;
            int layerSize = size * size;

            if ((offset == -1 && x == 0) ||
                (offset == 1 && x == size - 1) ||
                (offset == -layerSize && index < layerSize) ||
                (offset == layerSize && index >= layerSize * (size - 1)))
                return false;

            return true;
        }
    }
}
