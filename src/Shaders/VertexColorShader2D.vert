#ifndef NEW_GLSL
#define in attribute
#define out varying
#endif

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 0) uniform mat3 transformationProjectionMatrix;
#else
uniform highp mat3 transformationProjectionMatrix;
#endif

#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = 0) in highp vec3 position;
layout(location = 1) in lowp vec3 color;
#else
in highp vec3 position;
in lowp vec3 color;
#endif

out lowp vec3 interpolatedColor;

void main() {
    gl_Position.xywz = vec4(transformationProjectionMatrix*position, 0.0);
    interpolatedColor = color;
}
