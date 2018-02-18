#include "Scene.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene() {
  // Load shaders
  mShadowShader.load("src/shaders/shadows.vert", "src/shaders/shadows.frag");
  mSceneShader.load("src/shaders/model.vert", "src/shaders/model.frag");

  // Load models
  Model model("assets/BaymaxWhiteOBJ/Bigmax_White_OBJ.obj");
  glm::mat4 transform(1.0);
  transform = glm::scale(transform, glm::vec3(0.05));
  transform = glm::translate(transform, glm::vec3(0, -105, 0));
  model.set_model(transform);
  mModels.push_back(model);
  // Bind uniforms to proper index
  mSceneShader.Use();
  mSceneShader.SetUniform1i("uDirShadows[0]", 0);
  mSceneShader.SetUniform1i("uDirShadows[1]", 1);
  mSceneShader.SetUniform1i("uMaterial.diffuseTex", 8);
  mSceneShader.SetUniform1i("uMaterial.specularTex", 9);

  // Load lights
  Light pLight = get_directional(1600, 1200);
  pLight.direction = glm::vec3(0, -1.0, 0);
  pLight.usesShadows = true;
  mLights.push_back(pLight);
}

Scene::~Scene() {
  
}

void Scene::update(double dt) {
    move_camera(dt, &mCamera);
    mCamera.update(dt);
}

void Scene::render() {
  render_shadows();
  render_scene();
}

void Scene::render_shadows() {
  if (!mUseShadows) return;
  mShadowShader.Use();
  
  for (auto light : mLights) {
    render_shadows_from_light(&light, &mShadowShader, this);
  }
}

void Scene::render_scene() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mSceneShader.Use();
  
  mCamera.render(&mSceneShader);

  mSceneShader.SetUniform3f("uAmbient", 0.f, 0.f, 0.f);
  mSceneShader.SetUniform1i("uNumLights", mLights.size());
    
  for (int lidx = 0; lidx < mLights.size(); lidx++) {
    Light* light = &mLights[lidx];
    render_light(light, &mSceneShader, lidx);
  }

  render_models(&mSceneShader);
}

void Scene::render_models(Shader* shader) {
  for (auto model : mModels) {
    model.render(&mSceneShader);
  }
}
