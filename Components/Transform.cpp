#include "Transform.h"
#include "../GameObject.h"
#include "../imgui/imgui.h"

void Transform::DrawView() {
	if (ImGui::TreeNode("Transform")) {
		
		ImGui::DragFloat3("Position:", position.ptr());
		ImGui::DragFloat3("Scaling:", scaling.ptr());
		ImGui::DragFloat3("Rotation:", rotationEuler.ptr());
		ImGui::TreePop();
	}
}

void Transform::CalculateWorldTransform() {
	if (owner->parent) {
		Transform* trans = ((Transform*)owner->parent->FindComponent(ComponentType::Transform));
		if (trans) {
			worldTransform = trans->localTransform * localTransform;
		}
	}
	else {
		worldTransform = localTransform;
	}
}

void Transform::SetTransform(const aiMatrix4x4& trans) {
	localTransform.SetRow(0, float4(trans.a1, trans.a2, trans.a3, trans.a4));
	localTransform.SetRow(1, float4(trans.b1, trans.b2, trans.b3, trans.d4));
	localTransform.SetRow(2, float4(trans.c1, trans.c2, trans.c3, trans.c4));
	localTransform.SetRow(3, float4(trans.d1, trans.d2, trans.d3, trans.d4));
	
	localTransform.Decompose(position, rotation, scaling);
	rotationEuler = rotation.ToEulerXYZ();
	rotationEuler = RadToDeg(rotationEuler);
	CalculateWorldTransform();
}
void Transform::CalculateTransform() {
	rotation = rotation.FromEulerXYZ(DegToRad(rotationEuler.x), DegToRad(rotationEuler.y), DegToRad(rotationEuler.z));
	localTransform = localTransform.FromTRS(position, rotation, scaling);
	CalculateWorldTransform();
}
