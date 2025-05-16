using System.Security.Cryptography;
using Godot;

namespace VoxelParticleSimulator.Scripts
{
    public static class GodotShuffleExtensions
    {
        public static void Shuffle<T>(this T[] array, Godot.RandomNumberGenerator rng)
        {
            int n = array.Length;
            while (n > 1)
            {
                int k = rng.RandiRange(0, n - 1);
                n--;
                (array[n], array[k]) = (array[k], array[n]);
            }
        }
    }
}
