using System.Runtime.CompilerServices;
using VoxelParticleSimulator.Scripts.Cells.Behavior;
using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.X86;
using System;
using System.Numerics;
using Godot;

namespace VoxelParticleSimulator.Scripts.Cells.Simulations.GeneralMoving
{
    internal unsafe class MovingCellsDownMove : BaseSimulation
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void Simulate(ushort index, ref SimulationContext ctx)
        {
            var currentInd = ctx.CurrentIndicies[index];

            if (!ctx.IsCurrentCellActive(currentInd)) return;

            int belowIndex = currentInd - SimulatorConst.ChunkSize;
            if (currentInd < SimulatorConst.ChunkSize) return;

            bool isNotBottomRow = ((currentInd >> 5) & (SimulatorConst.ChunkSize - 1)) > 0;
            if (!isNotBottomRow) return;

            if ((ctx.CurrentCellsTypes[belowIndex] == CellType.Air) && !ctx.IsCurrentCellReserved((ushort)belowIndex))
            {
                MarkNeighborsActive(currentInd, ref ctx);
                SwapCells(currentInd, (ushort)belowIndex, index, ref ctx);
                ctx.SetCurrentCellReserved((ushort)belowIndex, true);
                ctx.SetCurrentCellHasMoved(currentInd, true);
            }
        }
    }
}
