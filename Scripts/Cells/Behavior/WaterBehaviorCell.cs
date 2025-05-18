using Godot;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    internal class WaterBehaviorCell : BaseBehaviorCell
    {
        private static int[] _offsets;
        private int _offsetDown;

        private void PrecomputeOffsets(int size)
        {
            int dx = 1;
            int dy = size;
            int dz = size * size;

            _offsets = new[]
            {
                dx, -dx,   // Left / Right
                dz, -dz    // Forward / Back
            };

            _offsetDown = -dy;
        }
        public override void Simulate(Chunk chunk, int index)
        {
            PrecomputeOffsets(Chunk.Size);

            int belowIndex = index + _offsetDown;
            if (!(((index / Chunk.Size) % Chunk.Size) == 0) && chunk.IsIndexInBounds(belowIndex))
            {
                var belowCell = chunk.GetCell(belowIndex);
                if (belowCell.IsAir && !belowCell.Reserved)
                {
                    chunk.MarkNeighborsActive(index);
                    chunk.SwapCells(index, belowIndex);
                    chunk.ReservedCell(belowIndex);
                    chunk.DeleteStaticCell(index);
                    return;
                }
            }
                //In theory its better than cycle
            int startIndex = (int)(Stopwatch.GetTimestamp() & 0x3) + 1;
            if (TryMove(index, _offsets[(startIndex + 0) & 3], chunk)) return;
            if (TryMove(index, _offsets[(startIndex + 1) & 3], chunk)) return;
            if (TryMove(index, _offsets[(startIndex + 2) & 3], chunk)) return;
            if (TryMove(index, _offsets[(startIndex + 3) & 3], chunk)) return;
            chunk.SetStaticCell(index, chunk.GetCell(index));
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private bool TryMove(int index, int offset, Chunk chunk)
        {
            int targetIndex = index + offset;
            if (IsMoveValid(index, offset, Chunk.Size) && chunk.IsIndexInBounds(targetIndex))
            {
                var targetCell = chunk.GetCell(targetIndex);
                if (targetCell.IsAir && !targetCell.Reserved)
                {
                    chunk.MarkNeighborsActive(index);
                    chunk.SwapCells(index, targetIndex);
                    chunk.ReservedCell(targetIndex);
                    chunk.DeleteStaticCell(index);
                    return true;
                }
                return false;
            }
            return false;
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
