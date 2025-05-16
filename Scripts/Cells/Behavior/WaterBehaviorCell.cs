using Godot;
using System.Linq;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    internal class WaterBehaviorCell : BaseBehaviorCell
    {
        private static readonly int[] DirectionIndices = [0, 1, 2, 3];
        private static readonly Vector3I[] OffsetDirections =
        [
            Vector3I.Right, Vector3I.Left,
            Vector3I.Forward, Vector3I.Back,
        ];

        private static readonly RandomNumberGenerator rng = new();

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
            int n = DirectionIndices.Length;
            while (n > 1)
            {
                int k = rng.RandiRange(0, n - 1);
                n--;
                (DirectionIndices[n], DirectionIndices[k]) = (DirectionIndices[k], DirectionIndices[n]);
            }
            for (int i = 0; i < 4; i++)
            {
                var offset = OffsetDirections[DirectionIndices[i]];
                var target = pos + offset;
                if (chunk.IsInBounds(target))
                {
                    var targetCell = chunk.GetCell(target);
                    if (targetCell.IsAir && !targetCell.Reserved)
                    {
                        chunk.MarkNeighborsActive(pos);
                        chunk.SwapCells(pos, target);
                        chunk.ReservedCell(target);
                        chunk.DeleteStaticCell(pos);
                        return;
                    }
                }
            }
            var cell = chunk.GetCell(pos);
            chunk.SetStaticCell(pos, cell);
        }
    }
}
