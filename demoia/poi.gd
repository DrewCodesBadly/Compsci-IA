@tool

extends Node2D

func set_collision_debug_color(c: Color):
	$CollisionShape2D.debug_color = c
