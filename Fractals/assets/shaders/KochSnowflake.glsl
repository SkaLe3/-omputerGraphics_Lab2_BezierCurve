
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

// Returns unit vector in direction of specified angle
vec2 N(float angle)
{
  return vec2(sin(angle), cos(angle));
}

void main() 
{
  vec2 uv = (gl_FragCoord.xy - .5 * v_ScreenSize.xy) / v_ScreenSize.y; // Normalized screen coordinates



  // Apply zoom factor
  uv /= v_Zoom;

    // Apply panning
  uv += v_Center;



  uv *= 2.;
  vec3 col = vec3(0);

  // Start reflect around y-axis
  uv.x = abs(uv.x);
  // End reflect around y-axis

  // Move the fractal pattern upwards
  uv.y += tan((5. / 6.) * 3.1415) * 0.5;


  // Calculate normal vector for the first reflection
  vec2 n = N((5. / 6.) * 3.1415);
  float d = dot(uv - vec2(.5,0.), n);

  // Reflect the uv coordinates based on the normal vector
  uv -= n * max(0., d) * 2;

  // Calculate normal vector for the second reflection
  n = N((2. / 3.) * 3.1415);
  uv.x += .5;
  float scale = 1.0 / v_Zoom;

  // Iterative folding
  for (int i = 0; i < 40; i++)
  {
    uv *= 3.;
    scale *= 3.;
    uv.x -=1.5;
    uv.x = abs(uv.x);
    uv.x -= .5;
    uv -= n * min(0., dot(uv, n)) * 2.;
  }

  // Calculate the distance of the modified uv coordinates from a line segment
  d = length(uv - vec2(clamp(uv.x, -1., 1.), 0));

  // Add color based on the distance and screen size
  col += smoothstep(2. / v_ScreenSize.y, .0, d / scale);

  color = vec4(col, 1.0);
}
