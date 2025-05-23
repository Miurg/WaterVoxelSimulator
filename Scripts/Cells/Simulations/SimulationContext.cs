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
        public readonly Span<int> CurrentIndicies;
        public readonly Span<int> NextIndicies;
        public SimulationContext(Span<CellType> currentTypes, Span<CellFlags> currentFlags, Span<CellType> nextTypes, Span<CellFlags> nextFlags, Span<int> currentInd, Span<int> nextInd)
        {
            CurrentCellsTypes = currentTypes;
            CurrentCellsFlags = currentFlags;
            NextCellsTypes = nextTypes;
            NextCellsFlags = nextFlags;
            CurrentIndicies = currentInd;
            NextIndicies = nextInd;
        }
        public bool IsCurrentCellReserved(int index) => (CurrentCellsFlags[index] & CellFlags.Reserved) != 0;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public bool IsCurrentCellActive(int index) => (CurrentCellsFlags[index] & CellFlags.Active) != 0;
        public bool IsCurrentCellHasMoved(int index) => (CurrentCellsFlags[index] & CellFlags.HasMoved) != 0;
        public void SetCurrentCellReserved(int index, bool value)
        {
            ref var flags = ref CurrentCellsFlags[index];
            if (value)
                flags |= CellFlags.Reserved;
            else
                flags &= ~CellFlags.Reserved;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void SetCurrentCellActive(int index, bool value)
        {
            ref var flags = ref CurrentCellsFlags[index];
            if (value)
                flags |= CellFlags.Active;
            else
                flags &= ~CellFlags.Active;
        }
        public void SetCurrentCellHasMoved(int index, bool value)
        {
            ref var flags = ref CurrentCellsFlags[index];
            if (value)
                flags |= CellFlags.HasMoved;
            else
                flags &= ~CellFlags.HasMoved;
        }
        public bool IsNextCellReserved(int index) => (NextCellsFlags[index] & CellFlags.Reserved) != 0;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public bool IsNextCellActive(int index) => (NextCellsFlags[index] & CellFlags.Active) != 0;
        public bool IsNextCellHasMoved(int index) => (NextCellsFlags[index] & CellFlags.HasMoved) != 0;
        public void SetNextCellReserved(int index, bool value)
        {
            ref var flags = ref NextCellsFlags[index];
            if (value)
                flags |= CellFlags.Reserved;
            else
                flags &= ~CellFlags.Reserved;
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void SetNextCellActive(int index, bool value)
        {
            ref var flags = ref NextCellsFlags[index];
            if (value)
                flags |= CellFlags.Active;
            else
                flags &= ~CellFlags.Active;
        }
        public void SetNextCellHasMoved(int index, bool value)
        {
            ref var flags = ref NextCellsFlags[index];
            if (value)
                flags |= CellFlags.HasMoved;
            else
                flags &= ~CellFlags.HasMoved;
        }
    }
}
