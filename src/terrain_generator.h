#ifndef TERRAINGEN_H
#define TERRAINGEN_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/tile_map_layer.hpp>
// #include <godot_cpp/classes/noise.hpp>
#include "noise.h"
#include "terrain_rng.h"
#include "biome.h"
#include <vector>
// #include "terrain_chunk.h"

using std::vector;

namespace godot
{

	// Forward declaration to avoid circular dependency in chunk.h
	class TerrainChunk;

	class TerrainGenerator : public Node
	{
		GDCLASS(TerrainGenerator, Node)

	private:
		int seed{0};
		Vector2i size;
		Vector2i chunk_size;
		Noise biome_noise;
		Noise density_noise;

		int scatter_tries{30};
		double small_object_radius{3.0};
		double large_object_radius{10.0};
		int max_removed_objects; // will not cause errors if <= 0

		// Room/tunnel customization
		double room_dist;
		Vector2i room_size_max;
		Vector2i room_size_min;
		int min_tunnels{1};
		int max_tunnels{2};

		vector<vector<TerrainChunk>> chunks;

		// Biomes enabled
		bool alien_enabled;
		bool organic_enabled;
		bool industrial_enabled;
		bool hybrid_enabled;

		TerrainRNG main_rng;

		// Data output for godot usage
		Array first_scatter_pass_points;
		Array second_scatter_pass_points;
		Array chunks_gd;

		double test_noise(Vector2i v);
		void object_scatter(double r, int k, int pass_idx);
		vector<vector<Vec2>> point_scatter(double r, int k);
		void insert_object(vector<vector<Vec2>> &grid, Vec2 p, double cellsize, double obj_size, int pass_idx);

	protected:
		static void _bind_methods();

	public:
		TerrainGenerator();
		~TerrainGenerator();

		void generate();
		Array get_first_scatter_pass_points();
		Array get_second_scatter_pass_points();
		Array get_chunks();
		Vector2i world_to_nearest_chunk_coords(Vector2 world);
		Vector2 chunk_to_world_coords(Vector2i chunk_coords);

		// getters/setters...
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

		void set_room_dist(const double d);
		double get_room_dist() const;
		void set_room_size_max(const Vector2i s);
		Vector2i get_room_size_max() const;
		void set_room_size_min(const Vector2i s);
		Vector2i get_room_size_min() const;

		TerrainRNG get_main_rng() const;

		void set_min_tunnels(const int t);
		void set_max_tunnels(const int t);
		int get_min_tunnels() const;
		int get_max_tunnels() const;
	};
}

#endif