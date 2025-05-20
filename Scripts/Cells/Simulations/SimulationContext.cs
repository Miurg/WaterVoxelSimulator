using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    public readonly ref struct SimulationContext
    {
        public readonly Span<CellType> CurrentTypes;
        public readonly Span<CellFlags> CurrentFlags;
        public readonly Span<CellType> NextTypes;
        public readonly Span<CellFlags> NextFlags;
        public SimulationContext(Span<CellType> currentTypes, Span<CellFlags> currentFlags, Span<CellType> nextTypes, Span<CellFlags> nextFlags)
        {
            CurrentTypes = currentTypes;
            CurrentFlags = currentFlags;
            NextTypes = nextTypes;
            NextFlags = nextFlags;
        }
        public bool IsCurrentReserved(int index) => (CurrentFlags[index] & CellFlags.Reserved) != 0;
        public bool IsCurrentActive(int index) => (CurrentFlags[index] & CellFlags.Active) != 0;

        public void SetCurrentReserved(int index, bool value)
        {
            if (value)
                CurrentFlags[index] |= CellFlags.Reserved;
            else
                CurrentFlags[index] &= ~CellFlags.Reserved;
        }

        public void SetCurrentActive(int index, bool value)
        {
            if (value)
                CurrentFlags[index] |= CellFlags.Active;
            else
                CurrentFlags[index] &= ~CellFlags.Active;
        }
        public bool IsNextReserved(int index) => (NextFlags[index] & CellFlags.Reserved) != 0;
        public bool IsNextActive(int index) => (NextFlags[index] & CellFlags.Active) != 0;

        public void SetNextReserved(int index, bool value)
        {
            if (value)
                NextFlags[index] |= CellFlags.Reserved;
            else
                NextFlags[index] &= ~CellFlags.Reserved;
        }

        public void SetNextActive(int index, bool value)
        {
            if (value)
                NextFlags[index] |= CellFlags.Active;
            else
                NextFlags[index] &= ~CellFlags.Active;
        }
    }
}
