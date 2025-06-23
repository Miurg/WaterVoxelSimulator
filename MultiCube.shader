shader_type mesh;

render_mode unshaded;

uniform storage_buffer positions;
uniform int cube_count = 1000;

void vertex() {
    int idx = INSTANCE_ID;
    if (idx < cube_count) {
        vec3 offset = vec3(
            positions[idx * 3 + 0],
            positions[idx * 3 + 1],
            positions[idx * 3 + 2]
        );
        VERTEX += offset;
    }
}