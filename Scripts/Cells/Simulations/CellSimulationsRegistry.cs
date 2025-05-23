using Godot;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.X86;
using System.Text;
using System.Threading.Tasks;
using VoxelParticleSimulator.Scripts.Cells.Simulations.GeneralMoving;
using VoxelParticleSimulator.Scripts.Cells.Simulations.GeneralMoving.Liquid;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    public delegate void CellSimulateDelegate(ref SimulationContext context);

    
    public static class CellSimulationsRegistry
    {
        const int VecSize = 32;
        private static readonly CellSimulateDelegate[] _simulateArray;
        private static readonly bool[] _isActiveArray;
        static CellSimulationsRegistry()
        {
            byte maxCellType = Enum.GetValues<CellType>().Cast<byte>().Max();
            _simulateArray = new CellSimulateDelegate[maxCellType + 1];
            _isActiveArray = new bool[maxCellType + 1];

            _simulateArray[(byte)CellType.Air] = SimulateAir;
            _simulateArray[(byte)CellType.Water] = SimulateWater;
            _simulateArray[(byte)CellType.Sand] = SimulateSand;

            _isActiveArray[(byte)CellType.Air] = false;
            _isActiveArray[(byte)CellType.Water] = true;
            _isActiveArray[(byte)CellType.Sand] = true;

        }

        public static void Simulate(CellType type,ref SimulationContext context)
        {
            _simulateArray[(byte)type]( ref context);
        }

        public static bool IsActive(CellType type)
        {
            return _isActiveArray[(byte)type];
        }

        private static void SimulateAir(ref SimulationContext ctx) { AirSimulationCell.Simulate(ref ctx); }
        private unsafe static void SimulateWater(ref SimulationContext ctx)
        {
            //Span<byte> currentCellsTypesBytes = MemoryMarshal.AsBytes(ctx.CurrentCellsTypes);
            //Span<byte> currentCellsFlagsBytes = MemoryMarshal.AsBytes(ctx.CurrentCellsFlags);
            //Span<byte> nextCellsTypesBytes = MemoryMarshal.AsBytes(ctx.NextCellsTypes);
            //Span<byte> nextCellsFlagsBytes = MemoryMarshal.AsBytes(ctx.NextCellsFlags);

            //Span<int> currentIndices = ctx.CurrentIndicies; 
            //Span<byte> currentIndicesBytes = MemoryMarshal.AsBytes(currentIndices);

            //Span<int> nextIndices = ctx.NextIndicies;
            //Span<byte> nextIndicesBytes = MemoryMarshal.AsBytes(nextIndices);

            //for (int i = 0; i < ctx.CurrentIndicies.Length; i += 32)
            //{
            //    Span<byte> currentFlagByte = stackalloc byte[32];
            //    Span<byte> currentTypeByte = stackalloc byte[32];
            //    Span<byte> belowCellFlagByte = stackalloc byte[32];
            //    Span<byte> belowCellTypeByte = stackalloc byte[32];
            //    Span<byte> currentIndicesByte = stackalloc byte[32];
            //    Span<byte> nextIndicesByte = stackalloc byte[32];
            //    for (int j = 0; j < VecSize; j++)
            //    {
            //        currentFlagByte[j] = currentCellsFlagsBytes[ctx.CurrentIndicies[i+j]];
            //        currentTypeByte[j] = currentCellsTypesBytes[ctx.CurrentIndicies[i + j]];
            //        belowCellFlagByte[j] = currentCellsFlagsBytes[ctx.CurrentIndicies[i + j] - SimulatorConst.ChunkSize];
            //        belowCellTypeByte[j] = currentCellsTypesBytes[ctx.CurrentIndicies[i + j] - SimulatorConst.ChunkSize];
            //        currentIndicesByte[j] = currentIndicesBytes[i + j];
            //        nextIndicesByte[j] = nextIndicesBytes[i + j];
            //    }
            //    fixed (byte* ptr = currentFlagByte,
            //         ptr2 = currentTypeByte,
            //         ptr3 = belowCellFlagByte,
            //         ptr4 = belowCellTypeByte,
            //         ptr5 = currentIndicesByte,
            //         ptr6 = nextIndicesByte)
            //    {
            //        var currentFlag = Avx.LoadVector256(ptr);
            //        var currentType = Avx.LoadVector256(ptr2);
            //        var belowType = Avx.LoadVector256(ptr4);
            //        var belowFlag = Avx.LoadVector256(ptr3);
            //        var currentInd = Avx.LoadVector256(ptr5);
            //        var nextInd = Avx.LoadVector256(ptr6);

            //        MovingCellsDownSIMD.Simulate(currentInd, nextInd, currentType, currentFlag, belowType, belowFlag);
            //    }

            //}
            for (ushort index = 0; index < ctx.CurrentIndicies.Length; index++)
            {
                MovingCellsDownMove.Simulate(index, ref ctx);
            }
            for (ushort index = 0; index < ctx.CurrentIndicies.Length; index++)
            {
                LiquidMove.Simulate(index, ref ctx);

                if (!ctx.IsCurrentCellActive(ctx.CurrentIndicies[index]))
                {
                    ctx.NextCellsTypes[ctx.CurrentIndicies[index]] = ctx.CurrentCellsTypes[ctx.CurrentIndicies[index]];
                    ctx.NextCellsFlags[ctx.CurrentIndicies[index]] |= ctx.CurrentCellsFlags[ctx.CurrentIndicies[index]];
                    ctx.NextIndicies[index] = ctx.CurrentIndicies[index];
                }
                else if (!ctx.IsCurrentCellHasMoved(ctx.CurrentIndicies[index]))
                {
                    ctx.SetCurrentCellActive(ctx.CurrentIndicies[index], false);
                    ctx.NextCellsTypes[ctx.CurrentIndicies[index]] = ctx.CurrentCellsTypes[ctx.CurrentIndicies[index]];
                    ctx.NextCellsFlags[ctx.CurrentIndicies[index]] |= ctx.CurrentCellsFlags[ctx.CurrentIndicies[index]];
                    ctx.NextIndicies[index] = ctx.CurrentIndicies[index];
                }
            }


            //for (int index = 0; index < SimulatorConst.ChunkSize3; index++)
            //{
            //    if (ctx.CurrentCellsTypes[index] == CellType.Air) continue;
            //    WaterSimulationCell.Simulate(index, ref ctx);
            //}

        }
        private static SandSimulationCell _sandBehavior = new SandSimulationCell();
        private static void SimulateSand(ref SimulationContext ctx) { /* песок */ }
    }
}
