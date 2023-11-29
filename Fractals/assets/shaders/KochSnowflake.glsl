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
uniform vec3 u_Color;

// Returns unit vector in direction of specified angle
vec2 N(float angle)
{
  return vec2(sin(angle), cos(angle));
}

void main() 
{
  vec2 uv = (gl_FragCoord.xy - .5 * u_ScreenSize.xy) / u_ScreenSize.y; // Normalized screen coordinates

  // Apply zoom factor
  uv /= u_Zoom *2.0;

    // Apply panning
  uv += u_Center;

  uv *= 2.;
  vec3 col = vec3(0);

  // Start reflect around y-axis
  uv.x = abs(uv.x);

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
  float scale = 1.0 / u_Zoom ;

  // Iterative folding
  for (int i = 0; i < u_itr; i++)
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
  col += smoothstep(2. / u_ScreenSize.y, .0, d / scale);
  col *= u_Color;
  color = vec4(col, 1.0);
}
