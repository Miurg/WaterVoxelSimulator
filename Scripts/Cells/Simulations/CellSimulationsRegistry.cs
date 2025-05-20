using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    public delegate void CellSimulateDelegate(int index, ref SimulationContext context);

    public static class CellSimulationsRegistry
    {
        private static readonly CellSimulateDelegate[] _simulateArray;
        private static readonly bool[] _isActiveArray;

        static CellSimulationsRegistry()
        {
            byte maxCellType = Enum.GetValues<CellType>().Cast<byte>().Max();
            _simulateArray = new CellSimulateDelegate[maxCellType + 1];
            _isActiveArray = new bool[maxCellType + 1];

            _simulateArray[(int)CellType.Air] = SimulateAir;
            _simulateArray[(int)CellType.Water] = SimulateWater;
            _simulateArray[(int)CellType.Sand] = SimulateSand;

            _isActiveArray[(int)CellType.Air] = false;
            _isActiveArray[(int)CellType.Water] = true;
            _isActiveArray[(int)CellType.Sand] = true;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void Simulate(CellType type, int index, ref SimulationContext context)
        {
            _simulateArray[(int)type](index, ref context);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsActive(CellType type)
        {
            return _isActiveArray[(int)type];
        }

        // Примеры логик
        private static AirSimulationCell _airBehavior = new AirSimulationCell();
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static void SimulateAir(int i, ref SimulationContext ctx) { _airBehavior.Simulate(i, ref ctx); }
        private static WaterSimulationCell _waterBehavior = new WaterSimulationCell();
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static void SimulateWater(int i, ref SimulationContext ctx) { _waterBehavior.Simulate(i, ref ctx); }
        private static SandSimulationCell _sandBehavior = new SandSimulationCell();
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static void SimulateSand(int i, ref SimulationContext ctx) { /* песок */ }
    }
}
