#shader vertex
#version 330 core

layout(location = 0) in vec2 position;

void main() {
    gl_Position = vec4(position, 0, 1);
}


#shader fragment
#version 330 core

precision highp float;

out vec4 color;
const float maxSteps = 250;
const float threshold = 2.5;
const float oneThird = 1.0 / 3.0;
const float twoThird = 2.0 / 3.0;

uniform vec4 centerWidthHeight;
uniform vec2 windowSize;
uniform int colorMap;

vec2 sqareImag(vec2 v) {
    return vec2(
        v.x * v.x - v.y * v.y,
        2 * v.x * v.y
    );
}

float iterate(vec2 coord) {
    vec2 temp = vec2(0.0, 0.0);

    for (int i = 0; i < maxSteps; i++) {
        temp = sqareImag(temp) + coord;
        if (length(temp) > threshold) return i/maxSteps;
    }

    return maxSteps;
}

vec4 getColorMap(float grayColor) {
    bool invert = colorMap / 100 != 0;
    int firstColor = (colorMap % 100) / 10;
    int secondColor = colorMap % 10;

    if (invert) grayColor = 1.0 - grayColor;

    float normal = grayColor;
    float mid = max((grayColor - oneThird) * 1.5, 0);
    float high = max((grayColor - twoThird) * 3, 0);

    float r, g, b;

    if (firstColor == 0) {
        r = normal;
        g = normal;
        b = normal;
    } else if (firstColor == 1) {
        r = normal;
        if (secondColor == 2) {
            g = mid;
            b = high;
        } else if (secondColor == 3) {
            g = high;
            b = mid;
        } else {
            g = 0;
            b = 0;
        }
    } else if (firstColor == 2) {
        g = normal;
        if (secondColor == 1) {
            r = mid;
            b = high;
        } else if (secondColor == 3) {
            r = high;
            b = mid;
        } else {
            r = 0;
            b = 0;
        }
    } else if (firstColor == 3) {
        b = normal;
        if (secondColor == 1) {
            r = mid;
            g = high;
        } else if (secondColor == 2) {
            r = high;
            g = mid;
        } else {
            r = 0;
            g = 0;
        }
    }

    return vec4(r, g, b, 1);

    //return vec4(grayColor, grayColor, grayColor, 1); //Temporär
}

void main() {
    vec2 size = centerWidthHeight.zw;
    vec2 center = centerWidthHeight.xy;
    vec2 offset = ((gl_FragCoord.xy / windowSize) - vec2(0.5, 0.5)) * 2;
    vec2 coord = center + (offset * size);

    /*float lerpValue = gl_FragCoord.y / 500.0f;
    color = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(0.2f, 0.2f, 0.2f, 0.0f), lerpValue);

    color = vec4(coord, 0, 1);
    if (color.x < 0.0 && color.y < 0.0) color.z = 1.0f;*/

    float grayColor = clamp(iterate(coord), 0, 1);

    color = getColorMap(grayColor);

    //color = vec4(0.5, 0.5, 0.0, 1.0);
};