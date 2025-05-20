using Godot;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    public class SandSimulationCell : BaseSimulationCell
    {
        private static int[] _offsets;
        private static int _offsetDown;

        static SandSimulationCell()
        {
            int size = SimulatorConst.ChunkSize;
            int dx = 1;
            int dy = size;
            int dz = size * size;

            _offsets = new[] {dx - dy, -dx - dy, dz - dy, -dz - dy};

            _offsetDown = -dy;
        }

        public override void Simulate(int index, ref SimulationContext ctx)
        {

            //    int belowIndex = index + _offsetDown;
            //    if (!(((index / SimulatorConst.ChunkSize) % SimulatorConst.ChunkSize) == 0) && SimulationUtils.IsIndexInBounds(belowIndex))
            //    {
            //        if (((ctx.CurrentTypes[belowIndex] == CellType.Air) || (ctx.CurrentTypes[belowIndex] == CellType.Water)) && !ctx.CurrentFlags[belowIndex])
            //        {
            //            ctx.Chunk.MarkNeighborsActive(index);
            //            ctx.Chunk.SwapCells(index, belowIndex);
            //            ctx.Chunk.ReservedCell(belowIndex);
            //            ctx.Chunk.DeleteStaticCell(index);
            //            return;
            //        }
            //    }
            //    //In theory its better than cycle
            //    int startIndex = (int)(Stopwatch.GetTimestamp() & 0x3) + 1;
            //    if (TryMove(index, _offsets[(startIndex + 0) & 3], ctx)) return;
            //    if (TryMove(index, _offsets[(startIndex + 1) & 3], ctx)) return;
            //    if (TryMove(index, _offsets[(startIndex + 2) & 3], ctx)) return;
            //    if (TryMove(index, _offsets[(startIndex + 3) & 3], ctx)) return;
            //    ctx.Chunk.SetStaticCell(index, new Cell(ctx.CurrentTypes[index], ctx.CurrentFlags[index], ctx.Active[index]));
        }
        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        //private bool TryMove(int index, int offset, SimulationContext ctx)
        //{
        //    int targetIndex = index + offset;
        //    if (IsMoveValid(index, offset, SimulatorConst.ChunkSize) && SimulationUtils.IsIndexInBounds(targetIndex))
        //    {
        //        if (((ctx.CurrentTypes[targetIndex] == CellType.Air) || (ctx.CurrentTypes[targetIndex] == CellType.Water)) && !ctx.CurrentFlags[targetIndex])
        //        {
        //            ctx.Chunk.MarkNeighborsActive(index);
        //            ctx.Chunk.SwapCells(index, targetIndex);
        //            ctx.Chunk.ReservedCell(targetIndex);
        //            ctx.Chunk.DeleteStaticCell(index);
        //            return true;
        //        }
        //        return false;
        //    }
        //    return false;
        //}
        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        //private bool IsMoveValid(int index, int offset, int size)
        //{
        //    int x = index % size;
        //    int y = (index / size) % size;
        //    int z = index / (size * size);

        //    if (y == 0) return false;             // Can't go at all if on the bottom edge
        //    //if (y == size - 1) return false;       // Can't go up if on the top edge

        //    if (offset == -1 - size && x == 0) return false;                // Can't go left-down if on the left edge
        //    if (offset == 1 - size && x == size - 1) return false;          // Can't go right-down if on the right edge

        //    if (offset == -size * size - size && z == 0) return false;      // Can't go back-down if on the back edge
        //    if (offset == size * size - size && z == size - 1) return false; // Can't go forward-down if on the front edge

        //    return true; // Movement in this direction is possible
        //}
    }
}
