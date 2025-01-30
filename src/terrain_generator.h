#ifndef TERRAINGEN_H
#define TERRAINGEN_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/tile_map_layer.hpp>
// #include <godot_cpp/classes/noise.hpp>
#include "noise.h"
#include "terrain_rng.h"
#include "biome.h"
#include <vector>

using std::vector;

// Forward declaration to avoid circular dependency in chunk.h
class Chunk;

namespace godot
{

	class TerrainGenerator : public Node
	{
		GDCLASS(TerrainGenerator, Node)

	private:
		int seed{0};
		NodePath tile_map;
		Vector2i size;
		Vector2i chunk_size;
		Noise biome_noise;
		Noise density_noise;

		int scatter_tries{5};
		double small_object_radius{3.0};
		double large_object_radius{10.0};
		int max_removed_objects; // will not cause errors if <= 0

		// Tunnel customization
		double tunnel_dist;
		double tunnel_length_max;
		double tunnel_length_min;

		vector<vector<Chunk>> chunks;

		// tiles
		int tile_source_id{-1};
		Vector2i floor_tile_organic;
		Vector2i floor_tile_hybrid;
		Vector2i floor_tile_industrial;
		Vector2i floor_tile_alien;
		Vector2i wall_tile_organic;
		Vector2i wall_tile_hybrid;
		Vector2i wall_tile_industrial;
		Vector2i wall_tile_alien;

		// Biomes enabled
		bool alien_enabled;
		bool organic_enabled;
		bool industrial_enabled;
		bool hybrid_enabled;

		TerrainRNG main_rng;

		double test_noise(Vector2i v);
		void object_scatter(double r, int k);
		vector<Vec2> point_scatter(double r, int k);
		void insert_object(vector<vector<Vec2>> &grid, Vec2 p, double cellsize, double obj_size);

	protected:
		static void _bind_methods();

	public:
		TerrainGenerator();
		~TerrainGenerator();

		void set_seed(const int given_seed);
		int get_seed() const;
		void set_biome_frequency(const double f);
		double get_biome_frequency() const;
		void set_density_frequency(const double f);
		double get_density_frequency() const;
		void set_size(const Vector2i s);
		Vector2i get_size() const;
		void set_chunk_size(const Vector2i s);
		Vector2i get_chunk_size() const;
		void set_scatter_tries(const int t);
		int get_scatter_tries() const;
		void set_small_object_radius(const double r);
		double get_small_object_radius() const;
		void set_large_object_radius(const double r);
		double get_large_object_radius() const;
		void set_max_removed_objects(const int m);
		int get_max_removed_objects() const;

		// tiles
		void set_tile_source_id(const int id);
		int get_tile_source_id() const;

		void set_floor_tile_organic(const Vector2i t);
		Vector2i get_floor_tile_organic() const;
		void set_floor_tile_hybrid(const Vector2i t);
		Vector2i get_floor_tile_hybrid() const;
		void set_floor_tile_industrial(const Vector2i t);
		Vector2i get_floor_tile_industrial() const;
		void set_floor_tile_alien(const Vector2i t);
		Vector2i get_floor_tile_alien() const;

		void set_wall_tile_organic(const Vector2i t);
		Vector2i get_wall_tile_organic() const;
		void set_wall_tile_hybrid(const Vector2i t);
		Vector2i get_wall_tile_hybrid() const;
		void set_wall_tile_industrial(const Vector2i t);
		Vector2i get_wall_tile_industrial() const;
		void set_wall_tile_alien(const Vector2i t);
		Vector2i get_wall_tile_alien() const;

		void set_alien_enabled(const bool e);
		bool get_alien_enabled() const;
		void set_organic_enabled(const bool e);
		bool get_organic_enabled() const;
		void set_industrial_enabled(const bool e);
		bool get_industrial_enabled() const;
		void set_hybrid_enabled(const bool e);
		bool get_hybrid_enabled() const;

		void set_tile_map(const NodePath &new_map);
		NodePath get_tile_map() const;

		void set_tunnel_dist(const double d);
		double get_tunnel_dist() const;
		void set_tunnel_length_max(const double l);
		double get_tunnel_length_max() const;
		void set_tunnel_length_min(const double l);
		double get_tunnel_length_min() const;

		TerrainRNG get_main_rng() const;

		void generate();
	};
}

#endif