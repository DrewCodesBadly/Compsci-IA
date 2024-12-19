#ifndef TERRAINGEN_H
#define TERRAINGEN_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/tile_map_layer.hpp>
#include "noise.h"

namespace godot
{

	class TerrainGenerator : public Node
	{
		GDCLASS(TerrainGenerator, Node)

	private:
		int seed;
		NodePath tile_map;
		Vector2i size;
		Vector2i chunk_size;
		Noise biome_noise;

		// tiles
		int tile_source_id;
		Vector2i floor_tile;
		Vector2i wall_tile_organic;
		Vector2i wall_tile_hybrid;
		Vector2i wall_tile_industrial;
		Vector2i wall_tile_alien;

	protected:
		static void _bind_methods();

	public:
		TerrainGenerator(int given_seed = 0, double biome_frequency = 1.0, int source_id = -1);
		~TerrainGenerator();

		void set_seed(const int given_seed);
		int get_seed() const;
		void set_biome_frequency(const double f);
		double get_biome_frequency() const;
		void set_size(const Vector2i s);
		Vector2i get_size() const;
		void set_chunk_size(const Vector2i s);
		Vector2i get_chunk_size() const;

		// tiles
		void set_tile_source_id(const int id);
		int get_tile_source_id() const;
		void set_floor_tile(const Vector2i t);
		Vector2i get_floor_tile() const;
		void set_wall_tile_organic(const Vector2i t);
		Vector2i get_wall_tile_organic() const;
		void set_wall_tile_hybrid(const Vector2i t);
		Vector2i get_wall_tile_hybrid() const;
		void set_wall_tile_industrial(const Vector2i t);
		Vector2i get_wall_tile_industrial() const;
		void set_wall_tile_alien(const Vector2i t);
		Vector2i get_wall_tile_alien() const;

		void set_tile_map(const NodePath &new_map);
		NodePath get_tile_map() const;
		double test_noise(Vector2i v);

		void generate();
	};

}

#endif