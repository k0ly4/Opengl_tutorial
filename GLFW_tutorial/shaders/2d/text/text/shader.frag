#version 330 core
////////////////
in vec2 texCoord;
out vec4 FragColor;
////Style---------------------
struct Style{
    vec4 color;
    float softness;
    float thickness;
};
///////////////
uniform Style style;
uniform Style outline;
uniform sampler2D glyph;
/////////////////
float smoothstep(Style s, float glyphShape){
     return smoothstep( s.thickness - s.softness,
                        s.thickness + s.softness,
                        glyphShape );

}
/////////////////
float smoothstepInvert(Style s,float glyphShape){
    return smoothstep(  1.f-s.thickness - s.softness,
                        1.f-s.thickness + s.softness,
                        glyphShape );
}

void main()
{    
    float a = texture(glyph, texCoord).r;

    float outlineFactor = smoothstep(outline,a);
    a = smoothstepInvert(style,a);

     //if (a < 0.5)
        //   discard;

    vec4 albedo = mix(outline.color,style.color,outlineFactor);

    FragColor =vec4(albedo.rgb, albedo.a*a);
}