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
uniform sampler2D u_Textures[32];
uniform int u_Reset;
uniform vec3 u_Color;

#define T(U, l) texture(u_Textures[1], U, l)

#define I(M, V) T( ( (U-V) * inverse(M) + C) / 12., -15.) // Find color of seed pixel

vec4 blendColors(vec4 col, vec3  newcol) {
    float maxComponent = max(max(newcol.r, newcol.g),newcol.b);
    maxComponent = max(maxComponent, 0.0001);
    vec4 scaled = vec4(newcol, 1.) / maxComponent;
    vec4 result = col * scaled;

    return result;
}

void main() 
{
    vec2 C = vec2(5.5, 1);
    vec2 R = u_ScreenSize;
    vec2 U = 12. * gl_FragCoord.xy / u_ScreenSize - C; // Normalize pixel coordinates to texture space
  
    float k = mix(.93855, .9375, (R.x*R.y-512.*288.)/(1600.*1200.-512.*288.) ); //for the .01 stem

    color = u_Reset == 1 ? vec4(step(length(U + U-1.), 1.)) // Seed with a blob
    :   max(T( gl_FragCoord.xy / u_ScreenSize, 0.) *0.9,
        max(max(I(  mat2( .01,    0,    0, .16),   vec2(0, 0  )),  // Copy to stem
                I(  mat2( .85,  .04, -.04, .85),   vec2(0, 1.6))), // Copy to next row
            max(I(  mat2( .20, -.26,  .23, .22),   vec2(0, 1.6)),  // Copy to mirror right side
                I(  mat2(-.15,  .28,  .26, .24),   vec2(0,  .4)))  // Copy to left side
              ) / k);

    float maxComponent = max(max(u_Color.r, u_Color.g),u_Color.b);
    maxComponent = max(maxComponent, 0.0001);
    vec3 scaled = u_Color / maxComponent;
    color = color * vec4(scaled, 1.);
}
