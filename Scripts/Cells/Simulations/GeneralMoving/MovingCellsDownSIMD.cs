using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.X86;
using System.Text;
using System.Threading.Tasks;
using VoxelParticleSimulator.Scripts.Cells.Behavior;

namespace VoxelParticleSimulator.Scripts.Cells.Simulations.GeneralMoving
{
    internal class MovingCellsDownSIMD
    {
        public static void Simulate(Span<byte> nextTypes, Span<byte> nextFlags, Span<int> nextIndices, int globalOffset)
        {
        }
        public static void Simulate(Vector256<byte> currentInd, Vector256<byte> nextIndex, Vector256<byte> currentType, Vector256<byte> currentFlag,
            Vector256<byte> belowType, Vector256<byte> belowFlag,
            Span<byte> nextTypes, Span<byte> nextFlags, Span<int> nextIndices, int globalOffset)
        {
            //    // Константы
            //    const byte TYPE_AIR = (byte)CellType.Air;
            //    const byte FLAG_ACTIVE = (byte)CellFlags.Active;
            //    const byte FLAG_RESERVED = (byte)CellFlags.Reserved;

            //    var zero = Vector256<byte>.Zero;
            //    var full = Vector256.Create((byte)0xFF);

            //    var isActive = Avx2.And(currentFlag, Vector256.Create(FLAG_ACTIVE));
            //    var maskInactive = Avx2.CompareEqual(isActive, zero);

            //    var isBelowReserved = Avx2.And(belowFlag, Vector256.Create(FLAG_RESERVED));
            //    var neq = Avx2.CompareEqual(isBelowReserved, zero);
            //    var maskBelowReserved = Avx2.Xor(neq, Vector256.Create((byte)0xFF));

            //    var isBelowAir = Avx2.CompareEqual(belowType, Vector256.Create(TYPE_AIR));
            //    var maskBelowNotAir = Avx2.Xor(isBelowAir, full);

            //    var nextInd = nextIndex.AsInt16();
            //    var lowerBound = Avx2.CompareGreaterThan(nextInd, Vector256<short>.Zero);
            //    var lowerBoundByte = lowerBound.AsByte();
            //    var low128 = nextInd.GetLower();
            //    var low256Int = Avx2.ConvertToVector256Int32(low128);
            //    var chunkSizeVector = Vector256.Create(SimulatorConst.ChunkSize3);
            //    var upperBound = Avx2.CompareGreaterThan(chunkSizeVector, low256Int).AsByte();

            //    var isInBounds = Avx2.AndNot(Avx2.Xor(lowerBoundByte, full), upperBound).AsByte(); 
            //    var maskInvalidIndex = Avx2.Xor(isInBounds, full);

            //    // Финальная маска запрета
            //    var finalMask = Avx2.Or(maskInactive,
            //                     Avx2.Or(maskBelowReserved,
            //                     Avx2.Or(maskBelowNotAir,
            //                                  maskInvalidIndex)));

            //    // Инвертируем: где МОЖНО двигаться (0xFF -> нельзя, 0x00 -> можно)
            //    var canMoveMask = Vector256.Xor(finalMask, full);

            //    // --- Применяем движения ---
            //    // Нам нужно пройтись по каждой клетке в этом векторе и если canMoveMask[i] == 0xFF,
            //    // сделать swap на уровне массивов

            //    byte[] maskBytes = new byte[32];
            //    canMoveMask.CopyTo(maskBytes);

            //    byte[] typeBytes = new byte[32];
            //    currentType.CopyTo(typeBytes);

            //    byte[] flagBytes = new byte[32];
            //    currentFlag.CopyTo(flagBytes);

            //    int[] currentIndices = new int[32];
            //    currentInd.CopyTo(MemoryMarshal.Cast<int, byte>(currentIndices));

            //    int[] nextIndicesLocal = new int[32];
            //    nextIndex.CopyTo(MemoryMarshal.Cast<int, byte>(nextIndicesLocal));

            //    for (int i = 0; i < 32; i++)
            //    {
            //        if (maskBytes[i] == 0xFF) // можно двигаться
            //        {
            //            int from = currentIndices[i];
            //            int to = nextIndicesLocal[i];

            //            // Swap: текущее значение идёт вниз
            //            nextTypes[to] = typeBytes[i];
            //            nextFlags[to] = flagBytes[i];

            //            // В текущей позиции — воздух и флаги сбрасываются
            //            nextTypes[from] = (byte)CellType.Air;
            //            nextFlags[from] = (byte)CellFlags.None;

            //            // Сохраняем, куда переместились
            //            nextIndices[globalOffset + i] = to;
            //        }
            //    }
        }
    }
}
