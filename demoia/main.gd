@tool
extends Node

@export var generator: TerrainGenerator
@export var poi_1_container: Node2D
@export var poi_2_container: Node2D
@export var chunk_container: Node2D
@export var gen_toggle: bool = false:
	set(v):
		generate()

@onready var poi_scene := preload("res://poi.tscn")
@onready var chunk_scene := preload("res://chunk.tscn")

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#generator.generate()
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass

func generate() -> void:
	for child in poi_1_container.get_children():
		child.queue_free()
	for child in poi_2_container.get_children():
		child.queue_free()
	for child in chunk_container.get_children():
		child.queue_free()
	
	generator.generate()
	
	for poi in generator.get_first_scatter_pass_points():
		var p := poi_scene.instantiate()
		p.set_collision_debug_color(Color.RED)
		poi_1_container.add_child(p)
		p.global_position = poi
	
	for poi in generator.get_second_scatter_pass_points():
		var p := poi_scene.instantiate()
		p.set_collision_debug_color(Color.BLUE)
		poi_2_container.add_child(p)
		p.global_position = poi
	
	var chunks := generator.get_chunks()
	for x in chunks.size():
		for y in chunks[0].size():
			var data: ChunkData = chunks[x][y]
			var chunk := chunk_scene.instantiate()
			chunk_container.add_child(chunk)
			chunk.position = Vector2i(x, y) * generator.chunk_size + Vector2i(8, 8)
			chunk.init_from_chunk(data)
