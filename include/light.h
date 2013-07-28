#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include <glm/glm.hpp>

/**
* A base light class, contains an ambient light component
*/
class Light {
public:
	Light(const glm::vec4 &ambient);
	///Check if the light is enabled
	bool enabled() const;
	///Change the enabled status
	void enabled(bool e);
	/**
	* Get a vector of floats of the lighting information
	* for passing to shaders
	* @return the ambient light compenents
	* TODO: How can i also pass the enabled field out?
	*/
	virtual std::vector<float> getRaw() const;

private:
	bool mEnabled;
	glm::vec4 ambient;
};

#endif
