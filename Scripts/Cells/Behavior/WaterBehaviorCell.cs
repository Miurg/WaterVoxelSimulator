using Godot;
using System.Linq;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    internal class WaterBehaviorCell : BaseBehaviorCell
    {
        private static readonly Vector3I[] OffsetDirections =
        [
            Vector3I.Right, Vector3I.Left,
            Vector3I.Forward, Vector3I.Back,
        ];

        private static readonly RandomNumberGenerator rng = new();

        public override void Simulate(Chunk chunk, Vector3I pos)
        {
            Vector3I below = pos + Vector3I.Down;

            if (chunk.IsInBounds(below) && chunk.GetCell(below).IsAir && !chunk.GetCell(below).Reserved)
            {
                chunk.MarkNeighborsActive(pos);
                chunk.MarkNeighborsActive(below);
                chunk.SwapCells(pos, below);
                chunk.ReservedCell(below);
                chunk.DeleteStaticCell(pos);
                return;
            }

            var directions = OffsetDirections.ToArray();
            directions.Shuffle(rng);

            foreach (var offset in directions)
            {
                var target = pos + offset;
                if (chunk.IsInBounds(target) && chunk.GetCell(target).IsAir && !chunk.GetCell(target).Reserved)
                {
                    chunk.MarkNeighborsActive(pos);
                    chunk.MarkNeighborsActive(target);
                    chunk.SwapCells(pos, target);
                    chunk.ReservedCell(target);
                    chunk.DeleteStaticCell(pos);
                    return;
                }
            }
            var cell = chunk.GetCell(pos);
            chunk.SetStaticCell(pos, cell);
        }
    }
}
