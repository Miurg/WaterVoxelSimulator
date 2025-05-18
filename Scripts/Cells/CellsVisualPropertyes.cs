using System.Collections.Generic;
using Godot;
using VoxelParticleSimulator.Scripts.Cells.Behavior;

namespace VoxelParticleSimulator.Scripts.Cells
{
    internal class CellsVisualPropertyes
    {
        public static Color GetColorForCellType(CellType cellType)
        {
            switch (cellType)
            {
                case CellType.Water:
                    return new Color(0.1f, 0.3f, 0.8f); 
                case CellType.Sand:
                    return new Color(0.9f, 0.8f, 0.4f); 
                default:
                    return new Color(0, 0, 0);
            }
        }
    }
}
