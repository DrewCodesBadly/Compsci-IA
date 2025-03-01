extends Node

@export var generator: TerrainGenerator
@export var poi_1_container: Node2D
@export var poi_2_container: Node2D
@export var chunk_container: Node2D

@onready var poi_scene := preload("res://poi.tscn")
@onready var chunk_scene := preload("res://chunk.tscn")

# Input showing/hiding ui
func _process(_delta: float) -> void:
	if Input.is_action_just_pressed("hide_vars"):
		$CanvasLayer/UI.visible = !$CanvasLayer/UI.visible

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
	
	TerrainGenerator

func _on_generate_btn_pressed() -> void:
	generate()

func _on_seed_box_value_changed(value: float) -> void:
	generator.seed = floori(value)

func _on_size_x_value_changed(value: float) -> void:
	generator.size.x = floori(value)

func _on_size_y_value_changed(value: float) -> void:
	generator.size.y = floori(value)

func _on_biome_freq_value_changed(value: float) -> void:
	generator.biome_frequency = value

func _on_density_freq_value_changed(value: float) -> void:
	generator.density_frequency = value

func _on_scatter_tries_value_changed(value: float) -> void:
	generator.scatter_tries = floori(value)

func _on_small_radius_value_changed(value: float) -> void:
	generator.small_object_radius = floori(value)

func _on_large_radius_value_changed(value: float) -> void:
	generator.large_object_radius = floori(value)

func _on_max_removed_obj_value_changed(value: float) -> void:
	generator.max_removed_objects = floori(value)

func _on_room_size_max_value_changed(value: float) -> void:
	generator.room_size_max = Vector2i(floori(value), floori(value))

func _on_room_size_min_value_changed(value: float) -> void:
	generator.room_size_min = Vector2i(floori(value), floori(value))

func _on_room_dist_value_changed(value: float) -> void:
	generator.room_distance = floori(value)
