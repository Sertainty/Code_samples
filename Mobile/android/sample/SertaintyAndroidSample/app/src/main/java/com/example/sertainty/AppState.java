package com.example.sertainty;

import android.content.Context;
import android.content.ContextWrapper;
import android.net.Uri;
import android.util.Log;

import org.Sertainty.ByteArray;
import org.Sertainty.File;
import org.Sertainty.Sys;

import java.io.InputStream;
import java.util.Arrays;

public class AppState {
    private AppState(){}

    public static synchronized AppState getInstance()
    {
        if (instance == null)
        {
            instance = new AppState();
        }
        return instance;
    }

    public boolean getInitialized() { return this.initialized; }
    public void setInitialized(boolean initialized) { this.initialized = initialized; }

    public File getOpenFile(){
        return this.openFile;
    }

    public void releaseOpenFile() {
        this.openFile = null;
    }

    public void setOpenFile(File openFile) {
        this.openFile = openFile;
    }

    public Context getContext() { return this.context; }

    public void setContext(Context context) { this.context = context; }

    public boolean loadInSandbox(Uri fileUri, ByteArray outFilename)
    {
        String[] fragments = fileUri.getPath().split("/");
        java.io.File sandbox = new java.io.File(getContext().getDataDir(), "sandbox");
        if(!sandbox.exists() && !sandbox.mkdirs())
        {
            Log.println(Log.ERROR,"Sandbox","Unable to create sandbox.");
            return false;
        }
        Log.println(Log.DEBUG,"Sandbox","Sandbox ready.");
        String sandboxFile = sandbox.getAbsolutePath() + "/"+ fragments[fragments.length - 1];
        Log.println(Log.DEBUG,"Sandbox",sandboxFile);
        try {
            InputStream in = getContext().getContentResolver().openInputStream(fileUri);
            ByteArray buffer = new ByteArray();
            Sys.CallStatus callStatus = new Sys.CallStatus();
            int len;
            byte[] buf = new byte[1024];
            while ((len = in.read(buf)) > 0) {
                buffer.appendData(Arrays.copyOf(buf, len));
            }
            Sys.fileWriteAll(callStatus, sandboxFile , buffer.getData(), 1);
            if (callStatus.hasError()) {
                Log.println(Log.ERROR, "Sandbox", callStatus.getErrorMessage());
            }
            java.io.File file = new java.io.File(sandboxFile);
            if(file.exists())
            {
                outFilename.setString(sandboxFile);
            }
            else
            {
                Log.println(Log.ERROR, "Sandbox","File doesn't exist in sandbox.");
                return false;
            }
            return !callStatus.hasError();
        }
        catch (Exception e)
        {
            Log.println(Log.ERROR, "Sandbox",e.getMessage());
            e.printStackTrace();
            return false;
        }
    }

    public boolean writeToExternal(String fileName, ByteArray fileContents, ByteArray outFilename)
    {
        Sys.CallStatus callStatus = new Sys.CallStatus();
        java.io.File outFile = new java.io.File( getContext().getExternalFilesDir(null),fileName);
        if(outFile.exists())
        {
            outFile.delete();
        }
        Sys.fileWriteAll(callStatus,outFile.getAbsolutePath(),fileContents.getData(),1);
        if(callStatus.hasError())
        {
            Log.println(Log.ERROR,"Write to External",callStatus.getErrorMessage());
        }
        if(outFile.exists())
        {
            outFilename.setString(outFile.getAbsolutePath());
        }
        else
        {
            Log.println(Log.ERROR,"Write to External","File doesn't exist in external.");
            return false;
        }
        return !callStatus.hasError();
    }

    private static AppState instance;

    private boolean initialized = false;
    private File openFile;

    private Context context = null;
}
