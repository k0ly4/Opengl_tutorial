#include "Frustum.h"
#define ANG2RAD 3.14159265358f/180.f

void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD) {

	// store the information
	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	// compute width and height of the near and far plane sections
	tanAngle = (float)tan(ANG2RAD * angle * 0.5f);
	near.y = nearD * tanAngle;
	near.x = near.y * ratio;
	far.y = farD * tanAngle;
	far.x = far.y * ratio;
}
void FrustumG::setCamDef(const Basis& basis) {

	glm::vec3 nc, fc, X, Y, Z;

	Z = -basis.front;

	X = basis.right;

	Y = basis.up;

	nc = basis.position - Z * nearD;
	fc = basis.position - Z * farD;

	ntl = nc + Y * near.y - X * near.x;
	ntr = nc + Y * near.y + X * near.x;
	nbl = nc - Y * near.y - X * near.x;
	nbr = nc - Y * near.y + X * near.x;

	ftl = fc + Y * far.y - X * far.x;
	ftr = fc + Y * far.y + X * far.x;
	fbl = fc - Y * far.y - X * far.x;
	fbr = fc - Y * far.y + X * far.x;

	pl[TOP].	set(ntr, ntl, ftl);
	pl[BOTTOM].	set(nbl, nbr, fbr);
	pl[LEFT].	set(ntl, nbl, fbl);
	pl[RIGHT].	set(nbr, ntr, fbr);
	pl[NEARP].	set(ntl, ntr, nbr);
	pl[FARP].	set(ftr, ftl, fbl);
}

FrustumG::Result FrustumG::point(const glm::vec3& p) {
	Result result = INSIDE;
	for (size_t  i = 0; i < 6; i++) 
		if (pl[i].distance(p) < 0) return OUTSIDE;
	return(result);
}

FrustumG::Result FrustumG::sphere(const glm::vec3& p, float radius) {

	Result result = INSIDE;
	float distance;

	for (size_t i = 0; i < 6; i++) {
		distance = pl[i].distance(p);
		if (distance < -radius) return OUTSIDE;
		else if (distance < radius) result = INTERSECT;
	}
	return result;

}

FrustumG::Result FrustumG::box(const AABox& b) {

	FrustumG::Result result = INSIDE;
	for (size_t i = 0; i < 6; i++) {

		if (pl[i].distance(b.getVertexP(pl[i].normal)) < 0) return OUTSIDE;
		else if (pl[i].distance(b.getVertexN(pl[i].normal)) < 0) result = INTERSECT;
	}
	return(result);

}