package io.github.mh321productions.mandelbrot;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.content.res.loader.ResourcesProvider;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.preference.PreferenceManager;
import android.view.View;
import android.view.animation.AccelerateDecelerateInterpolator;
import android.view.animation.DecelerateInterpolator;
import android.widget.Adapter;
import android.widget.CheckBox;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;

import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.android.material.floatingactionbutton.FloatingActionButton;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Locale;

public class MainActivity extends AppCompatActivity implements SeekBar.OnSeekBarChangeListener {

    private MandelbrotView viewMandelbrot;
    private BottomNavigationView viewOptions;
    private ArrayList<RadioButton> colorButtons;
    private RadioGroup groupFirst, groupSec;
    private FloatingActionButton btnSave, btnReset, btnOptions, btnLang;
    private CheckBox btnInvert;
    private TextView txtIterate, txtThreshold;
    private SeekBar sbIterate, sbThreshold;
    private MandelbrotControls controls = null;
    private AnimatorSet currentSet = null;
    private int currentLanguage = 0;

    private static final ArrayList<String> supportedLanguages;
    private static final ArrayList<Integer> flagIds;

    static {
        supportedLanguages = new ArrayList<>(3);
        Collections.addAll(supportedLanguages, "en", "de", "fr");

        flagIds = new ArrayList<>(3);
        Collections.addAll(flagIds,
                R.drawable.ic_flag_en,
                R.drawable.ic_flag_de,
                R.drawable.ic_flag_fr
        );
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //Get instances of the widgets
        viewMandelbrot = findViewById(R.id.mandelbrotView);
        viewOptions = findViewById(R.id.viewOptions);
        groupFirst = findViewById(R.id.rdbtnGroupFirst);
        groupSec = findViewById(R.id.rdbtnGroupSec);

        btnSave = findViewById(R.id.btnSave);
        btnSave.setOnClickListener(this::onSave);

        btnReset = findViewById(R.id.btnReset);
        btnReset.setOnClickListener(this::onReset);

        btnOptions = findViewById(R.id.btnOptions);
        btnOptions.setOnClickListener(this::onOptions);

        btnInvert = findViewById(R.id.btnInvert);
        btnInvert.setOnClickListener(this::onInvert);

        btnLang = findViewById(R.id.btnLang);
        btnLang.setOnClickListener(this::onLang);
        btnLang.setImageResource(flagIds.get(currentLanguage));

        txtThreshold = findViewById(R.id.txtThreshold);
        txtThreshold.setText(getString(R.string.txt_threshold_en, 2.5));

        txtIterate = findViewById(R.id.txtIterations);
        txtIterate.setText(getString(R.string.txt_iterations_en, 250));

        sbThreshold = findViewById(R.id.sbThreshold);
        sbThreshold.setOnSeekBarChangeListener(this);

        sbIterate = findViewById(R.id.sbIterations);
        sbIterate.setOnSeekBarChangeListener(this);

        colorButtons = new ArrayList<>(8);
        Collections.addAll(colorButtons,
                findViewById(R.id.rdbtnFirstNone),
                findViewById(R.id.rdbtnFirstRed),
                findViewById(R.id.rdbtnFirstGreen),
                findViewById(R.id.rdbtnFirstBlue),
                findViewById(R.id.rdbtnSecNone),
                findViewById(R.id.rdbtnSecRed),
                findViewById(R.id.rdbtnSecGreen),
                findViewById(R.id.rdbtnSecBlue)
        );
        for (RadioButton b: colorButtons) b.setOnClickListener(this::onColor);

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

    @Override
    protected void onPostCreate(@Nullable Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);

        Handler handler = new Handler(Looper.getMainLooper());
        handler.postDelayed(() -> {
            btnOptions.setEnabled(true);
            btnOptions.performClick();
        }, 250);
    }

    @Override
    protected void attachBaseContext(Context newBase) {
        super.attachBaseContext(updateContextLocale(newBase));
    }

    private Context updateContextLocale(Context context) {
        String lang = PreferenceManager.getDefaultSharedPreferences(context).getString("pref_key_language", "en");
        currentLanguage = supportedLanguages.indexOf(lang);
        System.out.println("Language: " + currentLanguage);
        Locale l = new Locale(lang);
        Locale.setDefault(l);
        Configuration config = context.getResources().getConfiguration();
        config.setLocale(l);
        return context.createConfigurationContext(config);
    }

    public void setControls(MandelbrotControls controls) {
        this.controls = controls;
    }

    //Listener methods
    private void onInvert(View v) {
        if (controls != null) controls.setInvert(btnInvert.isChecked());
    }

    private void onSave(View v) {
        //TODO: Implement save


        Intent i = new Intent(Intent.ACTION_APPLICATION_PREFERENCES);
        startActivity(i);
    }

    private void onLang(View v) {
        currentLanguage++;
        if (currentLanguage == 3) currentLanguage = 0;

        SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(this).edit();
        editor.putString("pref_key_language", supportedLanguages.get(currentLanguage));
        editor.apply();

        recreate();
    }

    private void onReset(View v) {
        if (controls != null) controls.reset();
        colorButtons.get(0).setChecked(true);
        onColor(colorButtons.get(0));
        btnInvert.setChecked(false);
        sbIterate.setProgress(250, true);
        sbThreshold.setProgress(250, true);
    }

    private void onColor(View v) {
        int index = colorButtons.indexOf(v);
        System.out.println(index);
        if (index != -1) {
            //Process indices and usability of the other buttons
            if (index == 0) {
                colorButtons.get(4).setChecked(true);
                for (RadioButton b: colorButtons.subList(4, 8)) b.setEnabled(false);
            } else if (index < 4) {
                for (RadioButton b: colorButtons.subList(4, 8)) b.setEnabled(true);
                colorButtons.get(index + 4).setEnabled(false);
                if (colorButtons.get(index + 4).isChecked()) colorButtons.get(4).setChecked(true);
            }

            //Set the colors in controls
            if (controls != null) {
                if (index < 4) controls.setFirstColor(index);
                else controls.setSecondColor(index % 4);
            }
        }
    }

    private void onOptions(View v) {
        if (currentSet != null) currentSet.cancel();

        int targetVisibility, targetTranslation;
        if (viewOptions.getVisibility() == View.VISIBLE) {
            targetVisibility = View.INVISIBLE;
            targetTranslation = viewOptions.getHeight();
        } else {
            viewOptions.setVisibility(View.VISIBLE);
            targetVisibility = View.VISIBLE;
            targetTranslation = 0;
        }

        AnimatorSet set = new AnimatorSet();
        set.play(ObjectAnimator.ofFloat(viewOptions, View.TRANSLATION_Y, targetTranslation));
        set.setDuration(1000);
        set.setInterpolator(new DecelerateInterpolator());
        set.addListener(new AnimatorListenerAdapter() {
            @Override
            public void onAnimationCancel(Animator animation) {
                viewOptions.setVisibility(targetVisibility);
                viewOptions.setTranslationY(targetTranslation);
                currentSet = null;
            }

            @Override
            public void onAnimationEnd(Animator animation) {
                viewOptions.setVisibility(targetVisibility);
                currentSet = null;
            }
        });
        set.start();
        currentSet = set;
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        if (seekBar == sbIterate) {
            txtIterate.setText(getString(R.string.txt_iterations_en, progress));
            if (controls != null) controls.setIterations(progress);
        } else if (seekBar == sbThreshold) {
            txtThreshold.setText(getString(R.string.txt_threshold_en, progress / 100.0));
            if (controls != null) controls.setThreshold(progress / 100.0f);
        }
    }

    //unused, have to implement them
    public void onStartTrackingTouch(SeekBar seekBar) {}
    public void onStopTrackingTouch(SeekBar seekBar) {}
}