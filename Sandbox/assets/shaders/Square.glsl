#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position; 
layout(location = 1) in vec2 a_TextureCoord; 

out vec2 v_TextureCoord; 

uniform mat4 u_ModelMatrix; 
uniform mat4 u_ViewProjection;

void main()
{
	v_TextureCoord = a_TextureCoord;
	gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position, 1.0); 
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color; 

in vec2 v_TextureCoord; 

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TextureCoord);
}