#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout (constant_id = 0) const int COLOR_MODE = 0;

void main()
{
	if(COLOR_MODE == 0)
	{
		outColor = vec4(fragColor, 1.0);
	}
	else if(COLOR_MODE == 1)
	{
		outColor = texture(texSampler, fragTexCoord);
	}
	else
	{
		outColor = vec4(fragColor, 1.0) * texture(texSampler, fragTexCoord);
	}
}