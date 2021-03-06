#include "ShaderUniformMapping.h"
#include <sstream>

const char* DEFFERRED_MODEL_SHADER_VERT = "src/shaders/model_defferred.vert";
const char* DEFFERRED_MODEL_SHADER_FRAG = "src/shaders/model_defferred.frag";

ModelShaderMapping ShaderUniformMapping::GlobalModelShaderMapping;
PostProcessorShaderMapping ShaderUniformMapping::GlobalPostProcessorShaderMapping;
ShadowShaderMapping ShaderUniformMapping::GlobalShadowShaderMapping;
SkyboxShaderMapping ShaderUniformMapping::GlobalSkyboxShaderMapping;
OrthographicShaderMapping ShaderUniformMapping::GlobalOrthographicShaderMapping;
TextShaderMapping ShaderUniformMapping::GlobalTextShaderMapping;
ParticleShaderMapping ShaderUniformMapping::GlobalParticleShaderMapping;
WaterShaderMapping ShaderUniformMapping::GlobalWaterShaderMapping;
DomeSkyShaderMapping ShaderUniformMapping::GlobalDomeSkyShaderMapping;
TerrainShaderMapping ShaderUniformMapping::GlobalTerrainShaderMapping;

void ShaderUniformMapping::initialize() {
	GlobalModelShaderMapping.initialize();
	GlobalPostProcessorShaderMapping.initialize();
	GlobalShadowShaderMapping.initialize();
	GlobalSkyboxShaderMapping.initialize();
	GlobalOrthographicShaderMapping.initialize();
	GlobalTextShaderMapping.initialize();
	GlobalParticleShaderMapping.initialize();
	GlobalWaterShaderMapping.initialize();
	GlobalDomeSkyShaderMapping.initialize();
	GlobalTerrainShaderMapping.initialize();
}

void ShaderUniformMapping::free() {
	glDeleteShader(GlobalModelShaderMapping.shader);
	glDeleteShader(GlobalPostProcessorShaderMapping.shader);
	glDeleteShader(GlobalShadowShaderMapping.shader);
	glDeleteShader(GlobalSkyboxShaderMapping.shader);
	glDeleteShader(GlobalOrthographicShaderMapping.shader);
	glDeleteShader(GlobalTextShaderMapping.shader);
	glDeleteShader(GlobalParticleShaderMapping.shader);
	glDeleteShader(GlobalWaterShaderMapping.shader);
	glDeleteShader(GlobalTerrainShaderMapping.shader);
}

inline int getArrayUniform(const Shader& shader, const int index, const char *attribute, const char *property = nullptr) {
    std::ostringstream ss;
    ss << attribute << "[" << index << "]";

    if (property) {
        ss << "." << property;
    }

    return getShaderUniform(shader, ss.str().c_str());
}

void LightUniformMapping::initialize(const Shader& shader) {
	LIGHT_AMBIENT = getShaderUniform(shader, "uAmbient");
	LIGHT_NUM_LIGHTS = getShaderUniform(shader, "uNumLights");
	LIGHT_FAR_NEAR_PLANE = getShaderUniform(shader, "uFarNear");

	for (int index = 0; index < Constants::Rendering::MAX_NUM_LIGHTS; index++) {
		LIGHT_COLOR[index] = getArrayUniform(shader, index, "uLights", "color");
		LIGHT_DIRECTION[index] = getArrayUniform(shader, index, "uLights", "direction");
		LIGHT_POSITION[index] = getArrayUniform(shader, index, "uLights", "position");
		LIGHT_CONSTANT[index] = getArrayUniform(shader, index, "uLights", "constant");
		LIGHT_LINEAR[index] = getArrayUniform(shader, index, "uLights", "linear");
		LIGHT_QUADRATIC[index] = getArrayUniform(shader, index, "uLights", "quadratic");
		LIGHT_COSINE_CUTOFF[index] = getArrayUniform(shader, index, "uLights", "cosineCutOff");
		LIGHT_DROP_OFF[index] = getArrayUniform(shader, index, "uLights", "dropOff");
		LIGHT_USES_SHADOWS[index] = getArrayUniform(shader, index, "uLights", "usesShadows");
		LIGHT_DIR_SHADOW[index] = getArrayUniform(shader, index, "uDirShadow");
		LIGHT_SHADOWMATRIX[index] = getArrayUniform(shader, index, "uLights", "shadowMatrix");
	}
}

void ModelShaderMapping::initialize() {
	const char* FORWARD_MODEL_SHADER_VERT = "src/shaders/model_forward.vert";
	const char* FORWARD_MODEL_SHADER_FRAG = "src/shaders/model_forward.frag";
	shader = loadShader(FORWARD_MODEL_SHADER_VERT, FORWARD_MODEL_SHADER_FRAG);

	UNIFORM_CLIP_PLANE = getShaderUniform(shader, "uClipPlane");
	cameraUniformMapping.initialize(shader, true);
	modelUniformMapping.initialize(shader, true);
	lightUniformMapping.initialize(shader);
}

void PostProcessorShaderMapping::initialize() {
	const char* POSTPROCESS_VERT = "src/shaders/model_postprocess.vert";
	const char* POSTPROCESS_FRAG = "src/shaders/model_postprocess.frag";
	shader = loadShader(POSTPROCESS_VERT, POSTPROCESS_FRAG);

	UNIFORM_SCENE_BUFFER = getShaderUniform(shader, "uSceneTexture");
	UNIFORM_EXPOSURE = getShaderUniform(shader, "uExposure");
}

void ShadowShaderMapping::initialize() {
	const char* SHADOW_SHADER_VERT = "src/shaders/shadows.vert";
	const char* SHADOW_SHADER_FRAG = "src/shaders/shadows.frag";
	shader = loadShader(SHADOW_SHADER_VERT, SHADOW_SHADER_FRAG);
	modelUniformMapping.initialize(shader, false);
	LIGHT_VIEW_MATRIX = getShaderUniform(shader, "uView");
	LIGHT_PROJ_MATRIX = getShaderUniform(shader, "uProjection");
}

void ModelUniformMapping::initialize(const Shader& shader, bool withMaterial) {
	MODEL = getShaderUniform(shader, "uModel");
	BONES = getShaderUniform(shader, "uBones");
	DISABLE_BONES = getShaderUniform(shader, "uDisableBones");

	if (withMaterial) {
		materialUniformMapping.initialize(shader);
	}
}

void MaterialUniformMapping::initialize(const Shader& shader) {
	MATERIAL_DIFFUSE = getShaderUniform(shader, "uMaterial.diffuse");
	MATERIAL_SPECULAR = getShaderUniform(shader, "uMaterial.specular");
	MATERIAL_AMBIENT = getShaderUniform(shader, "uMaterial.ambient");
	MATERIAL_EMISSIVE = getShaderUniform(shader, "uMaterial.emissive");
	MATERIAL_DIFFUSE_PROPERTY = getShaderUniform(shader, "uMaterial.diffuseProperty");
	MATERIAL_SPECULAR_PROPERTY = getShaderUniform(shader, "uMaterial.specularProperty");
	MATERIAL_OPACITY = getShaderUniform(shader, "uMaterial.opacity");
	MATERIAL_USE_TEXTURE = getShaderUniform(shader, "uMaterial.useTexture");
	MATERIAL_SHININESS = getShaderUniform(shader, "uMaterial.shininess");

	DIFFUSE = getShaderUniform(shader, "uDiffuse");
	SPECULAR = getShaderUniform(shader, "uSpecular");
	AMBIENT = getShaderUniform(shader, "uAmbientMap");
	NORMAL = getShaderUniform(shader, "uNormalMap");
}

void CameraUniformMapping::initialize(const Shader& shader, bool withEye) {
	PROJECTION_MATRIX = getShaderUniform(shader, "uProjection");
	VIEW_MATRIX = getShaderUniform(shader, "uView");
	WITH_EYE = withEye;
	if (withEye) {
		EYE_MATRIX = getShaderUniform(shader, "uEye");
	}
}

void SkyboxShaderMapping::initialize() {
	const char* SKYBOX_SHADER_VERT = "src/shaders/skybox.vert";
	const char* SKYBOX_SHADER_FRAG = "src/shaders/skybox.frag";
	shader = loadShader(SKYBOX_SHADER_VERT, SKYBOX_SHADER_FRAG);
	VIEW = getShaderUniform(shader, "uView");
	PROJECTION = getShaderUniform(shader, "uProjection");
}

void DomeSkyShaderMapping::initialize() {
	const char* SKYBOX_SHADER_VERT = "src/shaders/dome_skybox.vert";
	const char* SKYBOX_SHADER_FRAG = "src/shaders/dome_skybox.frag";
	shader = loadShader(SKYBOX_SHADER_VERT, SKYBOX_SHADER_FRAG);
    UNIFORM_LOW_COLOR = getShaderUniform(shader, "uLowColor");
	UNIFORM_HIGH_COLOR  = getShaderUniform(shader, "uHighColor");
	UNIFORM_MIX_POSITIONS = getShaderUniform(shader, "uMixYStartEnd");
	cameraMapping.initialize(shader, false);
}

void OrthographicShaderMapping::initialize() {
	const char* VERTEX = "src/shaders/Orthographic.vert";
	const char* FRAG = "src/shaders/Orthographic.frag";
	shader = loadShader(VERTEX, FRAG);
	PROJECTION = getShaderUniform(shader, "uProjection");
}

void TextShaderMapping::initialize() {
	const char* VERTEX = "src/shaders/Text.vert";
	const char* FRAG = "src/shaders/Text.frag";
	shader = loadShader(VERTEX, FRAG);
	PROJECTION = getShaderUniform(shader, "uProjection");
	GLYPH_TEXTURE = getShaderUniform(shader, "uGlyphTexture");
}

void ParticleShaderMapping::initialize() {
	const char* VERTEX = "src/shaders/Text.vert";
	const char* FRAG = "src/shaders/Text.frag";
	shader = loadShader(VERTEX, FRAG);
	cameraMapping.initialize(shader, false);
	MODEL = getShaderUniform(shader, "uModel");
	CAMERA_RIGHT = getShaderUniform(shader, "uCameraRight");
	CAMERA_UP = getShaderUniform(shader, "uCameraUp");
}


void WaterShaderMapping::initialize() {
	const char* VERTEX = "src/shaders/water.vert";
	const char* FRAG = "src/shaders/water.frag";
	shader = loadShader(VERTEX, FRAG);
	cameraMapping.initialize(shader, true);
	UNIFORM_MODEL = getShaderUniform(shader, "uModel");
	UNIFORM_TIME_PASSED_MS = getShaderUniform(shader, "uTimePassedMs");
	UNIFORM_PERIOD = getShaderUniform(shader, "uPeriod");
	UNIFORM_AMPLITUDE = getShaderUniform(shader, "uAmplitude");
	UNIFORM_REFLECTION = getShaderUniform(shader, "uReflection");
	UNIFORM_REFRACTION = getShaderUniform(shader, "uRefraction");
	UNIFORM_DU_DV_MAP = getShaderUniform(shader, "uDudvMap");
	UNIFORM_DU_DV_MOVE_FACTOR = getShaderUniform(shader, "uDudvMoveFactor");
	UNIFORM_NORMAL_MAP = getShaderUniform(shader, "uNormalMap");
	materialUniformMapping.initialize(shader);
	lightUniformMapping.initialize(shader);
}

void TerrainShaderMapping::initialize() {
	const char* VERTEX = "src/shaders/Terrain.vert";
	const char* FRAG = "src/shaders/Terrain.frag";
	shader = loadShader(VERTEX, FRAG);
	useShader(shader);

	lightUniformMapping.initialize(shader);
	cameraUniformMapping.initialize(shader, true);
	UNIFORM_MODEL = getShaderUniform(shader, "uModel");
	
	UNIFORM_DIFFUSE[0] = getShaderUniform(shader, "uDiffuse[0]");
	UNIFORM_DIFFUSE[1] = getShaderUniform(shader, "uDiffuse[1]");	
	UNIFORM_DIFFUSE[2] = getShaderUniform(shader, "uDiffuse[2]");

	UNIFORM_SPECULAR[0] = getShaderUniform(shader, "uSpecular[0]");
	UNIFORM_SPECULAR[1] = getShaderUniform(shader, "uSpecular[1]");	
	UNIFORM_SPECULAR[2] = getShaderUniform(shader, "uSpecular[2]");

	UNIFORM_NORMAL[0] = getShaderUniform(shader, "uNormal[0]");
	UNIFORM_NORMAL[1] = getShaderUniform(shader, "uNormal[1]");	
	UNIFORM_NORMAL[2] = getShaderUniform(shader, "uNormal[2]");
}
