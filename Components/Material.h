#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Component.h"
#include "../MathGeoLib/include/Math/float4.h"

class Material : public Component {
public:
	unsigned program;
	unsigned diffuseMap = 0;
	math::float4 diffuseColor = math::float4::zero;
	float kDiffuse = 0.0f;
	unsigned specularMap = 0;
	math::float3 specularColor = math::float3::zero;
	float shininess = 0.0f;
	float kSpecular = 0.0f;
	unsigned occlusionMap = 0;
	float kAmbient = 0.0f;
	unsigned emissiveMap = 0;
	math::float3 emissiveColor = math::float3::zero;
	std::vector<Texture> texturesLoaded;

public:
	Material(GameObject* owner) : Component(owner, ComponentType::Material) {}
	virtual ~Material(){}
	void DrawView();
};
#endif