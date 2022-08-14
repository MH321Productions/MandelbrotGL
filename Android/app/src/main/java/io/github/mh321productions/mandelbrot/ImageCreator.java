package io.github.mh321productions.mandelbrot;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Handler;

import java.io.File;
import java.nio.IntBuffer;

public class ImageCreator implements Runnable {

    private MainActivity main;

    public ImageCreator(MainActivity main) {
        this.main = main;
    }

    @Override
    public void run() {
        File f = new File(main.getFilesDir(), "images/Screenshot.png");

        Bitmap map = BitmapFactory.decodeFile(f.getPath());
        System.out.println("Bitmap finished");

        SaveFragment frag = SaveFragment.newInstance(map);
        main.startFragment(frag);
    }
}
