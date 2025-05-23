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
        public static void Simulate(int index, ref SimulationContext ctx)
        {
            if (!ctx.IsCurrentCellActive(ctx.CurrentIndicies[index])) return;

            int belowIndex = ctx.CurrentIndicies[index] -SimulatorConst.ChunkSize;
            if (!IsIndexInBounds(belowIndex)) return;

            bool isNotBottomRow = ((ctx.CurrentIndicies[index] >> 5) & (SimulatorConst.ChunkSize - 1)) > 0;
            if (!isNotBottomRow) return;

            if ((ctx.CurrentCellsTypes[belowIndex] == CellType.Air) && !ctx.IsCurrentCellReserved(belowIndex))
            {
                MarkNeighborsActive(ctx.CurrentIndicies[index], ref ctx);
                SwapCells(ctx.CurrentIndicies[index], belowIndex, index, ref ctx);
                ctx.SetCurrentCellReserved(belowIndex, true);
                ctx.SetCurrentCellHasMoved(ctx.CurrentIndicies[index], true);
            }
        }
    }
}
