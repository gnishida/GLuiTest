
uniform sampler2D color_texture; 
uniform sampler2D normal_texture; 
varying vec3 tangent;
varying vec3 bitangent;
varying vec3 normal;

void main()
{
  	mat3 objToTangentSpace = mat3( tangent, bitangent, normal );
	  
    vec3 eye_vec = vec3(0,0,1);
	  
	vec3 normal_vec = objToTangentSpace * normalize(texture2D(normal_texture, gl_TexCoord[0].st).rgb * 2.0 - 1.0);  

	vec3 light_vec = normalize(vec3(0.0, 0.0, 1.5));  

	// Calculate the diffuse component
	float diffuse = max(dot(normal_vec, light_vec), 0.0);  // use abs to shade backside too

	// Calculate the specular component
	vec3 reflect_vec = reflect(-light_vec, normal_vec);
	float specular = pow(max(0, dot(reflect_vec, eye_vec)), 8);  // use abs to shade backside too

	// Get texture color
	vec3 tex_color = texture2D(color_texture, gl_TexCoord[0].st).rgb;  

	// Put it all together!
	vec3 color = tex_color * (diffuse * 0.5 + specular * 0.5);

	// Set the output color of our current pixel  
	gl_FragColor = vec4(color,1);
}
