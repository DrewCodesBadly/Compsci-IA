shader_type canvas_item;

uniform float frequency;
uniform float seed;

void vertex() {
	// Called for every vertex the material is visible on.
}

float random (vec2 uv) {
	return fract(sin(dot(uv.xy,
		vec2(12.9898,78.233))) * 43758.5453123);
}

vec2 randvec(vec2 v) {
	float angle = random(v) * 2.0 * PI;
	return vec2(cos(angle), sin(angle));
}

void fragment() {
	vec2 v = UV * frequency;
	vec2 bl = trunc(v);
	vec2 frac = fract(v);
	vec2 tl = vec2(bl.x, bl.y + 1.0);
	vec2 tr = vec2(bl.x + 1.0, bl.y + 1.0);
	vec2 br = vec2(bl.x + 1.0, bl.y);
	
	bl = randvec(bl + vec2(seed));
	tl = randvec(tl + vec2(seed));
	tr = randvec(tr + vec2(seed));
	br = randvec(br + vec2(seed));
	
	float bl_dot = dot(bl, frac);
	float tl_dot = dot(tl, vec2(frac.x, frac.y - 1.0));
	float tr_dot = dot(tr, vec2(frac.x - 1.0, frac.y - 1.0));
	float br_dot = dot(br, vec2(frac.x - 1.0, frac.y));
	
	// smoothstep (can remove if needed)
	frac.x = frac.x * frac.x * (3.0 - (2.0 * frac.x));
	frac.y = frac.y * frac.y * (3.0 - (2.0 * frac.y));
	
	float bl_tl = ((1.0 - frac.y) * bl_dot) + (frac.y * tl_dot);
	float br_tr = ((1.0 - frac.y) * br_dot) + (frac.y * tr_dot);
	
	COLOR = vec4(vec3(((1.0 - frac.x) * bl_tl) + (frac.x * br_tr)), 1.0);
}
