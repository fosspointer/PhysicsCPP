uniform vec2 u_resolution;
uniform vec2 u_size;
uniform vec2 u_position;
uniform vec2 u_mouse;
uniform sampler2D u_texture;
uniform float u_smooth;

void main()
{
    vec2 size; float smooth; vec3 color; vec4 pixel;
    if(length(vec2(u_mouse - u_position)) < u_size.y / 2.0)
    {
        size = u_size;
        smooth = u_smooth * 3.5;
        color = vec3(0.2, 0.6, 0.6);
        pixel = texture2D(u_texture, gl_TexCoord[0].xy * 2.0 - 0.5);
    }
    else 
    {
        size = u_size;
        smooth = u_smooth;
        color = vec3(0.0f);
        pixel = texture2D(u_texture, gl_TexCoord[0].xy);
    }

    vec2 p = vec2(u_position.x - size.x / 2.0, u_resolution.y - size.y / 2.0 - u_position.y);
    vec2 uv = (gl_FragCoord.xy - p) / size * 2.0 - 1.0;
    uv.x *= size.x / size.y;    

    float distance = 1.0 - length(uv);
    distance = smoothstep(0.0, smooth, distance);
    gl_FragColor = vec4(color + gl_Color.xyz * pixel.xyz, distance);
}