#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleTexture.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
#include "ModuleImgui.h"
#include "ModuleCamera.h"
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
		model = new GameObject(node->mName.C_Str());
		model->parent = parent;
		
		((Transform*)model->FindComponent(ComponentType::Transform))->SetTransform(node->mTransformation);
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		newMesh = processMesh(mesh, scene);
		meshes.push_back(newMesh);
		model->components.push_back(new ComponentMesh(model, newMesh));
		++numMeshes;
		parent->children.push_back(model);

		
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) 
	{
		processNode(node->mChildren[i], scene,parent);
		
	}
	ComponentMesh* thisMesh = ((ComponentMesh*)model->FindComponent(ComponentType::Mesh));
	
}



Mesh* ModuleModelLoader::processMesh(aiMesh *mesh, const aiScene *scene) 
{
	Mesh* res = new Mesh;
	numVertices += mesh->mNumVertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		float3 vector;

		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		// texture coordinates
		if (mesh->mTextureCoords[0]) 
		{
			float2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else 
			vertex.TexCoords = float2(0.0f, 0.0f);

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
		res->vertices.push_back(vertex);
	}


	for (unsigned int i = 0; i < mesh->mNumFaces; i++) 
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			res->indices.push_back(face.mIndices[j]);
	}

	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	res->textures.insert(res->textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// 2. specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	res->textures.insert(res->textures.end(), specularMaps.begin(), specularMaps.end());

	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	res->textures.insert(res->textures.end(), normalMaps.begin(), normalMaps.end());

	// 4. height maps

	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	res->textures.insert(res->textures.end(), heightMaps.begin(), heightMaps.end());

	res->setupMesh();
	
	return res;
}


std::vector<Texture> ModuleModelLoader::loadMaterialTextures(aiMaterial *mat, aiTextureType type, char* typeName) 
{
	std::vector<Texture> textures;
	
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < texturesLoaded.size(); j++) 
		{
			if (std::strcmp(texturesLoaded[j].path, str.C_Str()) == 0)
			{
				textures.push_back(texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) 
		{
			if (FileExists(str.C_Str()))
			{
				App->imgui->AddLog("Texture loading from: %s\n", str.C_Str());
				finalPath = str.C_Str();
				std::size_t found = finalPath.find_last_of("/\\");
				finalPath = finalPath.substr(found + 1);
				finalPath = "Textures/"+finalPath;
			}
			else if (FileExists(modelPath.c_str()))
			{
				modelPath.append(str.C_Str());
				App->imgui->AddLog("Texture loading from Models Path: %s\n", modelPath.c_str());
				finalPath = modelPath.c_str();
			}
			else if(FileExists(myTexturesPath.c_str()))
			{
				myTexturesPath += str.C_Str();
				if (FileExists(myTexturesPath.c_str()))
				{
					App->imgui->AddLog("Texture loading from Textures Path: %s\n", myTexturesPath.c_str());
					finalPath = myTexturesPath.c_str();

				}
			}
			else {
				App->imgui->AddLog("Couldn't find a texture to load, loading previous one.\n");
				finalPath = "";
			}
			if (finalPath == "")
			{
				return textures;
			}
			Texture texture = App->texture->LoadTexture(finalPath.c_str());
			texture.type = typeName;
			textures.push_back(texture);
			texturesLoaded.push_back(texture); 
			
		}
	}
	return textures;
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
	/*float positionObject[3] = { 
		((Transform*)model->FindComponent(ComponentType::Transform))->position.x, 
		((Transform*)model->FindComponent(ComponentType::Transform))->position.y,
		((Transform*)model->FindComponent(ComponentType::Transform))->position.z
	};
	float rotationObject[3] = {
		((Transform*)model->FindComponent(ComponentType::Transform))->rotation.x,
		((Transform*)model->FindComponent(ComponentType::Transform))->rotation.y,
		((Transform*)model->FindComponent(ComponentType::Transform))->rotation.z
	};
	float scaleObject[3] = {
		((Transform*)model->FindComponent(ComponentType::Transform))->scaling.x,
		((Transform*)model->FindComponent(ComponentType::Transform))->scaling.y,
		((Transform*)model->FindComponent(ComponentType::Transform))->scaling.z
	};*/
	
	ImGui::Begin("Properties");
	if (ImGui::CollapsingHeader("Transformation"))
	{
		
		/*ImGui::DragFloat3("Position", positionObject);
		ImGui::DragFloat3("Rotation", rotationObject);
		ImGui::DragFloat3("Scale", scaleObject);*/
	}
	if (ImGui::CollapsingHeader("Geometry"))
	{
		ImGui::Text("Number of meshes: %d", numMeshes);
		ImGui::Text("Number of vertices: %d",numVertices);
		ImGui::Text("Number of triangles: %d", numVertices/3);
	}
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
