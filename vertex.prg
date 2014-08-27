
varying vec3 tangent;
varying vec3 bitangent;
varying vec3 normal;

void main() 
{
    // texture coordinates
	gl_TexCoord[0] = gl_MultiTexCoord0;

	// tangent space frame
	tangent = gl_NormalMatrix * vec3(gl_MultiTexCoord1.st, 0);
	bitangent = gl_NormalMatrix * vec3(gl_MultiTexCoord2.st, 0);
	normal = gl_NormalMatrix * gl_Normal;
	
	// vertex position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
