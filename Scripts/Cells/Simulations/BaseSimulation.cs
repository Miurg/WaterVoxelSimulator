using Godot;
using System.Runtime.CompilerServices;
using VoxelParticleSimulator.Scripts.Cells.Simulations;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    public abstract class BaseSimulation 
    {
        protected const int dx = 1;
        protected const int dy = SimulatorConst.ChunkSize;
        protected const int dz = SimulatorConst.ChunkSize * SimulatorConst.ChunkSize;
        protected static readonly int[] _neighbours = { dx, -dx, dy, -dy, dz, -dz };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsIndexInBounds(int index)
        {
            return index >= 0 && index < SimulatorConst.ChunkSize3;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void SwapCells(int index, int targetIndex, int indexInLocalMassive, ref SimulationContext ctx)
        {
            CellType tempType = ctx.CurrentCellsTypes[targetIndex];
            CellFlags tempFlags = ctx.CurrentCellsFlags[targetIndex];
            ctx.NextCellsTypes[targetIndex] = ctx.CurrentCellsTypes[index];
            ctx.NextCellsFlags[targetIndex] = ctx.CurrentCellsFlags[index];
            ctx.NextCellsTypes[index] = tempType;
            ctx.NextCellsFlags[index] = tempFlags;
            ctx.NextIndicies[indexInLocalMassive] = targetIndex;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void SetNextStaticCellOnIndex(int index, ref SimulationContext ctx)
        {
            ctx.NextCellsTypes[index] = ctx.CurrentCellsTypes[index];
            ctx.NextCellsFlags[index] |= ctx.CurrentCellsFlags[index];
        }
        //Unroll if possible
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void MarkNeighborsActive(int index, ref SimulationContext ctx)
        {
            int i;
            i = index + dx;
            if (IsIndexInBounds(i)) ctx.SetNextCellActive(i, true);

            i = index - dx;
            if (IsIndexInBounds(i)) ctx.SetNextCellActive(i, true);

            i = index + dy;
            if (IsIndexInBounds(i)) ctx.SetNextCellActive(i, true);

            i = index - dy;
            if (IsIndexInBounds(i)) ctx.SetNextCellActive(i, true);

            i = index + dz;
            if (IsIndexInBounds(i)) ctx.SetNextCellActive(i, true);

            i = index - dz;
            if (IsIndexInBounds(i)) ctx.SetNextCellActive(i, true);
        }
    }
}
