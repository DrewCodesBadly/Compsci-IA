extends Camera2D

@export var speed: float

# Basic movement/zooming
func _process(delta: float) -> void:
	var dir := Input.get_vector("left", "right", "up", "down")
	position += dir * speed * delta
	
	if Input.is_action_just_pressed("zoom_out"):
		zoom -= Vector2(0.05, 0.05)
	elif Input.is_action_just_pressed("zoom_in"):
		zoom += Vector2(0.05, 0.05)
