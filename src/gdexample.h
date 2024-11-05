#ifndef TERRAINGEN_H
#define TERRAINGEN_H

#include <godot_cpp/classes/sprite2d.hpp>

namespace godot
{

	class TerrainGenerator : public Node
	{
		GDCLASS(TerrainGenerator, Node)

	private:
		double time_passed;

	protected:
		static void _bind_methods();

	public:
		TerrainGenerator();
		~TerrainGenerator();
	};

}

#endif