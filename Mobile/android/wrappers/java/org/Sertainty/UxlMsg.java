package org.Sertainty;

public class UxlMsg {

    public static void loadSecureText(Sys.CallStatus callStatus, String domain, String filename, byte[] key)
    {
        loadSecureText(callStatus.getHandle(), domain, filename, key);
    }

    public static String getSecureText(Sys.CallStatus callStatus, String domain, int id)
    {
        return getSecureText(callStatus.getHandle(),domain,id);
    }

    private native static void loadSecureText(long cshandle, String domain, String filename, byte[] key);

    private native static String getSecureText(long cshandle, String domain, int id);
}
