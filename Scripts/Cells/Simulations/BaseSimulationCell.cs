using Godot;

namespace VoxelParticleSimulator.Scripts.Cells.Behavior
{
    public abstract class BaseSimulationCell
    {
        public abstract void Simulate(int index, ref SimulationContext ctx);
    }
}
