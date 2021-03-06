#pragma once
#include "Shader.h"
#include "Constants.h"

struct MaterialUniformMapping {
	int MATERIAL_DIFFUSE;
	int MATERIAL_SPECULAR;
	int MATERIAL_EMISSIVE;
	int MATERIAL_AMBIENT;
	int MATERIAL_DIFFUSE_PROPERTY;
	int MATERIAL_SPECULAR_PROPERTY;
	int MATERIAL_OPACITY;
	int MATERIAL_USE_TEXTURE;
	int MATERIAL_SHININESS;
	int DIFFUSE;
	int SPECULAR;
	int AMBIENT;
	int NORMAL;

	void initialize(const Shader& shader);
};

struct ModelUniformMapping {
	int MODEL;
	int BONES;
	int DISABLE_BONES;
	MaterialUniformMapping materialUniformMapping;

	void initialize(const Shader& shader, bool withMaterial);
};


struct CameraUniformMapping {
	int PROJECTION_MATRIX;
	int VIEW_MATRIX;
	int EYE_MATRIX;
	bool WITH_EYE;
	void initialize(const Shader& shader, bool withEye);
};

struct LightUniformMapping {
	int LIGHT_AMBIENT;
	int LIGHT_NUM_LIGHTS;
	int LIGHT_COLOR[Constants::Rendering::MAX_NUM_LIGHTS];
	int LIGHT_DIRECTION[Constants::Rendering::MAX_NUM_LIGHTS];
	int LIGHT_POSITION[Constants::Rendering::MAX_NUM_LIGHTS];
	int LIGHT_CONSTANT[Constants::Rendering::MAX_NUM_LIGHTS];
	int LIGHT_LINEAR[Constants::Rendering::MAX_NUM_LIGHTS];
	int LIGHT_QUADRATIC[Constants::Rendering::MAX_NUM_LIGHTS];
	int LIGHT_COSINE_CUTOFF[Constants::Rendering::MAX_NUM_LIGHTS];
	int LIGHT_DROP_OFF[Constants::Rendering::MAX_NUM_LIGHTS];
	int LIGHT_USES_SHADOWS[Constants::Rendering::MAX_NUM_LIGHTS];
	int LIGHT_DIR_SHADOW[Constants::Rendering::MAX_NUM_LIGHTS];
	int LIGHT_SHADOWMATRIX[Constants::Rendering::MAX_NUM_LIGHTS];
	int LIGHT_FAR_NEAR_PLANE;

	void initialize(const Shader& shader);
};

struct ModelShaderMapping {
	Shader shader;

	int UNIFORM_CLIP_PLANE;
	CameraUniformMapping cameraUniformMapping;
	ModelUniformMapping modelUniformMapping;
	LightUniformMapping lightUniformMapping;

	void initialize();
};


struct ShadowShaderMapping {
	Shader shader;
	ModelUniformMapping modelUniformMapping;
	int LIGHT_VIEW_MATRIX;
	int LIGHT_PROJ_MATRIX;
	void initialize();
};

struct SkyboxShaderMapping {
	Shader shader;
	int VIEW;
	int PROJECTION;
	void initialize();
};

struct DomeSkyShaderMapping {
	Shader shader;
	CameraUniformMapping cameraMapping;
	int UNIFORM_LOW_COLOR;
	int UNIFORM_HIGH_COLOR;
	int UNIFORM_MIX_POSITIONS;
    void initialize();
};

struct OrthographicShaderMapping {
	Shader shader;

	int PROJECTION;

	void initialize();
};

struct TextShaderMapping {
	Shader shader;

	int PROJECTION;
	int GLYPH_TEXTURE;

	void initialize();
};

struct ParticleShaderMapping {
	Shader shader;
	CameraUniformMapping cameraMapping;
	int MODEL;
	int CAMERA_RIGHT;
	int CAMERA_UP;
	void initialize();
};

struct WaterShaderMapping {
	Shader shader;
	CameraUniformMapping cameraMapping;
	int UNIFORM_MODEL;
	int UNIFORM_TIME_PASSED_MS;
	int UNIFORM_PERIOD;
	int UNIFORM_AMPLITUDE;
	int UNIFORM_REFLECTION;
	int UNIFORM_REFRACTION;
	int UNIFORM_DU_DV_MAP;
	int UNIFORM_DU_DV_MOVE_FACTOR;
	int UNIFORM_NORMAL_MAP;

	MaterialUniformMapping materialUniformMapping;
	LightUniformMapping lightUniformMapping;

	void initialize();
};

struct PostProcessorShaderMapping {
	Shader shader;
	int UNIFORM_SCENE_BUFFER;
	int UNIFORM_EXPOSURE;

	void initialize();
};

struct TerrainShaderMapping {
	Shader shader;

	LightUniformMapping lightUniformMapping;
	CameraUniformMapping cameraUniformMapping;
	int UNIFORM_MODEL;
	int UNIFORM_CLIP_PLANE;
	int UNIFORM_DIFFUSE[3];
	int UNIFORM_SPECULAR[3];
	int UNIFORM_NORMAL[3];

	void initialize();
	
};

namespace ShaderUniformMapping {
	extern ModelShaderMapping GlobalModelShaderMapping;
	extern PostProcessorShaderMapping GlobalPostProcessorShaderMapping;
	extern ShadowShaderMapping GlobalShadowShaderMapping;
	extern SkyboxShaderMapping GlobalSkyboxShaderMapping;
	extern OrthographicShaderMapping GlobalOrthographicShaderMapping;
	extern TextShaderMapping GlobalTextShaderMapping;
	extern ParticleShaderMapping GlobalParticleShaderMapping;
	extern WaterShaderMapping GlobalWaterShaderMapping;
	extern DomeSkyShaderMapping GlobalDomeSkyShaderMapping;
	extern TerrainShaderMapping GlobalTerrainShaderMapping;
	void initialize();
	void free(); // @TODO: Implement this
};
