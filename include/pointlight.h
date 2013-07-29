#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <vector>
#include <glm/glm.hpp>
#include "light.h"

/**
* Class for a basic point light
*/
class PointLight : public Light {
public:
	/**
	* Create a point light at some position
	* @param pos The position for the light
	* @param color The light color
	* @param ambient The light ambient color
	* @param strength Strength for specular highlights
	* @param xAttenuation The various attenuation values
	*/
	PointLight(const glm::vec4 &pos, const glm::vec4 &color,
		const glm::vec4 &ambient, float strength, float constAttenuation,
		float linearAttenuation, float quadraticAttenuation);
	/**
	* Create a point light at some position but use a scaling of the light color
	* as the ambient color value
	* @param pos The position for the light
	* @param color The light color
	* @param ambient The light ambient color
	* @param strength Strength for specular highlights
	* @param xAttenuation The various attenuation values
	*/
	PointLight(const glm::vec4 &pos, const glm::vec4 &color,
		float ambScale, float strength, float constAttenuation,
		float linearAttenuation, float quadraticAttenuation);
	/**
	* Get a packed float array of the lighting information
	* for passing to shaders
	* @return Light information ordered like so:
	* ambient, color, position, strength, const attenuation,
	* linear attenuation, quadratic attenuation
	*/
	std::vector<float> getRaw() const override;

private:
	glm::vec4 mPos, mColor;
	float mStrength, mConstAtt, mLinearAtt, mQuadrAtt;
};

#endif