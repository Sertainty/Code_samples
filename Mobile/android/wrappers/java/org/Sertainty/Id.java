package org.Sertainty;

public class Id {

    public static void publishToFile(Sys.CallStatus callStatus, String idFileSpec, String doc, int mods)
    {
        publishToFile(callStatus.getHandle(), idFileSpec, doc, mods);
    }

    private native static void publishToFile(long callStatusHandle, String idFileSpec, String doc, int mods);
}
