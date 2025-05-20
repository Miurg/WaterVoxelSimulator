using Godot;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    internal class WaterSimulationCell : BaseSimulationCell
    {
        private static int[] _neighbours;
        private static int[] _offsets;
        private static int _offsetDown;

        static WaterSimulationCell()
        {
            int size = SimulatorConst.ChunkSize;
            int dx = 1;
            int dy = size;
            int dz = size * size;
            _neighbours = new int[]{dx, -dx, dy, -dy, dz, -dz};
            _offsets = new[] { dx, -dx, dz, -dz };
            _offsetDown = -dy;
        }
        public override void Simulate(int index, ref SimulationContext ctx)
        {
            if (!ctx.IsCurrentActive(index))
            {
                ctx.NextTypes[index] = CellType.Water;
                ctx.SetNextActive(index,false);
                return;
            }
            int belowIndex = index + _offsetDown;
            if (!(((index / SimulatorConst.ChunkSize) % SimulatorConst.ChunkSize) == 0) && SimulationUtils.IsIndexInBounds(belowIndex))
            {
                if ((ctx.CurrentTypes[belowIndex] == CellType.Air) && !ctx.IsCurrentReserved(belowIndex))
                {
                    MarkNeighborsActive(index, ctx);
                    SimulationUtils.SwapCells(index, belowIndex, ctx);
                    ctx.SetCurrentReserved(belowIndex, true);
                    return;
                }
            }
            //In theory its better than cycle
            int startIndex = (int)(Stopwatch.GetTimestamp() & 0x3) + 1;
            if (TryMove(index, _offsets[(startIndex + 0) & 3], ctx)) return;
            if (TryMove(index, _offsets[(startIndex + 1) & 3], ctx)) return;
            if (TryMove(index, _offsets[(startIndex + 2) & 3], ctx)) return;
            if (TryMove(index, _offsets[(startIndex + 3) & 3], ctx)) return;
            ctx.NextTypes[index] = CellType.Water;
            ctx.SetNextActive(index, false);
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private bool TryMove(int index, int offset, SimulationContext ctx)
        {
            int targetIndex = index + offset;
            if (IsMoveValid(index, offset, SimulatorConst.ChunkSize) && SimulationUtils.IsIndexInBounds(targetIndex))
            {
                if ((ctx.CurrentTypes[targetIndex] == CellType.Air) && !ctx.IsCurrentReserved(targetIndex))
                {
                    MarkNeighborsActive(index, ctx);
                    SimulationUtils.SwapCells(index,targetIndex,ctx);
                    ctx.SetCurrentReserved(targetIndex, true);
                    return true;
                }
                return false;
            }
            return false;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private void MarkNeighborsActive(int index, SimulationContext ctx)
        {
            foreach (var neighbour in _neighbours)
            {
                var targetNeighbour = index + neighbour;
                if (SimulationUtils.IsIndexInBounds(targetNeighbour))
                {
                    ctx.SetNextActive(targetNeighbour, true);
                    ctx.SetCurrentActive(targetNeighbour, true);
                }
            }
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        //It is my personal hell.
        private bool IsMoveValid(int index, int offset, int size)
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
