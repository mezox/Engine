#pragma once

#include <vulkan/vulkan.hpp>

#include "Math/Vector2.h"
#include "Math/Vector3.h"

class Vertex
{
public:
    Engine::Math::Vector2<float> position;
    Engine::Math::Vector3<float> color;
    
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
    static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions()
    {
        std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions;
        attributeDescriptions[0].binding = 0;                           // The binding parameter tells Vulkan from which binding the per-vertex data comes
        attributeDescriptions[0].location = 0;                          // The location parameter references the location directive of the input in the vertex shader.
        attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;    // The format parameter implicitly defines the byte size of attribute data
        attributeDescriptions[0].offset = offsetof(Vertex, position);   // The offset parameter specifies the number of bytes since the start of the per-vertex data to read from.
        
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        return attributeDescriptions;
    }
};

