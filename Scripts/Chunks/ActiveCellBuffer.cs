using Godot;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace VoxelParticleSimulator.Scripts.Chunks
{
    public class ActiveCellsBuffer
    {
        private int[] _indices;
        private bool[] _cellMap;
        private int _count;
        private int _capacity;
        private readonly int _size;

        public ActiveCellsBuffer(int size, int initialCapacity = 1024)
        {
            _size = size;
            _indices = new int[initialCapacity];
            _cellMap = new bool[size * size * size];
            _capacity = initialCapacity;
            _count = 0;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Add(int index)
        {
            if (_cellMap[index])
                return;

            _cellMap[index] = true;


            if (_count >= _capacity)
            {
                _capacity *= 2;
                Array.Resize(ref _indices, _capacity);
            }

            _indices[_count++] = index;
        }

        public void Clear()
        {
            _count = 0;
            Array.Clear(_cellMap, 0, _cellMap.Length);
        }

        public Span<int> GetActiveIndices()
        {
            return new Span<int>(_indices, 0, _count);
        }

        public int Count => _count;

        public void Swap(ActiveCellsBuffer other)
        {
            (_indices, other._indices) = (other._indices, _indices);
            (_cellMap, other._cellMap) = (other._cellMap, _cellMap);
            (_count, other._count) = (other._count, _count);
            (_capacity, other._capacity) = (other._capacity, _capacity);
        }
    }
}
