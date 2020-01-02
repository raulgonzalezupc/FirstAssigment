#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "MathGeoLib.h"
#include "Component.h"

const int IS_IN = 0;
const int IS_OUT = 1;
const int INTERSECT = 2;

class Camera : public Component {
public:

	Camera(GameObject* owner);
	~Camera();

	void SetFrustum();
	
	void Draw();
	void GenerateFBOTexture(unsigned w, unsigned h);


	int isCollidingFrustum(const AABB& aabb) const;


	float4x4 proj, view, model;
	float cameraSpeed = 0.05f;
	unsigned fbo = 0;
	float3 camPos;

	float Hnear;
	float Wnear;

	float Hfar;
	float Wfar;

	float nearDist;
	float farDist;

	float3 nCenter;
	float3 fCenter;

	float3 ntl;//near top left
	float3 ntr;//near top right
	float3 nbl;
	float3 nbr;

	float3 ftl;
	float3 ftr;
	float3 fbl;//far bottom left
	float3 fbr;//far bottom right

	float height = 600;
	float width = 800;
private:
	float movementSpeed = cameraSpeed;
	float3 cameraTarget = float3::zero;
	bool orbit = false;
	float yaw = 0.0f;
	float pitch = -90.0f;

	Frustum frustum;
	float aspect = 1.f;

	unsigned fb_depth = 0;
	unsigned fb_tex = 0;
	unsigned fb_width = 0;
	unsigned fb_height = 0;
};
#endif