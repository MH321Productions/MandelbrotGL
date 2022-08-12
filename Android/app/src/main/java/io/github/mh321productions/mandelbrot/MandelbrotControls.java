package io.github.mh321productions.mandelbrot;

import android.opengl.GLES30;

/**
 * These variables are set by the UI
 * (events in {@link MainActivity})
 * and used by the renderer
 */
public class MandelbrotControls {

    private MainActivity main;

    private int firstColor = 0, secondColor = 0, colormap = 0;
    private boolean invert = false;

    private int iterations = 250;
    private float threshold = 2.5f;

    private int windowWidth = 1920, windowHeight = 1080;
    private float ratio = 16.0f/9.0f;

    private float posX = -0.75f, posY = 0.0f, width = 2.1f, height = width / ratio;

    private int locPosWidthHeight = 0, locThresItSize = 0, locColor = 0;

    public MandelbrotControls(MainActivity main) {
        this.main = main;
        main.setControls(this);
    }

    /**
     * <strong>Only in GL render thread:</strong><br/>
     * Fetches all uniform locations for the values
     */
    public void getUniformLocations(int shader) {
        locPosWidthHeight = GLES30.glGetUniformLocation(shader, "posWidthHeight");
        locThresItSize = GLES30.glGetUniformLocation(shader, "thresIterWindowSize");
        locColor = GLES30.glGetUniformLocation(shader, "colorMap");
    }

    /**
     * <strong>Only in GL render thread:</strong><br/>
     * Gives the calculated uniform values to OpenGL
     */
    public void transferUniformValues() {
        GLES30.glUniform4f(locThresItSize, threshold, iterations, windowWidth, windowHeight);
        GLES30.glUniform1i(locColor, colormap);
        GLES30.glUniform4f(locPosWidthHeight, posX, posY, width, height);
    }

    /**
     * Calculates the color map with the following signature: <br/>
     * <p style="text-align:center">
     *     123 <br/>
     *     ifs <br/>
     * </p>
     * i: invert digit <br/>
     * f: first color digit <br/>
     * s: second color digit
     */
    private void updateColorMap() {
        colormap = (secondColor % 4) + ((firstColor % 4) * 10) + (invert ? 100 : 0);
    }

    /**
     * Reset all values to standard
     */
    public void reset() {
        firstColor = 0;
        secondColor = 0;
        colormap = 0;
        invert = false;
        iterations = 250;
        threshold = 2.5f;
        posX = -0.75f;
        posY = 0;
        width = 2.1f;
        height = 2.1f / ratio;
    }

    public void setFirstColor(int firstColor) {
        this.firstColor = firstColor;
        updateColorMap();
    }

    public void setSecondColor(int secondColor) {
        this.secondColor = secondColor;
        updateColorMap();
    }

    public void setInvert(boolean invert) {
        this.invert = invert;
        updateColorMap();
    }

    public void setIterations(int iterations) {
        this.iterations = iterations;
    }

    public void setThreshold(float threshold) {
        this.threshold = threshold;
    }

    public void setWindowSize(int width, int height) {
        this.windowWidth = width;
        this.windowHeight = height;
        ratio = (float) width / (float) height;
        this.height = this.width / ratio;
    }

    public void setPos(float x, float y) {
        this.posX = x;
        this.posY = y;
    }

    public void setWidth(float width) {
        this.width = width;
    }

    public void move(float deltaX, float deltaY) {
        posX += deltaX * width / windowWidth;
        posY -= deltaY * height / windowHeight;
    }

    public void changeScale(float scalar) {
        width /= scalar;
        height /= scalar;
    }
}
