#version 460

in vec2 TexCoords;
in vec4 vertexColor; 
out vec4 FragColor;


void main()
{    
    FragColor = vertexColor;
}
