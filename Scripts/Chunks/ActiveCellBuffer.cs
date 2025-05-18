using Godot;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace VoxelParticleSimulator.Scripts.Chunks
{
    public class ActiveCellsBuffer
    {
        private Vector3I[] _positions;
        private bool[] _cellMap;
        private int _count;
        private int _capacity;
        private int _size;

        public ActiveCellsBuffer(int size, int initialCapacity = 1024)
        {
            _size = size;
            _positions = new Vector3I[initialCapacity];
            _cellMap = new bool[size * size * size];
            _capacity = initialCapacity;
            _count = 0;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private int ToIndex(Vector3I pos) => pos.X + pos.Y * _size + pos.Z * _size * _size;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Add(Vector3I pos)
        {
            int index = ToIndex(pos);

            if (_cellMap[index])
                return;

            _cellMap[index] = true;

            if (_count >= _capacity)
            {
                _capacity *= 2;
                Array.Resize(ref _positions, _capacity);
            }

            _positions[_count++] = pos;
        }

        public void Clear()
        {
            _count = 0;
            Array.Clear(_cellMap, 0, _cellMap.Length);
        }

        public Span<Vector3I> GetActivePositions()
        {
            return new Span<Vector3I>(_positions, 0, _count);
        }

        public int Count => _count;

        public void Swap(ActiveCellsBuffer other)
        {
            (_positions, other._positions) = (other._positions, _positions);
            (_cellMap, other._cellMap) = (other._cellMap, _cellMap);
            (_count, other._count) = (other._count, _count);
            (_capacity, other._capacity) = (other._capacity, _capacity);
        }
    }
}
