#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Math.h"
#include "Plane.h"
#include "Scene/Convex.h"
class FrustumG {

protected:

	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
	};

public:
	enum Result:byte
	{
		OUTSIDE, INTERSECT, INSIDE
	};
	Plane pl[6];
	glm::vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	float nearD, farD, ratio, angle, tanAngle;
	glm::vec2 near,far;


	FrustumG() {}
	virtual ~FrustumG() {}

	void setCamInternals(float angle, float ratio, float nearD, float farD);
	inline void setCamInternals(const ProjData& data) {
		setCamInternals(data.persp.fov, data.persp.ratio, data.persp.near, data.persp.far);
	}
	void setCamDef(const Basis& basis_camera);

	FrustumG::Result point(const glm::vec3& p);
	FrustumG::Result sphere(const glm::vec3& p, float raio);
	FrustumG::Result box(const AABox& b);
};
class DebugFrustumG:public FrustumG {
public:

	inline void draw(RenderTarget* render) {
		init();
		mesh.draw(*render, sRender::LINES,glShader::color_layout);
		initNorm();
		mesh_norm.draw(*render, sRender::LINES, glShader::color_layout);
	}
private:
	Color c = Color::WHITE;
	inline void loop(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4) {
		mesh.push_back({ v1,c });
		mesh.push_back({ v2,c });

		mesh.push_back({ v2,c });
		mesh.push_back({ v3,c });

		mesh.push_back({ v3,c });
		mesh.push_back({ v4,c });

		mesh.push_back({ v4,c });
		mesh.push_back({ v1,c });
	}
	inline void init() {
		mesh.clear();
		//near plane
		loop(ntl, ntr, nbr, nbl);
		//far plane	
		loop(ftr, ftl, fbl, fbr);
		//bottom plane
		loop(nbl, nbr, fbr, fbl);
		//top plane
		loop(ntr, ntl, ftl, ftr);
		//left plane
		loop(ntl, nbl, fbl, ftl);
		// right plane
		loop(nbr, ntr, ftr, fbr);	
	}
	inline void initNorm() {
		glm::vec3 a, b;
		Color c = Color::RED;
		mesh_norm.clear();
		// near
		a = (ntr + ntl + nbr + nbl) * 0.25f;
		b = a + pl[NEARP].normal;
		mesh_norm.push_back({ a,c });
		mesh_norm.push_back({ b,c });
		// far
		a = (ftr + ftl + fbr + fbl) * 0.25f;
		b = a + pl[FARP].normal;
		mesh_norm.push_back({ a,c });
		mesh_norm.push_back({ b,c });
		// left
		a = (ftl + fbl + nbl + ntl) * 0.25f;
		b = a + pl[LEFT].normal;
		mesh_norm.push_back({ a,c });
		mesh_norm.push_back({ b,c });

		// right
		a = (ftr + nbr + fbr + ntr) * 0.25f;
		b = a + pl[RIGHT].normal;
		mesh_norm.push_back({ a,c });
		mesh_norm.push_back({ b,c });

		// top
		a = (ftr + ftl + ntr + ntl) * 0.25f;
		b = a + pl[TOP].normal;
		mesh_norm.push_back({ a,c });
		mesh_norm.push_back({ b,c });

		// bottom
		a = (fbr + fbl + nbr + nbl) * 0.25f;
		b = a + pl[BOTTOM].normal;
		mesh_norm.push_back({ a,c });
		mesh_norm.push_back({ b,c });
	}
	MeshShell<ConvexVertex> mesh_norm;
	MeshShell<ConvexVertex> mesh;
};

#endif
