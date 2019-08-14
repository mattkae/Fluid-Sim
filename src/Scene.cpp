#include "Scene.h"
#include "Camera.h"
#include "GlmUtility.h"
#include "ImageUtil.h"
#include "Input.h"
#include "Physics.h"
#include "Logger.h"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Scene::Scene() {
	ui.generate();
	ui.scene = this;
    mShadowShader.load("src/shaders/shadows.vert", "src/shaders/shadows.frag");

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error DURING setup: %d\n", err);
    }
}

Scene::Scene(const char *jsonPath) : Scene() { this->loadFromJson(jsonPath); }

Scene::~Scene() {}

void to_json(json &j, const Scene &scene) {
    j = json{{"models", scene.models}, {"lights", scene.lights}, {"spheres", scene.spheres}};
}

void from_json(const json &j, Scene &scene) {
	if (j.count("models") != 0) {
		j.at("models").get_to<std::vector<Model>>(scene.models);
	}
	if (j.count("lights") != 0) {
		j.at("lights").get_to<std::vector<Light>>(scene.lights);
	}
	if (j.count("spheres") != 0) {
		j.at("spheres").get_to<std::vector<Sphere>>(scene.spheres);
	}
	if (j.count("particleEmitters") != 0) {
		j.at("particleEmitters").get_to<std::vector<ParticleEmitter>>(scene.particleEmitters);
	}
    if (j.count("terrain") == 1) {
        j.at("terrain").get_to<Terrain>(scene.mTerrain);
    }
    if (j.count("skybox") == 1) {
        j.at("skybox").get_to<Skybox>(scene.mSkybox);
    }

    if (j.count("useDeferredRendering") == 1) {
        j.at("useDeferredRendering").get_to<bool>(scene.useDefferredRendering);
    }

    if (scene.useDefferredRendering) {
        scene.mDeferredBuffer.generate();
        scene.mSceneShader.load("src/shaders/DefferedModel.vert", "src/shaders/DefferedModel.frag");
        scene.mSceneShader.use();
        scene.mSceneShader.set_uniform_1i("uPosition", 0);
        scene.mSceneShader.set_uniform_1i("uNormal", 1);
        scene.mSceneShader.set_uniform_1i("uAlbedoSpec", 2);
    } else {
        scene.mSceneShader.load("src/shaders/model.vert", "src/shaders/model.frag");
        scene.mSceneShader.use();
        scene.mSceneShader.set_uniform_1i("uDirShadow", 0);
        scene.mSceneShader.set_uniform_1i("uMaterial.diffuseTex", 8);
        scene.mSceneShader.set_uniform_1i("uMaterial.specularTex", 9);
    }

}

void Scene::loadFromJson(const char *jsonPath) {
    for (auto model : models) {
        model.free();
    }
    models.clear();
    for (auto mSphere : spheres) {
        mSphere.free();
	}
    spheres.clear();

    for (auto light : lights) {
        light.free();
    }
    lights.clear();

	for (auto emitter : particleEmitters) {
		emitter.free();
	}
	particleEmitters.clear();

    mSkybox.free();
    mTerrain.free();
    mDeferredBuffer.free();

    std::ifstream sceneFile(jsonPath);

	try {
		json sceneJson = json::parse(sceneFile);
		from_json(sceneJson, *this);
	} catch (std::exception e) {
        std::cerr << "Scene::load_from_json - " << e.what() << "\n";
	}
}

void Scene::update(double dt) {
    moveCamera(dt, &mCamera);
    mCamera.update(dt);

	Input* input = Input::getInstance();
    const ClickState& leftClickState = input->getLeftClickState();
    if (leftClickState.isDown) {
        printf("%f, %f\n", leftClickState.x, leftClickState.y);
	}
    
	for (auto& emitter : particleEmitters) {
		emitter.update(dt);
	}

    for (auto& light : lights) {
        light.update(dt);
    }

	ui.update(dt);
}

void Scene::render() const {
    renderShadows();
    renderGBuffer();
    renderScene();
}

void Scene::renderShadows() const {
    if (!mUseShadows)
        return;

    mShadowShader.use();

    glCullFace(GL_FRONT);
    for (auto light : lights) {
        light.render_shadows(mShadowShader, *this);
    }
    glCullFace(GL_BACK);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during shadow pass: %d\n", err);
    }
}

void Scene::renderGBuffer() const {
    if (!useDefferredRendering) {
        return;
    }

    mDeferredBuffer.renderToBuffer(mCamera, this);
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during gBuffer pass: %d\n", err);
    }
}

void Scene::renderScene() const {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mSkybox.render(mCamera);

	for (auto emitter : particleEmitters) {
	 	emitter.render(mCamera);
    }

    mSceneShader.use();
    mCamera.render(mSceneShader);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mSceneShader.set_uniform_3f("uAmbient", 0.3f, 0.3f, 0.3f);
    mSceneShader.set_uniform_1i("uNumLights", lights.size());

    for (int lidx = 0; lidx < lights.size(); lidx++) {
        lights[lidx].render(mSceneShader, lidx);
    }

    if (useDefferredRendering) {
        mDeferredBuffer.renderToScreen(mSceneShader);
    } else {
        renderModels(mSceneShader);
    }

    glDisable(GL_BLEND);

	ui.render();

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during scene pass: %i, %s\n", err, glewGetErrorString(err));
    }
}

void Scene::renderModels(const Shader &shader, bool withMaterial) const {
    mTerrain.render(shader, withMaterial);

    for (auto model : models) {
        model.render(shader, withMaterial);
    }

    for (auto sphere : spheres) {
	    sphere.render(shader, withMaterial);
    }
}
