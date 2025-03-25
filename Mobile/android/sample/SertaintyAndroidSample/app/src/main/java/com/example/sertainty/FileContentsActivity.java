package com.example.sertainty;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import org.Sertainty.File;
import org.Sertainty.VirtualFileInfo;
import org.Sertainty.VirtualFileInfoList;

public class FileContentsActivity extends AppCompatActivity {

    public static final String extraName = "com.example.sertainty.FileContentsActivity.parent";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_file_contents);

        Log.println(Log.INFO,"FileContents","Loaded");
        RecyclerView recyclerView = findViewById(R.id.rvVirtualFiles);

        Log.println(Log.INFO,"FileContents","Container Found");
        VirtualFileInfoList vfList = new VirtualFileInfoList();

        VirtualFileInfo root;
        if(getIntent().hasExtra(extraName))
        {
            vfList = AppState.getInstance().getOpenFile().getVirtualDirectory(getIntent().getStringExtra(extraName),0);
        }
        else
        {
            root = AppState.getInstance().getOpenFile().getVirtualFileInfo("/");
            vfList.add(root);
        }



        Log.println(Log.INFO,"FileContents","VirtualFileInfoList loaded");
        VirtualFileAdapter adapter = new VirtualFileAdapter(vfList);
        Log.println(Log.INFO,"FileContents","Adaptor initialized");

        recyclerView.setAdapter(adapter);
        Log.println(Log.INFO,"FileContents","Adapter attached");
        recyclerView.setLayoutManager(new LinearLayoutManager(this));
    }
}
