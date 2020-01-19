#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleTexture.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
#include "ModuleImgui.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "assimp/include/assimp/Importer.hpp"
#include "assimp/include/assimp/matrix4x4.h"
#include "assimp/include/assimp/scene.h"
#include "assimp/include/assimp/postprocess.h"
#include "assimp/include/assimp/Logger.hpp"
#include "assimp/include/assimp/DefaultLogger.hpp"
#include <io.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

#include "Components/Transform.h"
#include "Components/ComponentMesh.h"


class myStream : public Assimp::LogStream {

public:
	void write(const char *message) {

		App->imgui->AddLog("%s", message);
	}
};

ModuleModelLoader::ModuleModelLoader() 
{

}

ModuleModelLoader::~ModuleModelLoader()
{

}

bool ModuleModelLoader::Init() {

	return true;
}

bool ModuleModelLoader::CleanUp() 
{
	texturesLoaded.clear();
	meshes.clear();
	directory.clear();
	return true;
}

void ModuleModelLoader::Draw(unsigned int program)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i]->Draw(program);
}



void ModuleModelLoader::LoadModel(const char* path) 
{
	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);

	Assimp::DefaultLogger::get()->info("this is my info-call");

	const unsigned int severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;

	Assimp::DefaultLogger::get()->attachStream(new myStream, severity);
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		App->imgui->AddLog("ERROR::ASSIMP:: %s\n", importer.GetErrorString());
		return;
	}
	else {
		App->imgui->AddLog("Path of the geometry correct.\n");
	}	

	GameObject* modelLoaded = App->scene->CreateGameObjectByName(path);
	modelLoaded->parent = App->scene->root;
	App->scene->root->children.push_back(modelLoaded);
	processNode(scene->mRootNode, scene, modelLoaded);
}



void ModuleModelLoader::processNode(aiNode *node, const aiScene *scene, GameObject* parent)
{
	
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) 
	{
		const char* name = node->mName.C_Str();
		model = new GameObject(name);
		model->parent = parent;
		
		((Transform*)model->FindComponent(ComponentType::Transform))->SetTransform(node->mTransformation);
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		/*newMesh = processMesh(mesh, scene);
		meshes.push_back(newMesh);
		model->components.push_back(new ComponentMesh(model, newMesh));*/
		newMesh = processMesh(mesh, model);
		meshes.push_back(newMesh);
		model->components.push_back(new ComponentMesh(model, newMesh));
		processMaterials(scene->mMaterials[mesh->mMaterialIndex], model);
		parent->children.push_back(model);

		
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) 
	{
		processNode(node->mChildren[i], scene,parent);
		
	}
	//ComponentMesh* thisMesh = ((ComponentMesh*)model->FindComponent(ComponentType::Mesh));
	
}


Mesh* ModuleModelLoader::processMesh(const aiMesh* mesh, GameObject* owner) {
	Mesh* meshAux = new Mesh;
	meshAux->numPrimitives = mesh->mNumFaces;
	meshAux->numVertices = mesh->mNumVertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		float3 vector;
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		meshAux->box.maxPoint.x = max(meshAux->box.maxPoint.x, vector.x);
		meshAux->box.minPoint.x = min(meshAux->box.minPoint.x, vector.x);
		meshAux->box.maxPoint.y = max(meshAux->box.maxPoint.y, vector.y);
		meshAux->box.minPoint.y = min(meshAux->box.minPoint.y, vector.y);
		meshAux->box.maxPoint.z = max(meshAux->box.maxPoint.z, vector.z);
		meshAux->box.minPoint.z = min(meshAux->box.minPoint.z, vector.z);

		vertex.Position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) {
			float2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = float2(0.0f, 0.0f);
		}
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		meshAux->vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			meshAux->indices.push_back(face.mIndices[j]);
	}
	meshAux->SetUpMesh();
	return meshAux;
}

void ModuleModelLoader::processMaterials(const aiMaterial* mat, GameObject* owner) {
	Material* material = (Material*)owner->CreateComponent(ComponentType::Material);
	// 1. diffuse maps
	loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse", material);
	// 2. specular maps
	loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular", material);
	// 3. normal maps
	loadMaterialTextures(mat, aiTextureType_AMBIENT, "texture_normal", material);
	// 4. height maps
	loadMaterialTextures(mat, aiTextureType_EMISSIVE, "texture_height", material);
	material->program = int(ProgramType::Default);
}

void ModuleModelLoader::loadMaterialTextures(const aiMaterial* mat, aiTextureType type, const char* typeName, Material* material) {
	App->imgui->AddLog("\nLoading textures of type : %s", typeName);
	for (unsigned i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		aiTextureMapping mapping;
		unsigned uvindex = 0;
		mat->GetTexture(type, i, &str, &mapping, &uvindex);
		std::string path = str.C_Str();
		std::string textureName = path.substr(path.find_last_of('\\') + 1, path.size());
		App->imgui->AddLog("Trying to load texture: %s", path.c_str());
		if (FileExists(path.c_str()) == 1) {
			path = directory;
			path = path.append(textureName);
			App->imgui->AddLog("Trying to load texture: %s", path.c_str());
			if (FileExists(path.c_str()) == 1) {
				path = TEXTURE_PATH;
				path = path.append(textureName);
				App->imgui->AddLog("Trying to load texture: %s", path.c_str());
				if (FileExists(path.c_str()) == 1) {
					path = TEXTURE_PATH;
					path = path.append(DEFAULT_TEXTURE);
					App->imgui->AddLog("Trying to load texture: %s", path.c_str());
				}
			}
		}

		material->shininess = 64.0f;
		material->kSpecular = 0.6f;
		material->kDiffuse = 0.5f;
		material->kAmbient = 1.0f;
	}
}

void ModuleModelLoader::computeModelBoundingBox()
{

	//Min values
	float minX = 100000000.0f, minY = 100000000.0f, minZ = 100000000.0f;

	//Max values
	float maxX = -100000000.0f, maxY = -100000000.0f, maxZ = -100000000.0f;

	for (auto mesh : meshes)
	{
		for (auto vertex : mesh->vertices)
		{
			//Min vertex
			if (vertex.Position.x < minX)
				minX = vertex.Position.x;

			if (vertex.Position.y < minY)
				minY = vertex.Position.y;

			if (vertex.Position.z < minZ)
				minZ = vertex.Position.z;

			//Max vertex
			if (vertex.Position.x > maxX)
				maxX = vertex.Position.x;

			if (vertex.Position.y > maxY)
				maxY = vertex.Position.y;

			if (vertex.Position.z > maxZ)
				maxZ = vertex.Position.z;
		}
	}

	modelBox.push_back(float3(minX, minY, minZ));
	modelBox.push_back(float3(maxX, minY, minZ));
	modelBox.push_back(float3(maxX, minY, maxZ));
	modelBox.push_back(float3(minX, minY, maxZ));
	modelBox.push_back(float3(minX, maxY, minZ));
	modelBox.push_back(float3(maxX, maxY, minZ));
	modelBox.push_back(float3(maxX, maxY, maxZ));
	modelBox.push_back(float3(minX, maxY, maxZ));


	newCameraPos = float3((maxX + minX) / 2, (maxY + minY) / 2, -2 * (maxZ - minZ));
	modelCenter = newCameraPos;
	modelCenter.z = (maxZ - minZ) / 2;


	float distance = 3 * (maxX - minX);
	if (App->camera->frustum.farPlaneDistance < distance)
	{
		App->camera->frustum.farPlaneDistance = distance;
	}
	else
	{
		App->camera->frustum.farPlaneDistance = 100.0f;
	}
}
void ModuleModelLoader::ChangeModel(const char* path)
{
	texturesLoaded.clear();
	meshes.clear();
	directory.clear();
	LoadModel(path);
}

void ModuleModelLoader::ShowModelUI()
{
	
	
	ImGui::Begin("Properties");
	
	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::Text("Texture id: %d", textureId);
		ImGui::Text("Texture width: %d", textureWidth);
		ImGui::Text("Texture height: %d", textureHeight);
		//Show the model with only the mesh
		ImGui::Checkbox("Show Mesh", &meshed);
		if (meshed) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		//texture image
		for (int i = 0; i < texturesLoaded.size(); ++i)
		{
			ImGui::Image((void*)(intptr_t)texturesLoaded[i].id, ImVec2(200 * 0.5f, 200 * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		}
		
		
	}

	ImGui::End();
}


bool ModuleModelLoader::FileExists(const char* path)
{
	if (access(path, 0) == -1)
	{
		App->imgui->AddLog("File does NOT exist in path %s:\n", path);
		return false;
	}
	return true;
}
