shader_type canvas_item;

uniform float seed;
uniform float freq;

void vertex() {
	// Called for every vertex the material is visible on.
}

float fade(float t) {
  return t*t*t*(t*(t*6.0 - 15.0) + 10.0);
}

vec2 randvec(float s) {
	float angle = sin(s / 15.234) * 234.63465;
	return vec2(cos(angle), sin(angle));
}

vec2 grad(vec2 p) {
	return randvec(seed + p.x + p.y);
}

float noise(vec2 p) {
  /* Calculate lattice points. */
  vec2 p0 = floor(p);
  vec2 p1 = p0 + vec2(1.0, 0.0);
  vec2 p2 = p0 + vec2(0.0, 1.0);
  vec2 p3 = p0 + vec2(1.0, 1.0);

  /* Look up gradients at lattice points. */
  vec2 g0 = grad(p0);
  vec2 g1 = grad(p1);
  vec2 g2 = grad(p2);
  vec2 g3 = grad(p3);

  float t0 = p.x - p0.x;
  float fade_t0 = fade(t0); /* Used for interpolation in horizontal direction */

  float t1 = p.y - p0.y;
  float fade_t1 = fade(t1); /* Used for interpolation in vertical direction. */

  /* Calculate dot products and interpolate.*/
  float p0p1 = (1.0 - fade_t0) * dot(g0, (p - p0)) + fade_t0 * dot(g1, (p - p1)); /* between upper two lattice points */
  float p2p3 = (1.0 - fade_t0) * dot(g2, (p - p2)) + fade_t0 * dot(g3, (p - p3)); /* between lower two lattice points */

  /* Calculate final result */
  return (1.0 - fade_t1) * p0p1 + fade_t1 * p2p3;
}

void fragment() {
	COLOR = vec4(vec3(noise(UV * freq)), 1.0);
}
