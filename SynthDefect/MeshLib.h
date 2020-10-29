#pragma once

#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Modifier_base.h>
#include <CGAL/exceptions.h>
#include <string>
#include <fstream>
#include <exception>

namespace MeshLib {
	namespace IO {

		// The Converter class builds a CGAL::Polyhedron_3 from any format of mesh file.
		// Now, only Wavefron OBJ file is supported 
		template<typename T>
		class Converter : CGAL::Modifier_base<T>
		{
			// Constructor
		public:
			Converter(std::string const& filePath) : m_path(filePath) {};
		};
	}
}
