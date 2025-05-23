using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    //This structure is aggressive like a mad dog. Woof-woof!
    public readonly ref struct SimulationContext
    {
        public readonly Span<CellType> CurrentCellsTypes;
        public readonly Span<CellFlags> CurrentCellsFlags;
        public readonly Span<CellType> NextCellsTypes;
        public readonly Span<CellFlags> NextCellsFlags;
        public readonly Span<ushort> CurrentIndicies;
        public readonly Span<ushort> NextIndicies;
        public SimulationContext(Span<CellType> currentTypes, Span<CellFlags> currentFlags, Span<CellType> nextTypes, Span<CellFlags> nextFlags, Span<ushort> currentInd, Span<ushort> nextInd)
        {
            CurrentCellsTypes = currentTypes;
            CurrentCellsFlags = currentFlags;
            NextCellsTypes = nextTypes;
            NextCellsFlags = nextFlags;
            CurrentIndicies = currentInd;
            NextIndicies = nextInd;
        }
        public bool IsCurrentCellReserved(ushort index) => (CurrentCellsFlags[index] & CellFlags.Reserved) != 0;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public bool IsCurrentCellActive(ushort index) => (CurrentCellsFlags[index] & CellFlags.Active) != 0;
        public bool IsCurrentCellHasMoved(ushort index) => (CurrentCellsFlags[index] & CellFlags.HasMoved) != 0;
        public void SetCurrentCellReserved(ushort index, bool value)
        {
            ref var flags = ref CurrentCellsFlags[index];
            if (value)
                flags |= CellFlags.Reserved;
            else
                flags &= ~CellFlags.Reserved;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void SetCurrentCellActive(ushort index, bool value)
        {
            ref var flags = ref CurrentCellsFlags[index];
            if (value)
                flags |= CellFlags.Active;
            else
                flags &= ~CellFlags.Active;
        }
        public void SetCurrentCellHasMoved(ushort index, bool value)
        {
            ref var flags = ref CurrentCellsFlags[index];
            if (value)
                flags |= CellFlags.HasMoved;
            else
                flags &= ~CellFlags.HasMoved;
        }
        public bool IsNextCellReserved(ushort index) => (NextCellsFlags[index] & CellFlags.Reserved) != 0;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public bool IsNextCellActive(ushort index) => (NextCellsFlags[index] & CellFlags.Active) != 0;
        public bool IsNextCellHasMoved(ushort index) => (NextCellsFlags[index] & CellFlags.HasMoved) != 0;
        public void SetNextCellReserved(ushort index, bool value)
        {
            ref var flags = ref NextCellsFlags[index];
            if (value)
                flags |= CellFlags.Reserved;
            else
                flags &= ~CellFlags.Reserved;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void SetNextCellActive(ushort index, bool value)
        {
            ref var flags = ref NextCellsFlags[index];
            if (value)
                flags |= CellFlags.Active;
            else
                flags &= ~CellFlags.Active;
        }
        public void SetNextCellHasMoved(ushort index, bool value)
        {
            ref var flags = ref NextCellsFlags[index];
            if (value)
                flags |= CellFlags.HasMoved;
            else
                flags &= ~CellFlags.HasMoved;
        }
    }
}
