using Godot;
using System;

namespace VoxelParticleSimulator.Scripts.Cells
{
    public struct Cell
    {
        public CellType Type;
        public CellFlags Flags;

        public Cell (CellType type, CellFlags flags)
        {
            this.Type = type;
            this.Flags = flags;
        }
    }
    public struct CellBuffer
    {
        public CellType[] Type;
        public CellFlags[] Flags;

        public CellBuffer(int size)
        {
            Type = new CellType[size];
            Flags = new CellFlags[size];
        }

        public bool IsAirAt(int index) => Type[index] == CellType.Air;

        public bool IsReserved(int index) => (Flags[index] & CellFlags.Reserved) != 0;
        public bool IsActive(int index) => (Flags[index] & CellFlags.Active) != 0;
        public bool IsAlreadyMove(int index) => (Flags[index] & CellFlags.HasMoved) != 0;

        public void SetReserved(int index, bool value)
        {
            if (value)
                Flags[index] |= CellFlags.Reserved;
            else
                Flags[index] &= ~CellFlags.Reserved;
        }
        public void SetAlreadyMove(int index, bool value)
        {
            if (value)
                Flags[index] |= CellFlags.HasMoved;
            else
                Flags[index] &= ~CellFlags.HasMoved;
        }

        public void SetActive(int index, bool value)
        {
            if (value)
                Flags[index] |= CellFlags.Active;
            else
                Flags[index] &= ~CellFlags.Active;
        }

        public void CopyTo(CellBuffer target)
        {
            Array.Copy(Type, target.Type, Type.Length);
            Array.Copy(Flags, target.Flags, Flags.Length);
        }
    }

}
