#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <vector>
#include <glm/glm.hpp>
#include "light.h"

/**
* A simple directional light class
*/
class DirectionalLight : public Light {
public:
	/**
	* Create the directional light giving a light color, ambient component
	* direction and viewing direction. The viewing direction and light
	* direction will be used to compute the half vector
	* Also, because the light direction when computing is with respect to the object
	* we flip the light direction vector
	* @param lightDir The direction the light is traveling in
	* @param color Light color
	* @param ambient Ambient color
	* @param lookDir The direction the camera is looking
	* @param strength Strength for specular highlights
	*/
	DirectionalLight(const glm::vec4 &lightDir, const glm::vec4 &color, 
		const glm::vec4 &ambient, glm::vec4 lookDir, float strength);
	/**
	* Use this constructor to set the ambient as some percentage of the color
	* Also, because the light direction when computing is with respect to the object
	* we flip the light direction vector
	* @param lightDir The direction the light is traveling in
	* @param color Light color
	* @param ambScale Value to scale the color by for the ambient component
	* @param lookDir The direction the camera is looking
	* @param strength Strength for specular highlights
	*/
	DirectionalLight(const glm::vec4 &lightDir, const glm::vec4 &color, 
		float ambScale, glm::vec4 lookDir, float strength);
	///Update the viewing direction to re-compute the half vector
	void updateViewDir(const glm::vec4 &viewDir);
	/**
	* Get a packed float array of the lighting information
	* for passing to shaders
	* @return Light information ordered like so:
	* ambient, color, direction, half vector, strength
	*/
	std::vector<float> getRaw() const override;

private:
	glm::vec4 mColor, mLightDir, mHalfVector;
	float mStrength;
};

#endif
