package io.github.mh321productions.mandelbrot;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.widget.Toast;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MandelbrotRenderer implements GLSurfaceView.Renderer {

    private MandelbrotView view;
    private MainActivity main;
    public MandelbrotControls controls;
    private AssetManager assMan;
    private boolean runSave = false;

    //GL variables
    private int shaderId = 0, vertexId = 0, indexId = 0, vaoId = 0;

    //Buffer data
    private static final float[] vertexData = new float[] {
        -1.0f, 1.0f,
        1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f
    };

    private static final byte[] indexData = new byte[] {
        0, 1, 2,
        1, 2, 3
    };

    public MandelbrotRenderer(MandelbrotView view, MainActivity main, AssetManager assMan) {
        this.view = view;
        this.main = main;
        this.assMan = assMan;
        controls = new MandelbrotControls(main);
    }

    public void startSave() {
        runSave = true;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        GLES30.glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

        //Create buffers
        IntBuffer b = IntBuffer.allocate(2);
        GLES30.glGenBuffers(2, b);
        vertexId = b.get();
        indexId = b.get();
        b.clear();

        //Fill vertex buffer
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, vertexId);
        GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, vertexData.length * 4, FloatBuffer.wrap(vertexData), GLES30.GL_STATIC_DRAW);

        //Fill index buffer
        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, indexId);
        GLES30.glBufferData(GLES30.GL_ELEMENT_ARRAY_BUFFER, indexData.length, ByteBuffer.wrap(indexData), GLES30.GL_STATIC_DRAW);

        //Generate vertex array object (vao)
        GLES30.glGenVertexArrays(1, b);
        vaoId = b.get();
        GLES30.glBindVertexArray(vaoId);

        //Bind vertex buffer to attrib array slot 0
        GLES30.glEnableVertexAttribArray(0);
        GLES30.glVertexAttribPointer(0, 2, GLES30.GL_FLOAT, false, 0, 0);

        //Load shader
        loadShader();

        //Use shader (once)
        GLES30.glUseProgram(shaderId);

        //Bind Index shader for later draw calls (once)
        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, indexId);

        controls.getUniformLocations(shaderId);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        controls.setWindowSize(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT);

        controls.transferUniformValues();

        //Draw call
        GLES30.glDrawElements(GLES30.GL_TRIANGLES, indexData.length, GLES30.GL_UNSIGNED_BYTE, 0);

        if (runSave) {
            File dir = new File(main.getFilesDir(), "images");
            dir.mkdir();
            File f = new File(dir, "Screenshot.png");
            boolean success = saveImage(controls.getWindowWidth(), controls.getWindowHeight(), f.getPath());

            if (!success) {
                Toast.makeText(main.getApplicationContext(), R.string.txt_save_failed, Toast.LENGTH_SHORT).show();
                runSave = false;
                return;
            }

            Handler h = new Handler(Looper.getMainLooper());
            h.post(new ImageCreator(main));

            runSave = false;
        }
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
            int vertexId = compileShader(GLES30.GL_VERTEX_SHADER, strVertex);
            int fragmentId = compileShader(GLES30.GL_FRAGMENT_SHADER, strFragment);
            if (vertexId == 0 || fragmentId == 0) return false;

            //Create and link program
            shaderId = GLES30.glCreateProgram();
            GLES30.glAttachShader(shaderId, vertexId);
            GLES30.glAttachShader(shaderId, fragmentId);
            GLES30.glLinkProgram(shaderId);
            GLES30.glValidateProgram(shaderId);

            //Delete shaders
            GLES30.glDeleteShader(vertexId);
            GLES30.glDeleteShader(fragmentId);
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    private int compileShader(int type, String src) {
        int id = GLES30.glCreateShader(type);
        GLES30.glShaderSource(id, src);
        GLES30.glCompileShader(id);

        IntBuffer result = IntBuffer.allocate(1);
        GLES30.glGetShaderiv(id, GLES30.GL_COMPILE_STATUS, result);
        if (result.get() == 0) { //compile error
            String err = GLES30.glGetShaderInfoLog(id);
            Log.e("Mandelbrot/LoadShader", "Couldn't compile the " + (type == GLES30.GL_VERTEX_SHADER ? "vertex" : "fragment") + " shader: " + err, null);

            GLES30.glDeleteShader(id);
            return 0;
        }

        return id;
    }

    private native boolean saveImage(int width, int height, String path);
}
