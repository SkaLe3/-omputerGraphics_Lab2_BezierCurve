
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;

void main()
{
  gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

out vec4 color;
uniform vec2 u_ScreenSize;
uniform vec2 u_Center;
uniform float u_Zoom;
uniform int u_itr;

vec4 map_to_color(float t) {
    float r = 9.0 * (1.0 - t) * t * t * t;
    float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
    float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;

    return vec4(r, g, b, 1.0);
}

void main()
{
    vec2 z, c;
    c.x = u_ScreenSize.x/u_ScreenSize.y * (gl_FragCoord.x / u_ScreenSize.x - 0.5);
    c.y = (gl_FragCoord.y / u_ScreenSize.y - 0.5);

    c.x /= u_Zoom;
    c.y /= u_Zoom;

    c.x += u_Center.x;
    c.y += u_Center.y;

    int i;
    for(i = 0; i < u_itr; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
		float y = (z.y * z.x + z.x * z.y) + c.y;

		if((x * x + y * y) > 2.0) break;
		z.x = x;
		z.y = y;
    }

    float t = float(i) / float(u_itr);

    color = map_to_color(float(t));
}
