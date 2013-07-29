#include <vector>
#include <glm/glm.hpp>
#include "../include/light.h"
#include "../include/pointlight.h"

PointLight::PointLight(const glm::vec4 &pos, const glm::vec4 &color,
	const glm::vec4 &ambient, float strength, float constAttenuation,
	float linearAttenuation, float quadraticAttenuation)
	: Light(ambient), mPos(pos), mColor(color), mStrength(strength), mConstAtt(constAttenuation),
		mLinearAtt(linearAttenuation), mQuadrAtt(quadraticAttenuation)
{
}
PointLight::PointLight(const glm::vec4 &pos, const glm::vec4 &color,
	float ambScale, float strength, float constAttenuation,
	float linearAttenuation, float quadraticAttenuation)
	: Light(ambScale * color), mPos(pos), mColor(color), mStrength(strength), 
		mConstAtt(constAttenuation), mLinearAtt(linearAttenuation), mQuadrAtt(quadraticAttenuation)
{
}
std::vector<float> PointLight::getRaw() const {
	std::vector<float> vals = Light::getRaw();
	for (int i = 0; i < 4; ++i)
		vals.push_back(mColor[i]);
	for (int i = 0; i < 4; ++i)
		vals.push_back(mPos[i]);
	vals.push_back(mStrength);
	vals.push_back(mConstAtt);
	vals.push_back(mLinearAtt);
	vals.push_back(mQuadrAtt);
	return vals;
}
