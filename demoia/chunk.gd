@tool

extends Node2D

@export var floor_sprite: Sprite2D
@export var u_wall: Line2D
@export var l_wall: Line2D
@export var d_wall: Line2D
@export var r_wall: Line2D
@export var thing: bool:
	set(v):
		floor_sprite.self_modulate = Color(0.25, 0.25, 0.25)
		floor_sprite.queue_redraw()

# ADD
func init_from_chunk(d: ChunkData) -> void:
	if d.out_of_bounds:
		queue_free()
		return
	
	for dir in d.wall_sides:
		if dir.x > 0:
			r_wall.visible = true
		elif dir.x < 0:
			l_wall.visible = true
		elif dir.y > 0:
			d_wall.visible = true
		else:
			u_wall.visible = true
	
	#print(d.biome)
	match d.biome:
		0: #Biome::ALIEN
			floor_sprite.self_modulate = Color(1, 1, 1)
			floor_sprite.queue_redraw()
		1: #Biome::HYBRID
			floor_sprite.self_modulate = Color(0.75, 0.75, 0.75)
			floor_sprite.queue_redraw()
		2: #Biome::INDUSTRIAL
			floor_sprite.self_modulate = Color(0.5, 0.5, 0.5)
			floor_sprite.queue_redraw()
		3: #Biome::ORGANIC
			floor_sprite.self_modulate = Color(0.25, 0.25, 0.25)
			floor_sprite.queue_redraw()
		_:
			push_warning("unknown biome index")
