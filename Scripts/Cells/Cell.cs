using Godot;

namespace VoxelParticleSimulator.Scripts.Cells
{
    public struct Cell
    {
        public CellType Type;
        public Color Color;
        public bool Reserved;

        public bool IsAir => Type == CellType.Air;
    }
}
