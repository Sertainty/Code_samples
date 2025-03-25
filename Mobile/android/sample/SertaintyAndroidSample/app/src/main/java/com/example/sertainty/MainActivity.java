package com.example.sertainty;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import org.Sertainty.ByteArray;
import org.Sertainty.File;
import org.Sertainty.Id;
import org.Sertainty.Sys;
import org.Sertainty.VirtualFile;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private String root(){
        return getExternalFilesDir(null).getPath() + "/Sertainty/examples/";
    }
    private String idXmlSpec(){
        return root() + "sampleid.xml";
    }
    private String idFileSpec(){
        return root() + "sampleid.iic";
    }
    private String uxpFileSpec(){
        return root() + "sample.uxp";
    }
    private String dataPdfSpec(){
        return root() + "data.pdf";
    }
    private String dataPdf2Spec() {
        return root() + "data2.pdf";
    }
    private String copy1Spec(){
        return root() + "copy1.pdf";
    }
    private String copy2Spec(){
        return root() + "copy2.pdf";
    }

    private static final int INSTALL_LICENSE = 850;
    private static final int OPEN_FILE = 31;
    
    private int colIdFailure;
    private int colIdSuccess;

    private TextView libStatus, createIdStatus, createFileStatus, compareContentsStatus;
    private Button btnRunSample, btnOpenFile, btnReset, btnInstallLicense;


    private boolean checkPermissions()
    {
        String[] appPermissions = new String[]{
                Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.INTERNET
        };

        List<String> missingPermissions = new ArrayList<String>();

        for (String permission: appPermissions) {
            if(ContextCompat.checkSelfPermission(this, permission)
                    != PackageManager.PERMISSION_GRANTED){
                missingPermissions.add(permission);
            }
        }

        if (missingPermissions.size() > 0) {
            String [] requestPermissions = new String[missingPermissions.size()];
            requestPermissions = missingPermissions.toArray(requestPermissions);
            requestPermissions(requestPermissions,0);
        }

        return missingPermissions.size() == 0;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        boolean granted = true;
        StringBuilder message = new StringBuilder();
        for(int i = 0; i < permissions.length; i++)
        {
            String permission = permissions[i];
            int grantResult = grantResults[i];

            if (permission.compareTo(Manifest.permission.READ_EXTERNAL_STORAGE) == 0)
            {
                message.append("READ_EXTERNAL_STORAGE: ");
            }
            else if (permission.compareTo(Manifest.permission.WRITE_EXTERNAL_STORAGE) == 0)
            {
                message.append("WRITE_EXTERNAL_STORAGE: ");
            }
            else
            {
                message.append("INTERNET: ");
            }

            if (grantResult == PackageManager.PERMISSION_GRANTED)
            {
                message.append("Granted");
            }
            else
            {
                message.append("Denied");
                granted = false;
            }
            if(i < permission.length() - 1)
            {
                message.append("\n");
            }
        }

        if (granted)
        {
            initializeLicense();
        }

        Toast.makeText(this,message.toString(),Toast.LENGTH_SHORT).show();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if(resultCode == RESULT_OK) {
            Log.println(Log.INFO, "ActivityResult", data.getData().getPath());
            switch (requestCode)
            {
                case INSTALL_LICENSE:
                {
                    Log.println(Log.INFO,"ActivityResult","Install License");

                    String licenseFile = data.getData().getPath();
                    if(licenseFile.endsWith(".lic"))
                    {
                        try {
                            java.io.File internalCopy = new java.io.File(getFilesDir(), "license.lic");
                            java.io.File license = new java.io.File(licenseFile);

                            ByteArray inlic = new ByteArray();
                            InputStream in = getContentResolver().openInputStream(data.getData());
                            byte[] buf = new byte[1024];
                            int len;
                            while((len = in.read(buf)) > 0)
                            {
                                inlic.appendData(Arrays.copyOf(buf,len));
                            }

                            Sys.CallStatus callStatus = new Sys.CallStatus();
                            Sys.fileWriteAll(callStatus, internalCopy.getAbsolutePath(), inlic.getData(), 1);
                            if (callStatus.hasError()) {
                                Log.println(Log.ERROR, "Copy License: Write", callStatus.getErrorMessage());
                                throw new IOException();
                            }

                            licenseFile = internalCopy.getAbsolutePath();
                            Log.println(Log.INFO, "InstallLicense", licenseFile);
                            ByteArray errBuffer = new ByteArray();
                            if (Sys.installLicense(errBuffer, licenseFile)) {
                                Log.println(Log.INFO, "InstallLicense", "License installed successfully.");
                                Toast.makeText(this, "License Installed Successfully", Toast.LENGTH_LONG);
                                finish();
                                startActivity(getIntent());
                            } else {
                                Log.println(Log.ERROR, "InstallLicense", errBuffer.getString());
                                Toast.makeText(this, "Unable to install license: " + errBuffer.getString(), Toast.LENGTH_LONG);
                            }
                        }
                        catch (IOException licioex)
                        {
                            Log.println(Log.ERROR,"InstallLicense",licioex.getMessage());
                            licioex.printStackTrace();
                        }
                    }
                    else
                    {
                        Log.println(Log.ERROR, "InstallLicense","Invalid file type selected.");
                        Toast.makeText(this,"Invalid file type selected.",Toast.LENGTH_LONG);
                    }
                    break;
                }
                case OPEN_FILE:
                {
                    Log.println(Log.INFO,"ActivityResult","Open Uxp");
                    break;
                }
            }

        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        createIdStatus = findViewById(R.id.createIdStatus);
        createFileStatus = findViewById(R.id.createFileStatus);
        compareContentsStatus = findViewById(R.id.compareContentsStatus);

        colIdFailure = ContextCompat.getColor(getApplicationContext(),R.color.colorFailure);
        colIdSuccess = ContextCompat.getColor(getApplicationContext(),R.color.colorSuccess);

        btnRunSample = findViewById(R.id.btnRunSample);
        btnOpenFile = findViewById(R.id.btnOpenFile);
        btnReset = findViewById(R.id.btnReset);
        btnInstallLicense = findViewById(R.id.btnInstallLicense);

        final Intent getFileIntent = new Intent(Intent.ACTION_GET_CONTENT);
        getFileIntent.setType("*/*");

        if (checkPermissions()) {
            initializeLicense();
        }

        btnRunSample.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                createNewId();
            }
        });

        btnOpenFile.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                AppState.getInstance().releaseOpenFile();
                Intent openFileIntent = new Intent(MainActivity.this,OpenFileActivity.class);
                startActivity(openFileIntent);
            }
        });

        btnReset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                reloadUI();
            }
        });

        btnInstallLicense.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivityForResult(getFileIntent,INSTALL_LICENSE);
            }
        });

        reloadUI();
    }

    private void initializeLicense() {
        if(installAssets()) {
            libStatus = findViewById(R.id.librartStatusTxt);
            ByteArray errBuffer = new ByteArray();
            if (!AppState.getInstance().getInitialized()) {

                Log.println(Log.INFO, "Library", "Not initialized. Initializing.");
                String appRoot = getFilesDir().getAbsolutePath();
                Log.println(Log.INFO, "AppRoot", appRoot);


                if (!Sys.initLibrary(errBuffer, "SampleJNI", "SampleJNI", "1.0")) {
                    libStatus.setText(getResources().getText(R.string.library_failed));
                    libStatus.setTextColor(colIdFailure);
                    AppState.getInstance().setInitialized(false);
                    Log.println(Log.ERROR, "Library", errBuffer.getString());
                } else {
                    AppState.getInstance().setInitialized(true);
                    AppState.getInstance().setContext(getApplicationContext());
                    Log.println(Log.INFO, "Library", "Initialized.");
                }

            } else {
                Log.println(Log.INFO, "Library", "Pre initialized.");
            }
        }
    }

    private void reloadUI() {
        if(AppState.getInstance().getInitialized())
        {
            libStatus.setText(getResources().getText(R.string.library_initialized));
            libStatus.setTextColor(colIdSuccess);

        }

        java.io.File uxp = new java.io.File(uxpFileSpec());

        btnRunSample.setVisibility(AppState.getInstance().getInitialized()? View.VISIBLE:View.INVISIBLE);

        btnOpenFile.setVisibility(AppState.getInstance().getInitialized() && uxp.exists()? View.VISIBLE:View.INVISIBLE);

        createIdStatus.setText("");
        createFileStatus.setText("");
        compareContentsStatus.setText("");

    }

    private void createNewId()
    {
        Sys.CallStatus callStatus = new Sys.CallStatus();

        ByteArray buffer = new ByteArray();

        Sys.fileReadAll(callStatus,idXmlSpec(),buffer);

        String message;
        if(callStatus.hasError())
        {
            message = "Error reading file: " + callStatus.getErrorMessage();
            Toast.makeText(getApplicationContext(),message,Toast.LENGTH_SHORT);
            Log.println(Log.ERROR,"CreateNewId",message);
            createIdStatus.setText(R.string.failure);
            createIdStatus.setTextColor(colIdFailure);
        }
        else
        {
            message = "XML file read";
            Log.println(Log.INFO, "CreateNewId",message);

            Id.publishToFile(callStatus,idFileSpec(),buffer.getString(),1);

            if(callStatus.hasError())
            {
                message = "Error creating ID: " + callStatus.getErrorMessage();
                Toast.makeText(getApplicationContext(),message,Toast.LENGTH_SHORT);
                Log.println(Log.ERROR, "CreateNewId",message);
                createIdStatus.setText(R.string.failure);
                createIdStatus.setTextColor(colIdFailure);
            }
            else
            {
                message = "ID created";
                Log.println(Log.INFO, "CreateNewId",message);
                createIdStatus.setText(R.string.success);
                createIdStatus.setTextColor(colIdSuccess);

                createNewFile();
            }
        }
    }

    private void createNewFile()
    {
        File file = File.create(uxpFileSpec(), idFileSpec(),3,1,0);

        Sys.CallStatus callStatus = file.getCallStatus();

        String message;

        if(callStatus.hasError())
        {
            message = "Error creating Data: " + callStatus.getErrorMessage();
            Toast.makeText(getApplicationContext(),message, Toast.LENGTH_SHORT);
            Log.println(Log.ERROR, "CreateNewFile",message);

            createFileStatus.setText(R.string.failure);
            createFileStatus.setTextColor(colIdFailure);
        }
        else
        {
            message = "Data file created";
            Toast.makeText(getApplicationContext(),message,Toast.LENGTH_SHORT);
            Log.println(Log.INFO, "CreateNewFile",message);

            file.addVirtualFile("data.pdf",dataPdfSpec(),-1,-1, 8);

            if(callStatus.hasError())
            {
                message = "Error creating virtual file: " + callStatus.getErrorMessage();
                Toast.makeText(getApplicationContext(),message, Toast.LENGTH_SHORT);
                Log.println(Log.ERROR, "CreateNewFile",message);

                createFileStatus.setText(R.string.failure);
                createFileStatus.setTextColor(colIdFailure);
            }
            else
            {
                message = "Virtual file data.pdf created";
                Toast.makeText(getApplicationContext(),message,Toast.LENGTH_SHORT);
                Log.println(Log.INFO, "CreateNewFile",message);

                file.addVirtualFile("data2.pdf",dataPdf2Spec(),-1,-1, 8);

                if(callStatus.hasError())
                {
                    message = "Error creating virtual file: " + callStatus.getErrorMessage();
                    Toast.makeText(getApplicationContext(),message, Toast.LENGTH_SHORT);
                    Log.println(Log.ERROR, "CreateNewFile",message);

                    createFileStatus.setText(R.string.failure);
                    createFileStatus.setTextColor(colIdFailure);
                }
                else
                {
                    message = "Virtual file data.pdf created";
                    Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT);
                    Log.println(Log.INFO, "CreateNewFile", message);



                    VirtualFile fileHandle = file.openVirtualFile("data.pdf",1);

                    if(callStatus.hasError())
                    {
                        message = "Error opening virtual file: " + callStatus.getErrorMessage();
                        Toast.makeText(getApplicationContext(),message, Toast.LENGTH_SHORT);
                        Log.println(Log.ERROR, "CreateNewFile",message);

                        createFileStatus.setText(R.string.failure);
                        createFileStatus.setTextColor(colIdFailure);
                    }
                    else
                    {
                        createFileStatus.setText(R.string.success);
                        createFileStatus.setTextColor(colIdSuccess);

                        message = "Virtual file opened";
                        Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT);
                        Log.println(Log.INFO, "CreateNewFile", message);

                        java.io.File fp = new java.io.File(copy1Spec());
                        if(fp.exists()) fp.delete();
                        try {
                            fp.createNewFile();

                            OutputStream os = new FileOutputStream(fp);

                            ByteArray buffer = new ByteArray();
                            while(fileHandle.read(buffer,10000) > 0)
                            {
                                os.write(buffer.getData());
                            }

                            os.close();

                            fileHandle.close();

                            if (file.compareExternalFile("data.pdf",copy1Spec()))
                            {
                                message = "Comparison of data.pdf to copy1.pdf: successful";
                                Toast.makeText(getApplicationContext(),message, Toast.LENGTH_SHORT);
                                Log.println(Log.INFO, "CreateNewFile",message);

                                compareContentsStatus.setText(R.string.success);
                                compareContentsStatus.setTextColor(colIdSuccess);
                            }
                            else
                            {
                                message = "Comparison of data.pdf to copy1.pdf: failed";
                                Toast.makeText(getApplicationContext(),message, Toast.LENGTH_SHORT);
                                Log.println(Log.ERROR, "CreateNewFile",message);

                                compareContentsStatus.setText(R.string.failure);
                                compareContentsStatus.setTextColor(colIdFailure);
                            }
                        }
                        catch (IOException ioe)
                        {
                            message = "File IO Error: " + ioe.getMessage();
                            Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT);
                            Log.println(Log.ERROR, "CreateNewFile", message);

                            compareContentsStatus.setText(R.string.failure);
                            compareContentsStatus.setTextColor(colIdFailure);
                        }
                    }
                }
            }
        }

        if (file.isOpen())
        {
            file.close();
        }

        java.io.File uxp = new java.io.File(uxpFileSpec());

        btnRunSample.setVisibility(AppState.getInstance().getInitialized()? View.VISIBLE:View.INVISIBLE);

        btnOpenFile.setVisibility(AppState.getInstance().getInitialized() && uxp.exists()? View.VISIBLE:View.INVISIBLE);
    }

    private boolean installAssets()
    {
        java.io.File appRoot = new java.io.File(getFilesDir(),"Sertainty");
        java.io.File boot_ini = new java.io.File(appRoot, "boot.ini");
        java.io.File home = new java.io.File(appRoot, "home");

        if(!appRoot.exists()) {
            appRoot.mkdir();
        }
        if(appRoot.exists()) {
            try {
                if(!boot_ini.exists()) {
                    extractAsset(appRoot, "Sertainty", "/boot.ini");
                }
                if (!home.exists() && home.mkdir()) {
                    String parentDirName = "Sertainty/home";
                    String[] assetList = getAssets().list(parentDirName);
                    for (String assetName : assetList) {

                        extractAsset(home, parentDirName, "/" + assetName);
                    }
                }
            } catch (java.io.IOException ioex) {
                Log.println(Log.ERROR, "Install Assets", ioex.getMessage());
                ioex.printStackTrace();
            }
        }

        java.io.File exampleRoot = new java.io.File(root());
        if (!exampleRoot.exists()) {
            try {
                if (exampleRoot.mkdirs()) {
                    try {
                        extractAsset(exampleRoot, "Sertainty/example", "/data.pdf");
                        extractAsset(exampleRoot, "Sertainty/example", "/data2.pdf");
                        extractAsset(exampleRoot, "Sertainty/example", "/sampleid.xml");
                    } catch (java.io.IOException ioex) {
                        Log.println(Log.ERROR, "Install Example", ioex.getMessage());
                        ioex.printStackTrace();
                    }
                } else {
                    throw new IOException();
                }
            }
            catch(IOException ioex)
            {
                Log.println(Log.ERROR, "Install Assets", ioex.getMessage());
            }
        }

        return appRoot.exists() && boot_ini.exists() && home.exists() && exampleRoot.exists();
    }

    private void extractAsset(java.io.File parent, String assetParent, String assetName) throws IOException {
        InputStream in = getAssets().open(assetParent + assetName);
        java.io.File assetFile = new java.io.File(parent,assetName);
        OutputStream os = new FileOutputStream(assetFile);

        byte[] buffer = new byte[1024];
        int read = 0;
        while((read = in.read(buffer)) != -1)
        {
            os.write(buffer,0,read);
        }
        in.close();
        os.close();
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
    }
}
