#version 460                                                          // GLSL v4.6 matched OpenGL 4.6

// Variable declarations may have at most one storage qualifier specified in front of the type
// in - the storage qualifier which linkage into a shader from a previous stage, variable is copied in
// layout - specify the input variables with 'location' metadata 
//          for configuring the vertex attributes on the CPU
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;
// out - the storage qualifier which linkage out of a shader to a subsequent stage, variable is copied out
out vec4 vertexColor;
out vec3 Normal;
out vec3 FragPos;

// uniform - the storage qualifier which means the value does not change accross the primitive being processed
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    // gl_Position is built-in variable in vertex language
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    vertexColor = aColor;
    Normal = mat3(transpose(inverse(model))) * aNormal;
}