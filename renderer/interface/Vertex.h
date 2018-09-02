#pragma once

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct CameraTransform
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};

class Vertex
{
public:
    glm::vec3 position;
    glm::vec3 color;
	glm::vec2 texCoords;

	bool operator==(const Vertex& other) const {
		return position == other.position && color == other.color && texCoords == other.texCoords;
	}
    
    /*
     * brief:   A vertex binding describes at which rate to load data from memory throughout the vertices.
     It specifies the number of bytes between data entries and whether to move to the next data
     entry after each vertex or after each instance.
     */
    static const vk::VertexInputBindingDescription getBindingDescription()
    {
        vk::VertexInputBindingDescription bindingDescription;
        bindingDescription.binding = 0;                                 // The binding parameter specifies the index of the binding in the array of bindings.
        bindingDescription.stride = sizeof(Vertex);                     // The stride parameter specifies the number of bytes from one entry to the next
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;
        return bindingDescription;
    }
    
    /*
     * brief:   An attribute description struct describes how to extract a vertex attribute from a chunk of vertex
     data originating from a binding description.
     */
    static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions()
    {
        std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions;
        attributeDescriptions[0].binding = 0;                           // The binding parameter tells Vulkan from which binding the per-vertex data comes
        attributeDescriptions[0].location = 0;                          // The location parameter references the location directive of the input in the vertex shader.
        attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat; // The format parameter implicitly defines the byte size of attribute data
        attributeDescriptions[0].offset = offsetof(Vertex, position);   // The offset parameter specifies the number of bytes since the start of the per-vertex data to read from.
        
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = vk::Format::eR32G32Sfloat;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoords);

        return attributeDescriptions;
    }
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoords) << 1);
		}
	};
}

