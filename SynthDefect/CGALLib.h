#pragma once

#include <CGAL/Surface_mesh.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>
#include <CGAL/exceptions.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Inexact_kernel;
typedef CGAL::Exact_predicates_exact_constructions_kernel Exact_kernel;
typedef CGAL::Polyhedron_3<Exact_kernel> Polyhedron;
typedef CGAL::Surface_mesh<Exact_kernel::Point_3> Surface_mesh;
typedef CGAL::Nef_polyhedron_3<Exact_kernel> Nef_polyhedron;
typedef Polyhedron::HalfedgeDS HalfedgeDS;

// BuildPolyhedron modifier class creates a new polyhedron in the halfedge data structure using the incremental builder
template<class HDS>
class BuildPolyhedron : public CGAL::Modifier_base<HDS>
{
public:
	BuildPolyhedron(std::vector<Vertex> const& vs, std::vector<aiFace> const& fs) : m_vertices(vs), m_faces(fs) {};
	void operator() (HDS& hds)
	{
		// Postcondition: hds is a valid polyhedral surface.
		CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
		// Save the data to HDS
		B.begin_surface(m_vertices.size(), m_faces.size(), size_t((m_vertices.size() + m_faces.size() - 2) * 2.1));

		typedef typename HDS::Vertex Vertex;
		typedef typename Vertex::Point Point;
		for (size_t i = 0; i < m_vertices.size(); ++i)
			B.add_vertex(Point(m_vertices[i].Position.x, m_vertices[i].Position.y, m_vertices[i].Position.z));

		B.begin_facet();

		for (size_t i = 0; i < m_vertices.size(); ++i)
			B.add_vertex_to_facet(i);

		B.end_facet();
		B.end_surface();
	}

private:
	std::vector<Vertex> m_vertices;
	std::vector<aiFace> m_faces;
};


