#include <vector>
#include <glm/glm.hpp>
#include "../include/light.h"
#include "../include/directionallight.h"

DirectionalLight::DirectionalLight(const glm::vec4 &lightDir, const glm::vec4 &color, 
	const glm::vec4 &ambient, glm::vec4 lookDir, float strength)
	: Light(ambient), mColor(color), mLightDir(glm::normalize(-lightDir)), mStrength(strength)
{
	lookDir = glm::normalize(-lookDir);
	mHalfVector = (mLightDir + lookDir) / glm::length(mLightDir + lookDir);
}
DirectionalLight::DirectionalLight(const glm::vec4 &lightDir, const glm::vec4 &color, 
	float ambScale, glm::vec4 lookDir, float strength)
	: Light(ambScale * color), mColor(color), mLightDir(glm::normalize(-lightDir)), mStrength(strength)
{
	lookDir = glm::normalize(-lookDir);
	mHalfVector = (mLightDir + lookDir) / glm::length(mLightDir + lookDir);
}
void DirectionalLight::updateViewDir(const glm::vec4 &viewDir){
	mHalfVector = (mLightDir + viewDir) / glm::length(mLightDir + viewDir);
}
std::vector<float> DirectionalLight::getRaw() const {
	std::vector<float> vals = Light::getRaw();
	for (int i = 0; i < 4; ++i)
		vals.push_back(mColor[i]);
	for (int i = 0; i < 4; ++i)
		vals.push_back(mLightDir[i]);
	for (int i = 0; i < 4; ++i)
		vals.push_back(mHalfVector[i]);
	vals.push_back(mStrength);
	return vals;
}
