using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    public static class CellBehaviorRegistry
    {
        private static readonly BaseBehaviorCell[] BehaviorArray;
        static CellBehaviorRegistry()
        {
            byte maxCellType = Enum.GetValues<CellType>().Cast<byte>().Max();
            BehaviorArray = new BaseBehaviorCell[maxCellType + 1];

            BehaviorArray[(int)CellType.Air] = new AirBehaviorCell();
            BehaviorArray[(int)CellType.Water] = new WaterBehaviorCell();
            BehaviorArray[(int)CellType.Sand] = new SandBehaviorCell();
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static BaseBehaviorCell GetBehavior(CellType type)
        {
            return BehaviorArray[(int)type];
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsActive(CellType type)
        {
            return type != CellType.Air;
        }
    }
}
