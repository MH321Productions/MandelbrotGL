package io.github.mh321productions.mandelbrot;

import androidx.core.content.FileProvider;

public class MyFileProvider extends FileProvider {
    public MyFileProvider() {
        super(R.xml.files_path);
    }
}
