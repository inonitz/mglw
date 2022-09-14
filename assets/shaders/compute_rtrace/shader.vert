#version 450 core
layout(location = 0) in  vec4 vPos;
layout(location = 1) in  vec2 vTexCoord;
layout(location = 0) out vec2 texCoords;
layout(location = 1) out vec4 pos;


void main()
{
	gl_Position = pos = vPos;
	texCoords   = vTexCoord;
}