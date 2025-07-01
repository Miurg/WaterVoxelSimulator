shader_type spatial;
render_mode unshaded;

varying vec4 instance_color;

void vertex()
{
    instance_color = INSTANCE_CUSTOM;
}

void fragment()
{
	if (instance_color.a < 0.01)
	{
        discard;
    }
    ALBEDO = instance_color.rgb;
}
