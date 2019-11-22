#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleTexture.h"
#include "ModuleImgui.h"
#include "ModuleCamera.h"
#include "assimp/include/assimp/Importer.hpp"
#include "assimp/include/assimp/scene.h"
#include "assimp/include/assimp/postprocess.h"



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
	return true;
}

void ModuleModelLoader::Draw(unsigned int program)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i]->Draw(program);
}



void ModuleModelLoader::LoadModel(const char* path) 
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		App->imgui->AddLog("ERROR::ASSIMP:: %s\n", importer.GetErrorString());
		return;
	}
	else {
		App->imgui->AddLog("Path of the geometry correct.\n");
	}
	processNode(scene->mRootNode, scene);
}



void ModuleModelLoader::processNode(aiNode *node, const aiScene *scene) 
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) 
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(new Mesh(processMesh(mesh, scene)));
		++numMeshes;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) 
	{
		processNode(node->mChildren[i], scene);
	}
}



Mesh ModuleModelLoader::processMesh(aiMesh *mesh, const aiScene *scene) 
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
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
		numPolys = (vertices.size())/3;
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
		vertices.push_back(vertex);
	}


	for (unsigned int i = 0; i < mesh->mNumFaces; i++) 
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	if (textures[0].type == "texture_diffuse") {
		textureType = textures[0].type;
	}
	// 2. specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	if (textures[0].type == "texture_specular") {
		textureType = textures[0].type;
	}
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	if (textures[0].type == "texture_normal") {
		textureType = textures[0].type;
	}
	// 4. height maps

	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	
	textureWidth = textures[0].width;//TODO, IMPROVE CODE
	textureHeight = textures[0].height;//TODO, IMPROVE CODE
	textureId = textures[0].id;//TODO, IMPROVE CODE
	numPolys /= 3;
	return Mesh(vertices, indices, textures);
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
			Texture texture = App->texture->LoadTexture(str.C_Str());
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
	float minX = 10000000.0f, minY = 10000000.0f, minZ = 10000000.0f;

	//Max values
	float maxX = -1000000.0f, maxY = -1000000.0f, maxZ = -1000000.0f;

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


	correctCameraPositionForModel = float3((maxX + minX) / 2, (maxY + minY) / 2, -2 * (maxZ - minZ));
	App->imgui->AddLog("Compute the camera position depending of model size: (%.3f,%.3f,%.3f)", correctCameraPositionForModel.x, correctCameraPositionForModel.y, correctCameraPositionForModel.z);

	modelCenter = correctCameraPositionForModel;
	modelCenter.z = (maxZ - minZ) / 2;
	App->imgui->AddLog("Computing models center: (%.3f,%.3f,%.3f) \n", modelCenter.x, modelCenter.y, modelCenter.z);


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
