
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

out vec2 v_ScreenPos;
out vec2 v_ScreenSize;
out float v_ScreenRatio;
out vec2 v_Center;
out float v_Zoom;
flat out int v_itr;


uniform mat4 u_ViewProjection;

uniform float u_ScreenRatio;
uniform vec2 u_ScreenSize;
uniform vec2 u_Center;
uniform float u_Zoom;
uniform int u_itr;

void main()
{
  gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
  v_ScreenPos = gl_Position.xy;
  v_ScreenSize = u_ScreenSize;
  v_ScreenRatio = u_ScreenRatio;
  v_Center = u_Center;
  v_Zoom = u_Zoom;
  v_itr = u_itr;
}

#type fragment
#version 330 core

out vec4 color;
in vec2 v_ScreenPos;
in vec2 v_ScreenSize;
in float v_ScreenRatio;
in vec2 v_Center;
in float v_Zoom;
flat in int v_itr;

vec4 map_to_color(float t) {
    float r = 9.0 * (1.0 - t) * t * t * t;
    float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
    float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;

    return vec4(r, g, b, 1.0);
}

void main()
{
    vec2 z, c;
    c.x = v_ScreenRatio * (gl_FragCoord.x / v_ScreenSize.x - 0.5);
    c.y = (gl_FragCoord.y / v_ScreenSize.y - 0.5);

    c.x /= v_Zoom;
    c.y /= v_Zoom;

    c.x += v_Center.x;
    c.y += v_Center.y;

    int i;
    for(i = 0; i < v_itr; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
		float y = (z.y * z.x + z.x * z.y) + c.y;

		if((x * x + y * y) > 2.0) break;
		z.x = x;
		z.y = y;
    }

    float t = float(i) / float(v_itr);

    color = map_to_color(float(t));
}
