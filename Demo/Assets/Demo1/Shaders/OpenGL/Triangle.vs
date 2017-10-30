#version 330 core
layout (location = 0) in vec3 POSITION;   // the position variable has attribute position 0
layout (location = 1) in vec3 COLOR; // the color variable has attribute position 1
  
out vec3 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = vec4(POSITION, 1.0);
    ourColor = COLOR; // set ourColor to the input color we got from the vertex data
}