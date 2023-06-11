#version 130

struct Material {
  sampler2D diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

out vec4 frag_col;

in vec3 frag_3Dpos;
in vec3 vs_normal;
in vec2 vs_tex_coord;

uniform Material material;
uniform Light light_1;
uniform Light light_2;
uniform vec3 view_pos;

vec3 calculatePhong(Light light) {
  // Ambient
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, vs_tex_coord));

  vec3 light_dir = normalize(light.position - frag_3Dpos);

  // Diffuse
  float diff = max(dot(vs_normal, light_dir), 0.0);
  vec3 diffuse = light.diffuse * max(dot(vs_normal, light_dir), 0.0)
    * vec3(texture(material.diffuse, vs_tex_coord));

  // Specular
  vec3 view_dir = normalize(view_pos - frag_3Dpos);
  vec3 reflect_dir = reflect(-light_dir, vs_normal);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * material.specular;

  return ambient + diffuse + specular;
}

void main() {
  vec3 result = calculatePhong(light_1);
  result += calculatePhong(light_2);
  frag_col = vec4(result, 1.0);
}