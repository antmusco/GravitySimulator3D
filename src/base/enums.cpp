#include "enums.h"

std::map<std::string, ShaderType> stringShaders = {

	std::make_pair(std::string("Mesh"), ShaderType::Mesh),
	std::make_pair(std::string("Splat"), ShaderType::Splat),

};

std::map<Uniform, std::string> meshUniformStrings = {

	std::make_pair(Uniform::ModelToProjection, std::string("modelToProjectionMatrix")),
	std::make_pair(Uniform::ModelToWorld,	   std::string("modelToWorldMatrix")),
	std::make_pair(Uniform::Texture, 		   std::string("texture")),
	std::make_pair(Uniform::LightSource, 	   std::string("lightSource")),
	std::make_pair(Uniform::AmbientLight,	   std::string("ambientLight")),

};

std::map<MeshAttribute, std::string> meshAttributeStrings = {

	std::make_pair(MeshAttribute::Position, std::string("modelPosition")),
	std::make_pair(MeshAttribute::Color,	std::string("modelColor")),
	std::make_pair(MeshAttribute::Normal, 	std::string("modelNormal")),
	std::make_pair(MeshAttribute::TexCoord, std::string("modelTexCoord")),

};