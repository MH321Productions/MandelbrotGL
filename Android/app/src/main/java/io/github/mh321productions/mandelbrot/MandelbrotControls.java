package io.github.mh321productions.mandelbrot;

import android.opengl.GLES20;

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

    private int width = 1920, height = 1080;

    private float posX = 0.0f, posY = 0.0f, scale = 1.0f;

    private int locPosWidthHeight = 0, locSize = 0, locColorItThres = 0;

    public MandelbrotControls(MainActivity main) {
        this.main = main;
    }

    /**
     * <strong>Only in GL render thread:</strong><br/>
     * Fetches all uniform locations for the values
     */
    public void getUniformLocations(int shader) {
        locPosWidthHeight = GLES20.glGetUniformLocation(0, "name");
    }

    /**
     * <strong>Only in GL render thread:</strong><br/>
     * Gives the calculated uniform values to OpenGL
     */
    public void transferUniformValues() {

    }

    public void setFirstColor(int firstColor) {
        this.firstColor = firstColor;
    }

    public void setSecondColor(int secondColor) {
        this.secondColor = secondColor;
    }

    public void setInvert(boolean invert) {
        this.invert = invert;
    }

    public void setIterations(int iterations) {
        this.iterations = iterations;
    }

    public void setThreshold(float threshold) {
        this.threshold = threshold;
    }

    public void setSize(int width, int height) {
        this.width = width;
        this.height = height;
    }

    public void setPos(float x, float y) {
        this.posX = x;
        this.posY = y;
    }

    public void setScale(float scale) {
        this.scale = scale;
    }

    public void move(float deltaX, float deltaY) {
        posX += deltaX;
        posY += deltaY;
    }

    public void changeScale(float multiplier) {
        scale *= multiplier;
    }
}
