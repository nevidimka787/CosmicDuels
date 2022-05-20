#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 position;//position
uniform vec2 size;//size

uniform int text_size;
uniform int text_length;
uniform float scale;

out vec2 texture_position;
out vec2 pixel_position;

vec2 SetCurrentTexturePosition(vec2 textarea_pos)
{
    return (pixel_position - vec2(textarea_pos.x - text_size / 200.0f / abs(size.x) * text_length, textarea_pos.y * scale - text_size / 200.0f / abs(size.y / scale)))
    * vec2(size.x, -size.y / scale) / 4.0f * 25.0f / text_size;
}

void main()
{
    pixel_position = aPos * vec2(1.0f, scale);
    texture_position = SetCurrentTexturePosition(vec2(0.5f, 0.5f));
    gl_Position = vec4(pixel_position * size + position * vec2(1.0f, scale) + vec2(0.0f, 1.0f -  scale), 0.0f, 1.0f);
}