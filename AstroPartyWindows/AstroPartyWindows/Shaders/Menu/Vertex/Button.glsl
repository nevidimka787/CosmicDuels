#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 position;  // position
uniform vec2 size;      // size

uniform int text_size;
uniform int text_length;
uniform float scale;

out vec2 position_on_text_area; // one letter from 0 to 1
out vec2 pixel_position;    // position of the pixel in the button box
out vec2 wrap_size;

uniform float corners[256];
uniform int corners_count;

vec2 SetCurrentTexturePosition(vec2 textarea_pos)
{
    return (pixel_position - vec2(textarea_pos.x - text_size / 200.0f / abs(size.x) * text_length, textarea_pos.y * scale - text_size / 200.0f / abs(size.y / scale)))
    * vec2(size.x, -size.y / scale) / 4.0f * 25.0f / text_size;
}

void main()
{
    float cors[2];
    vec2 cor_farest = vec2(0.0, 0.0);
    for (int i = 0; i < corners_count * 2; ++i)
     {
        cors[i % 2] = corners[i];
        if (i % 2 != 1) continue;

        cor_farest.x = max(abs(cor_farest.x), abs(cors[0]));
        cor_farest.y = max(abs(cor_farest.y), abs(cors[1]));
    }

    vec2 wrap_local_size = cor_farest * 2.0f;

    wrap_size = wrap_local_size * size; // size of the rectangle
    vec2 wrap_pos = position; // center of the rectangle
    const float VERTEX_BUFFER_SIZE = 2.0f; // [-1:1]
    wrap_size /= VERTEX_BUFFER_SIZE; // transform wrap size to vertexbuffer

    const float TEXT_SCALER = 0.01f;
    
    pixel_position = aPos * wrap_local_size / 2.0f; // set pixel position to button edges
    position_on_text_area = pixel_position * size / TEXT_SCALER / text_size; // set scale of text area
    position_on_text_area.y = -position_on_text_area.y; // flip along Y axis
    position_on_text_area += vec2(text_length / 2.0, 0.5); // set position of left down corner
    
    vec2 new_pos = aPos; // position in the vertex buffer
    new_pos = new_pos * wrap_size + wrap_pos; // rtansform coordinates to wrap buffer, that has size and position
    new_pos = new_pos * vec2(1.0f, scale) + vec2(0.0f, 1.0f - scale); // move vertex buffer to virtual window

    gl_Position = vec4(new_pos, 0.0f, 1.0f);
}
