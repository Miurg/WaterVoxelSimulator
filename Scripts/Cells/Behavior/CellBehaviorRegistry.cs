using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    public static class CellBehaviorRegistry
    {
        private static readonly HashSet<CellType> activeTypes = new()
        {
            CellType.Sand,
            CellType.Water
        };

        public static Dictionary<CellType, BaseBehaviorCell> Behaviors = new()
        {
            { CellType.Air, new AirBehaviorCell() },
            { CellType.Sand, new SandBehaviorCell() },
            { CellType.Water, new WaterBehaviorCell() }
        };

        public static bool IsActive(CellType type)
        {
            return activeTypes.Contains(type);
        }
    }
}
