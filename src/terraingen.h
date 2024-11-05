#ifndef TERRAINGEN_H
#define TERRAINGEN_H

#include <godot_cpp/classes/node.hpp>

namespace godot
{

	class TerrainGenerator : public Node
	{
		GDCLASS(TerrainGenerator, Node)

	private:
		int seed;

	protected:
		static void _bind_methods();

	public:
		TerrainGenerator(int given_seed = 0);
		~TerrainGenerator();

		void set_seed(const int given_seed);
		int get_seed() const;
	};

}

#endif