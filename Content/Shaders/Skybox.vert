#version 330

out vec3 vDirection;

layout(std140) uniform Camera
{
    mat4 uView;
    mat4 uProjection;
};

void main()
{
    const vec2 triangle[3] = vec2[3](
        vec2(-1.0, -1.0), vec2(3.0, -1.0), vec2(-1.0, 3.0)
    );

    gl_Position = vec4(triangle[gl_VertexID], 1.0, 1.0);

    vDirection = mat3(transpose(uView)) * (inverse(uProjection) * gl_Position).xyz;
}