@tool
extends Node

@export var generator: TerrainGenerator
@export var gen_toggle: bool = false:
	set(v):
		generate()

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#generator.generate()
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass

func generate() -> void:
	generator.generate()
