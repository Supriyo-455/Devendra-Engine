#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (binding = 0) uniform sampler2D samp;

uniform mat4 mv_Matrix;
uniform mat4 proj_Matrix;

out vec4 varyingColor;
out vec2 tc;

mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);
mat4 buildScale(float x, float y, float z);

void main(void)
{
	gl_Position = proj_Matrix * mv_Matrix * vec4(position,1.0);
	varyingColor = vec4(0, 0, 0, 0);
	tc = texCoord;
}

// builds and returns a translation matrix
mat4 buildTranslate(float x, float y, float z)
{ 
	mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
					 0.0, 1.0, 0.0, 0.0,
					 0.0, 0.0, 1.0, 0.0,
					 x, y, z, 1.0 );
	return trans;
}

// builds and returns a matrix that performs a rotation around the X axis
mat4 buildRotateX(float rad)
{	
	mat4 xrot = mat4(1.0, 0.0, 0.0, 0.0,
					 0.0, cos(rad), -sin(rad), 0.0,
					 0.0, sin(rad), cos(rad), 0.0,
					 0.0, 0.0, 0.0, 1.0 );
	return xrot;
}

// builds and returns a matrix that performs a rotation around the Y axis
mat4 buildRotateY(float rad)
{ 
	mat4 yrot = mat4(cos(rad), 0.0, sin(rad), 0.0,
					 0.0, 1.0, 0.0, 0.0,
					 -sin(rad), 0.0, cos(rad), 0.0,
					 0.0, 0.0, 0.0, 1.0 );
	return yrot;
}

// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float rad)
{ 
	mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
					 sin(rad), cos(rad), 0.0, 0.0,
					 0.0, 0.0, 1.0, 0.0,
					 0.0, 0.0, 0.0, 1.0 );
	return zrot;
}

// builds and returns a scale matrix
mat4 buildScale(float x, float y, float z)
{ 
	mat4 scale = mat4(x, 0.0, 0.0, 0.0,
					 0.0, y, 0.0, 0.0,
					 0.0, 0.0, z, 0.0,
					 0.0, 0.0, 0.0, 1.0 );
	return scale;
}
