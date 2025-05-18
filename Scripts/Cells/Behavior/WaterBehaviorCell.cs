using Godot;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    internal class WaterBehaviorCell : BaseBehaviorCell
    {
        private static readonly Vector3I[] OffsetDirections =
        [
            Vector3I.Right, Vector3I.Left,
            Vector3I.Forward, Vector3I.Back,
        ];


        public override void Simulate(Chunk chunk, Vector3I pos)
        {
            Vector3I below = pos + Vector3I.Down;

            if (chunk.IsInBounds(below))
            {
                var belowCell = chunk.GetCell(below);
                if (belowCell.IsAir && !belowCell.Reserved)
                {
                    chunk.MarkNeighborsActive(pos);
                    chunk.SwapCells(pos, below);
                    chunk.ReservedCell(below);
                    chunk.DeleteStaticCell(pos);
                    return;
                }
            }
            int startIndex = (int)(Stopwatch.GetTimestamp() & 0x3) + 1;
            //in theory its better than cycle
            if (TryMove(pos, OffsetDirections[(startIndex + 0) & 3], chunk)) return;
            if (TryMove(pos, OffsetDirections[(startIndex + 1) & 3], chunk)) return;
            if (TryMove(pos, OffsetDirections[(startIndex + 2) & 3], chunk)) return;
            if (TryMove(pos, OffsetDirections[(startIndex + 3) & 3], chunk)) return;
            chunk.SetStaticCell(pos, chunk.GetCell(pos));
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private bool TryMove(Vector3I pos, Vector3I offset, Chunk chunk)
        {
            var target = pos + offset;
            if (!chunk.IsInBounds(target))
                return false;

            var targetCell = chunk.GetCell(target);
            if (!targetCell.IsAir || targetCell.Reserved)
                return false;

            chunk.MarkNeighborsActive(pos);
            chunk.SwapCells(pos, target);
            chunk.ReservedCell(target);
            chunk.DeleteStaticCell(pos);
            return true;
        }
    }
}
