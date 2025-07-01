extends Node

@onready var FpsLabel = $FpsLabel
@onready var PhysicProcessTimeLabel = $PhysicProcessTimeLabel
@onready var WorldSimulation = $World

func _process(delta: float) -> void:
	FpsLabel.text = "Fps: " + String.num_int64(Engine.get_frames_per_second())
func _physics_process(delta: float) -> void:
	PhysicProcessTimeLabel.text = "Physic iteration: " + str(WorldSimulation.GetPhysicIteration()) + "ms"
