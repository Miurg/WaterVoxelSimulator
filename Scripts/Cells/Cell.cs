using Godot;

namespace VoxelParticleSimulator.Scripts.Cells
{
    public struct Cell
    {
        public CellType Type;
        public bool Reserved;

        public bool IsAir => Type == CellType.Air;
    }
}
