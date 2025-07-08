extends Camera3D

@export var speed: float = 15.0
@export var sensitivity: float = 0.003

var yaw: float = 0.0
var pitch: float = 0.0

func _ready():
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)

func _input(event):
	if event is InputEventMouseMotion:
		yaw -= event.relative.x * sensitivity
		pitch -= event.relative.y * sensitivity
		pitch = clamp(pitch, -PI/2, PI/2)
		
		rotation.y = yaw
		rotation.x = pitch

func _process(delta):
	var input_vector = Vector3.ZERO
	
	if Input.is_key_pressed(KEY_W):
		input_vector -= transform.basis.z
	if Input.is_key_pressed(KEY_S):
		input_vector += transform.basis.z
	if Input.is_key_pressed(KEY_A):
		input_vector -= transform.basis.x
	if Input.is_key_pressed(KEY_D):
		input_vector += transform.basis.x
	
	if input_vector != Vector3.ZERO:
		input_vector = input_vector.normalized()
		position += input_vector * speed * delta
	
	# ESC для выхода из режима захвата мыши
	if Input.is_action_just_pressed("ui_cancel"):
		Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
