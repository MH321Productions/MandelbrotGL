package io.github.mh321productions.mandelbrot;

import android.os.Bundle;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.RadioGroup;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.common.collect.Lists;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    private MandelbrotView mandelbrotView;
    private ArrayList<RadioButton> colorButtons;
    private RadioGroup groupFirst, groupSec;
    private FloatingActionButton btnSave, btnReset;
    private MandelbrotControls controls = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //Get instances of the widgets
        mandelbrotView = findViewById(R.id.mandelbrotView);
        groupFirst = findViewById(R.id.rdbtnGroupFirst);
        groupSec = findViewById(R.id.rdbtnGroupSec);
        btnSave = findViewById(R.id.btnSave);
        //btnReset = findViewById(R.id.btnReset);
        colorButtons = Lists.newArrayList(
                findViewById(R.id.rdbtnFirstNone),
                findViewById(R.id.rdbtnFirstRed),
                findViewById(R.id.rdbtnFirstGreen),
                findViewById(R.id.rdbtnFirstBlue),
                findViewById(R.id.rdbtnSecNone),
                findViewById(R.id.rdbtnSecRed),
                findViewById(R.id.rdbtnSecGreen),
                findViewById(R.id.rdbtnSecBlue)
        );

        WindowInsetsControllerCompat windowInsetsController =
                ViewCompat.getWindowInsetsController(getWindow().getDecorView());
        if (windowInsetsController == null) {
            return;
        }
        // Configure the behavior of the hidden system bars
        windowInsetsController.setSystemBarsBehavior(
                WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
        );
        // Hide both the status bar and the navigation bar
        windowInsetsController.hide(WindowInsetsCompat.Type.systemBars());
    }

    public void setControls(MandelbrotControls controls) {
        this.controls = controls;
    }
}