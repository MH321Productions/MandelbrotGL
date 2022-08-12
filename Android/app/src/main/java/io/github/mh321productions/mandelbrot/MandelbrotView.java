package io.github.mh321productions.mandelbrot;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;

public class MandelbrotView extends GLSurfaceView {

    private final ScaleGestureDetector scaleDetector;
    private final GestureDetector moveDetector;
    private MandelbrotRenderer renderer;
    private MandelbrotControls controls;
    private AssetManager assMan;
    private MainActivity main;

    public MandelbrotView(Context context) {
        super(context);
        main = (context instanceof MainActivity) ? (MainActivity) context : null;
        assMan = context.getAssets();
        scaleDetector = new ScaleGestureDetector(context, new ScaleListener());
        moveDetector = new GestureDetector(context, new MoveListener());
        init();
    }

    public MandelbrotView(Context context, AttributeSet attrs) {
        super(context, attrs);
        main = (context instanceof MainActivity) ? (MainActivity) context : null;
        assMan = context.getAssets();
        scaleDetector = new ScaleGestureDetector(context, new ScaleListener());
        moveDetector = new GestureDetector(context, new MoveListener());
        init();
    }

    private void init() {
        setEGLContextClientVersion(3);
        setPreserveEGLContextOnPause(true);
        renderer = new MandelbrotRenderer(this, main, assMan);
        controls = renderer.controls;
        setRenderer(renderer);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        scaleDetector.onTouchEvent(event);
        if (event.getPointerCount() == 1) moveDetector.onTouchEvent(event);
        return true;
    }

    private class ScaleListener extends ScaleGestureDetector.SimpleOnScaleGestureListener {
        @Override
        public boolean onScale(ScaleGestureDetector detector) {
            controls.changeScale(detector.getScaleFactor());

            return true;
        }
    }

    private class MoveListener extends GestureDetector.SimpleOnGestureListener {
        @Override
        public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
            //renderer.touch(-distanceX, -distanceY);
            controls.move(distanceX, distanceY);
            return true;
        }
    }
}
