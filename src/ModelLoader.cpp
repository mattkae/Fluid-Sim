#include "ModelLoader.h"
#include "BinarySerializer.h"
#include "LoadModel.h"
#include "BinarySerializer.h"
#include "TextureInfo.h"
#include "Logger.h"
#include "Mesh.h"
#include "Box.h"
#include "Bone.h"
#include <SOIL.h>

GLuint loadTexture(String path);

void ModelLoader::loadTextureList(const char* path) {
	textureList.allocate(16);

	BinarySerializer serializer(path, SerializationMode::READ);
	int numTextures = serializer.readInt32();
	for (int textureIndex = 0; textureIndex < numTextures; textureIndex++) {
		TextureInfo info;
		info.read(serializer);

		GeneratedTexture item;
		item.uniqueId = info.uniqueId;
		item.texture = loadTexture(info.fullpath);
		textureList.add(&item);
	}

	serializer.close();
}

GLuint loadTexture(String path) {
	GLuint texture;
	int width, height;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	const char* cStrPath = path.getValue();
    unsigned char* image = SOIL_load_image(cStrPath, &width, &height, 0, SOIL_LOAD_RGBA);
	if (image == NULL) {
	  logger_error("Unable to load image from path: %s, reason: %s", path.getValueConst(), SOIL_last_result());
		return 0;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return texture;
}

ModelLoadResult ModelLoader::loadSerializedModel(const char* path) {
	BinarySerializer serializer(path, SerializationMode::READ);
	LoadModel intermediateModel;
	intermediateModel.readLoadModel(serializer);
	serializer.close();
	return ModelLoader::loadFromLoadModel(intermediateModel);
}

void copyBoneTreeNode(BoneTreeNode* node, const LoadBoneNode& nodeToCopy) {
	node->boneIndex = nodeToCopy.boneIndex;
	node->nodeTransform = nodeToCopy.nodeTransform;
	node->numChildren = nodeToCopy.children.numElements;
	node->children = new BoneTreeNode[nodeToCopy.children.numElements];
	for (unsigned int nodeIdx = 0; nodeIdx < node->numChildren; nodeIdx++) {
		copyBoneTreeNode(&node->children[nodeIdx], nodeToCopy.children[nodeIdx]);
	}
}

ModelLoadResult ModelLoader::loadFromLoadModel(LoadModel& intermediateModel) {
	ModelLoadResult retval;
	retval.model.inverseRootNode = intermediateModel.inverseRootNode;
	retval.model.numMeshes = intermediateModel.meshes.numElements;
	retval.model.meshes = new Mesh[retval.model.numMeshes];
	retval.model.numBones = intermediateModel.bones.numElements;
	retval.model.bones = new Bone[retval.model.numBones];
	for (unsigned int boneIdx = 0; boneIdx < retval.model.numBones; boneIdx++) {
		retval.model.bones[boneIdx].offsetMatrix = intermediateModel.bones[boneIdx].offsetMatrix;
	}

	if (intermediateModel.rootNode.children.numElements > 0) {
		retval.model.rootNode = new BoneTreeNode();
		copyBoneTreeNode(retval.model.rootNode, intermediateModel.rootNode);
	}

	for (int meshIdx = 0; meshIdx < retval.model.numMeshes; meshIdx++) {
		retval.model.meshes[meshIdx].initialize(intermediateModel.meshes[meshIdx], &textureList);
	}

	retval.box.lowerLeft = intermediateModel.lowerLeftBoundingBoxCorner;
	retval.box.upperRight = intermediateModel.upperRightBoundingBoxCorner;
	retval.model.animationController.numAnimations = intermediateModel.animations.numElements;
	retval.model.animationController.animationList  = new Animation[retval.model.animationController.numAnimations];
	for (unsigned int animationIdx = 0; animationIdx < retval.model.animationController.numAnimations; animationIdx++) {
		retval.model.animationController.animationList[animationIdx] = intermediateModel.animations[animationIdx];
		retval.model.animationController.animationList[animationIdx].totalTicks = retval.model.animationController.animationList[animationIdx].duration 
			* retval.model.animationController.animationList[animationIdx].ticksPerSecond;
	}

	return retval;
}

void ModelLoader::free() {
	textureList.deallocate();
}
