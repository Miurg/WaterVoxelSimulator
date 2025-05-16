shader_type spatial;
render_mode unshaded;

varying vec3 instance_color;


void vertex() {
    instance_color = INSTANCE_CUSTOM.rgb;
}

void fragment() {
    ALBEDO = instance_color;
}
