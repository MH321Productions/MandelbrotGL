package io.github.mh321productions.mandelbrot;

import android.content.res.AssetManager;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.Log;

import com.google.common.io.CharStreams;

import java.io.ByteArrayOutputStream;
import java.io.IOError;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.CharBuffer;
import java.nio.IntBuffer;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MandelbrotRenderer implements GLSurfaceView.Renderer {

    private MandelbrotView view;
    private MainActivity main;
    private MandelbrotControls controls;
    private AssetManager assMan;

    //GL variables
    private int shaderId = 0, bufferId;


    public MandelbrotRenderer(MandelbrotView view, MainActivity main, AssetManager assMan) {
        this.view = view;
        this.main = main;
        this.assMan = assMan;
        controls = new MandelbrotControls(main);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        GLES20.glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

        //Load shader
        loadShader();

        controls.getUniformLocations(0);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        controls.setSize(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);

        controls.transferUniformValues();
    }

    private boolean loadShader() {
        try {
            //Open files
            InputStream inVertex = assMan.open("shader/Vertex.shader");
            InputStream inFragment = assMan.open("shader/Fragment.shader");

            //Read files into strings
            ByteArrayOutputStream bufStr = new ByteArrayOutputStream();
            byte[] buffer = new byte[1024];
            int len;

            //Vertex
            while ((len = inVertex.read(buffer)) != -1) bufStr.write(buffer, 0, len);
            String strVertex = bufStr.toString("UTF-8");
            inVertex.close();
            bufStr.reset();

            //Fragment
            while ((len = inFragment.read(buffer)) != -1) bufStr.write(buffer, 0, len);
            String strFragment = bufStr.toString("UTF-8");
            inFragment.close();
            bufStr.close();

            //Compile Shader
            int vertexId = compileShader(GLES20.GL_VERTEX_SHADER, strVertex);
            int fragementId = compileShader(GLES20.GL_FRAGMENT_SHADER, strFragment);
            if (vertexId == 0 || fragementId == 0) return false;

            //Create and link program
            shaderId = GLES20.glCreateProgram();
            GLES20.glAttachShader(shaderId, vertexId);
            GLES20.glAttachShader(shaderId, fragementId);
            GLES20.glLinkProgram(shaderId);
            GLES20.glValidateProgram(shaderId);

            //Delete shaders
            GLES20.glDeleteShader(vertexId);
            GLES20.glDeleteShader(fragementId);
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    private int compileShader(int type, String src) {
        int id = GLES20.glCreateShader(type);
        GLES20.glShaderSource(id, src);
        GLES20.glCompileShader(id);

        IntBuffer result = IntBuffer.allocate(1);
        GLES20.glGetShaderiv(id, GLES20.GL_COMPILE_STATUS, result);
        if (result.get() == 0) { //compile error
            String err = GLES20.glGetShaderInfoLog(id);
            Log.e("Mandelbrot/LoadShader", "Couldn't compile the " + (type == GLES20.GL_VERTEX_SHADER ? "vertex" : "fragment") + " shader: " + err, null);

            GLES20.glDeleteShader(id);
            return 0;
        }

        return id;
    }
}
