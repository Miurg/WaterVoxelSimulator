using Godot;
using System.Linq;
namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    public class SandBehaviorCell : BaseBehaviorCell
    {
        private static readonly int[] DirectionIndices = [0, 1, 2, 3];
        private static readonly Vector3I[] OffsetDirections =
        [
           Vector3I.Right, Vector3I.Left,
            Vector3I.Forward, Vector3I.Back,
        ];

        private static readonly RandomNumberGenerator rng = new();

        public override void Simulate(Chunk chunk, int index)
        {
            //Vector3I below = pos + Vector3I.Down;

            //if (chunk.IsInBounds(below) && (chunk.GetCell(below).IsAir || chunk.GetCell(below).Type == CellType.Water) && !chunk.GetCell(below).Reserved)
            //{
            //    chunk.MarkNeighborsActive(pos);
            //    chunk.MarkNeighborsActive(below);
            //    chunk.SwapCells(pos, below);
            //    chunk.ReservedCell(below);
            //    return;
            //}
            //int n = DirectionIndices.Length;
            //while (n > 1)
            //{
            //    int k = rng.RandiRange(0, n - 1);
            //    n--;
            //    (DirectionIndices[n], DirectionIndices[k]) = (DirectionIndices[k], DirectionIndices[n]);
            //}
            //for (int i = 0; i < 4; i++)
            //{
            //    var offset = OffsetDirections[DirectionIndices[i]];
            //    var target = pos + Vector3I.Down + offset;
            //    if (chunk.IsInBounds(target) && (chunk.GetCell(target).IsAir || chunk.GetCell(target).Type == CellType.Water) && !chunk.GetCell(target).Reserved)
            //    {
            //        chunk.MarkNeighborsActive(pos);
            //        chunk.MarkNeighborsActive(target);
            //        chunk.SwapCells(pos, target);
            //        chunk.ReservedCell(target);
            //        return;
            //    }
            //}
            //var cell = chunk.GetCell(pos);
            //chunk.SetStaticCell(pos, cell);
        }
    }
}
