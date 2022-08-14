package io.github.mh321productions.mandelbrot;

import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;

import java.io.File;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link SaveFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class SaveFragment extends Fragment {

    private static final String ARG_MAP = "pBitmap";
    private static final String uriProvider = "io.github.mh321productions.fileprovider";

    private Bitmap map;
    private ImageView img;
    private Button btnSave, btnShare, btnClose;
    private File f;

    public SaveFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param map The generated bitmap
     * @return A new instance of fragment SaveFragment.
     */
    public static SaveFragment newInstance(Bitmap map) {
        SaveFragment fragment = new SaveFragment();
        Bundle args = new Bundle();
        args.putParcelable(ARG_MAP, map);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            map = getArguments().getParcelable(ARG_MAP);
        }
        f = new File(getActivity().getFilesDir(), "images/Screenshot.png");
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View root = inflater.inflate(R.layout.fragment_save, container, false);

        img = root.findViewById(R.id.imgSave);
        img.setImageBitmap(map);

        btnSave = root.findViewById(R.id.btnSaveFile);
        btnSave.setOnClickListener(this::onSave);

        btnShare = root.findViewById(R.id.btnShare);
        btnShare.setOnClickListener(this::onShare);

        btnClose = root.findViewById(R.id.btnClose);
        btnClose.setOnClickListener(this::onClose);

        return root;
    }

    private void onSave(View v) {

    }

    private void onShare(View v) {
        Uri uri = MyFileProvider.getUriForFile(getContext(), uriProvider, f);
        //getContext().grantUriPermission("", uri, Intent.FLAG_GRANT_READ_URI_PERMISSION);

        Intent share = new Intent();
        share.setAction(Intent.ACTION_SEND);
        //share.setData(uri);
        share.putExtra(Intent.EXTRA_STREAM, uri);
        share.setFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
        //share.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        share.setType("image/png");
        startActivity(Intent.createChooser(share, getString(R.string.btn_share)));
    }

    private void onClose(View v) {
        MainActivity main = (MainActivity) getActivity();
        main.stopFragment(this);
    }
}