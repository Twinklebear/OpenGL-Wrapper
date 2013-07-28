#include <vector>
#include <glm/glm.hpp>
#include "../include/light.h"

Light::Light(const glm::vec4 &ambient) : mEnabled(true), ambient(ambient)
{}
bool Light::enabled() const {
	return mEnabled;
}
void Light::enabled(bool e){
	mEnabled = e;
}
std::vector<float> Light::getRaw() const {
	std::vector<float> vals;
	vals.reserve(4);
	for (int i = 0; i < 4; ++i)
		vals.push_back(ambient[i]);
	return vals;
}
