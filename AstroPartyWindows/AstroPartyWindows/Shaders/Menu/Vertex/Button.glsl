#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 position;//position
uniform vec2 size;//size

uniform int text_size;
uniform int text_length;

out vec2 texture_position;
out vec2 pixel_position;

void main()
{
    pixel_position = aPos;
    texture_position = (aPos - vec2(0.5f - text_size / 200.0f / abs(size.x) * text_length, 0.5f - text_size / 200.0f / abs(size.y)))
    * vec2(size.x, -size.y) / 4.0f * 25.0f / text_size;
    gl_Position = vec4(aPos * size + position, 0.0f, 1.0f);
}