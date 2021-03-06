#version 460                                                          // GLSL v4.6 matched OpenGL 4.6

// Variable declarations may have at most one storage qualifier specified in front of the type
// in - the storage qualifier which linkage into a shader from a previous stage, variable is copied in
// layout - specify the input variables with 'location' metadata 
//          for configuring the vertex attributes on the CPU
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
// out - the storage qualifier which linkage out of a shader to a subsequent stage, variable is copied out
out vec4 vertexColor;

void main()
{
    // gl_Position is built-in variable in vertex language
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor;
}