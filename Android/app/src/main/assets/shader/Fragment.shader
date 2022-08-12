#version 300 es
precision highp float;

const float oneThird = 1.0 / 3.0;
const float twoThird = 2.0 / 3.0;

uniform vec4 posWidthHeight;
uniform vec4 thresIterWindowSize;
uniform int colorMap;

//Functions
vec2 sqareImag(vec2 v) {
    return vec2(
        v.x * v.x - v.y * v.y,
        2.0 * v.x * v.y
    );
}

float iterate(vec2 coord, float maxSteps, float threshold) {
    vec2 temp = vec2(0.0, 0.0);

    for (float i = 0.0; i < maxSteps; i += 1.0) {
        temp = sqareImag(temp) + coord;
        if (length(temp) > threshold) return i/maxSteps;
    }

    return maxSteps/1.0;
}

vec4 getColorMap(float grayColor) {
    bool invert = colorMap / 100 != 0;
    int firstColor = (colorMap % 100) / 10;
    int secondColor = colorMap % 10;

    if (invert) grayColor = 1.0 - grayColor;

    float normal = grayColor;
    float mid = max((grayColor - oneThird) * 1.5, 0.0);
    float high = max((grayColor - twoThird) * 3.0, 0.0);

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
            g = 0.0;
            b = 0.0;
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
            r = 0.0;
            b = 0.0;
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
            r = 0.0;
            g = 0.0;
        }
    }

    return vec4(r, g, b, 1.0);

    //return vec4(grayColor, grayColor, grayColor, 1); //Temporär
}

out vec4 color;
void main() {
    float threshold = thresIterWindowSize.x;
    float maxSteps = thresIterWindowSize.y;
    vec2 windowSize = thresIterWindowSize.zw;
    vec2 size = posWidthHeight.zw;
    vec2 center = posWidthHeight.xy;
    vec2 offset = ((gl_FragCoord.xy / windowSize) - vec2(0.5, 0.5)) * 2.0;
    vec2 coord = center + (offset * size);


    /*float lerpValue = gl_FragCoord.y / 500.0f;
    color = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(0.2f, 0.2f, 0.2f, 0.0f), lerpValue);

    color = vec4(coord, 0, 1);
    if (color.x < 0.0 && color.y < 0.0) color.z = 1.0f;*/

    float grayColor = clamp(iterate(coord, maxSteps, threshold), 0.0, 1.0);

    color = getColorMap(grayColor);

    //color = vec4(0.5, 0.5, 0.0, 1.0);
}