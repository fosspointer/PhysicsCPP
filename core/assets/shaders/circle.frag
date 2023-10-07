uniform vec2 u_Resolution;
uniform vec2 u_Size;
uniform vec2 u_Position;
uniform vec2 u_Mouse;
uniform sampler2D u_Texture;
uniform float u_Smooth;

void main()
{
    vec2 size; float smooth; vec3 color; vec4 pixel;
    if(length(vec2(u_Mouse - u_Position)) < u_Size.y / 2.0)
    {
        size = u_Size;
        smooth = u_Smooth * 5.0;
        color = vec3(0.2, 0.6, 0.6);
        pixel = texture2D(u_Texture, gl_TexCoord[0].xy * 2.0 - 0.5);
    }
    else 
    {
        size = u_Size;
        smooth = u_Smooth;
        color = vec3(0.0f);
        pixel = texture2D(u_Texture, gl_TexCoord[0].xy);
    }


    vec2 p = vec2(u_Position.x - size.x / 2.0, u_Resolution.y - size.y / 2.0 - u_Position.y);
    vec2 uv = (gl_FragCoord.xy - p) / size * 2.0 - 1.0;
    uv.x *= size.x / size.y;
    

    float distance = 1.0 - length(uv);
    distance = smoothstep(0.0, smooth, distance);
    gl_FragColor = vec4(color + gl_Color.xyz * pixel.xyz, distance);
}