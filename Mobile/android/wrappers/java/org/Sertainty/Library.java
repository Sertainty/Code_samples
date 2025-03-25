package org.Sertainty;

import android.os.Build;
import android.util.Log;

public class Library {
    public static String SertaintyCore(){
        String library = "SertaintyCoreJni"; //Release
        //String library = "SertaintyCoreJnid"; //Debug
        Log.println(Log.INFO,"Library", library);
        return library;
    };// = "SertaintyCoreJni";
}
