
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

uniform vec3 u_light_color;
uniform vec3 u_light_target;
uniform vec3 u_light_pos;

uniform vec3 u_ambient_light;

uniform bool u_has_light;

uniform float u_light_intensity;

void main()
{
	vec2 uv = v_uv;

	vec3 N = normalize(v_normal);
	vec3 D = u_light_target;
	vec3 L = -D;
	vec3 light = vec3(0.0);

	vec4 color = u_color;
	color *= texture2D( u_texture, uv );
	
	float NdotL = clamp(dot(L,N), 0.0, 1.0);

	light += NdotL * u_light_intensity * u_light_color + u_ambient_light; 

	color.xyz *= light;

	gl_FragColor = color;
}
