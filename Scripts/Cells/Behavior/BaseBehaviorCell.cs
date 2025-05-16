using Godot;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    public abstract class BaseBehaviorCell
    {
        public abstract void Simulate(Chunk chunk, Vector3I pos);
    }
}
