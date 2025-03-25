using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace Sertainty
{
  struct SertaintyCore
  {
    public const string dll =
#if UXPDEBUG
     "SertaintyCored"
#else
     "SertaintyCore"
#endif
      ;
  }

  struct SertaintyServices
  {
    public const string dll =
#if UXPDEBUG
    "SertaintyServicesd"
#else
    "SertaintyServices"
#endif
      ;
  }
    
  public enum Modifiers
  {
    Replace = 0x00001,
    Merge = 0x00002,
    Recurse = 0x00004,
    Compress = 0x00008,
    IncHidden = 0x00010,
    Shred = 0x00020,
    IncRealities = 0x00040,
    NoOptimize = 0x00080,
    DeleteSrc = 0x00100,
    MinSize = 0x00200,
    Reclaim = 0x00400,
    RecurseVirtual = 0x00800,
    Create = 0x01000,
    Load = 0x02000,
    ReadWrite = 0x04000,
    Protect = 0x08000,
    IncScript = 0x10000,
    Formatted = 0x20000,
    TokenReplace = 0x40000
  }
  public enum UxpMode
  {
    ShareAll = 0,
    ShareNone = 1,
    ShareReadOnly = 2
  }
  public enum Mode
  {
    ReadOnly = 1,
    WriteOnly = 2,
    ReadWrite = 3
  }
  public enum AuthorizationStatus
  {
    NotAuthorized = 0x00001,  /*!< Access to UXP is not authorized */
    InvalidUsername = 0x00002,  /*!< Invalid username was provided */
    FileMoved = 0x00004,  /*!< File has been moved */
    ScheduleViolation = 0x00008,  /*!< Schedule violation */
    Authorized = 0x00010,  /*!< User has been authorized */
    ConfigNotFound = 0x00020,  /*!< Address configuration not reconized */
    LocationNotFound = 0x00040,  /*!< Address location not recognized */
    DeviceNotFound = 0x00080,  /*!< Address device not recognized */
    DeviceLocationFound = 0x00100,  /*!< Address location and device pair not recognized */
    Challenged = 0x00200,  /*!< User is challenged */
    Panic = 0x00400,  /*!< User has indicated a panic situation */
    GlobalSchedViolation = 0x00800,  /*!< Global schedule violation */
    Threat = 0x01000,  /*!< Unauthoried access threat detected */
    Canceled = 0x02000,  /*!< User canceled access attempt */
    LdapViolation = 0x04000,  /*!< LDAP approval violation */
    ConfigFound = 0x08000,  /*!< Address configuration was recognized */
    NoSingleSignOn = 0x10000  /*!< Single sign-on attempt failed */
  }

  public enum SMEXStatus
  {
    Success = 0x0001,     /*!< Success status code */
    Failure = 0x0000,     /*!< Failure status code */
  }

  public enum VirtualFileType
  {
    VirtualUserFile = 1,
    VirtualFolder = 2,
    VirtualReality = 3,
    VirtualSQL = 18
  }

  public static class MappingsCApi
  {
    public static string ReadString(this IntPtr ptr)
    {
      if (ptr == IntPtr.Zero)
        return "";
      int len = 0;
      while (Marshal.ReadByte(ptr, len) != 0)
        len++;
      if (len == 0)
        return "";
      byte[] array = new byte[len];
      Marshal.Copy(ptr, array, 0, len);

      return Encoding.UTF8.GetString(array);
    }

    public static byte[] ReadBytes(this IntPtr ptr, int len)
    {
      if (ptr == IntPtr.Zero)
        return new byte[0];
      if (len == 0)
        return new byte[0];
      byte[] array = new byte[len];
      Marshal.Copy(ptr, array, 0, len);
      return array;
    }

#region uxpsys
    [DllImport(SertaintyCore.dll)]
    public static extern void uxpsys_setLogFile(string prefix, string version);

    [DllImport(SertaintyCore.dll, CharSet = CharSet.Ansi)]
    public static extern long uxpsys_installLicense(IntPtr error, string licenseFile);

    [DllImport(SertaintyCore.dll)]
    public static extern bool uxpsys_hasError(IntPtr handle);

    [DllImport(SertaintyCore.dll, CharSet = CharSet.Ansi)]
    public static extern string uxpsys_getErrorMessage(IntPtr handle);

    [DllImport(SertaintyCore.dll, CharSet = CharSet.Ansi)]
    public static extern long uxpsys_initLibrary(IntPtr error, long argc, string[] argv, string licenseFile, string appKey, string prefix, string version);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpsys_newCallStatusHandle();

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpsys_freeCallStatusHandle(IntPtr cs_handle);

    [DllImport(SertaintyCore.dll, CharSet = CharSet.Ansi)]
    public static extern void uxpsys_fileReadAll(IntPtr status, string filespec, IntPtr outbuf);

    [DllImport(SertaintyCore.dll, CharSet = CharSet.Ansi)]
    public static extern void uxpsys_fileWriteAll(IntPtr status, string filespec, IntPtr inbuf, long len, long mods);

#endregion

#region uxpba
    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpba_newHandle();

    [DllImport(SertaintyCore.dll)]
    //[return: MarshalAs(UnmanagedType.LPStr)]
    public static extern IntPtr uxpba_getData(IntPtr handle);

    [DllImport(SertaintyCore.dll)]
    public static extern long uxpba_getSize(IntPtr handle);

    [DllImport(SertaintyCore.dll, CharSet = CharSet.Ansi)]
    public static extern void uxpba_setData(IntPtr handle, [MarshalAs(UnmanagedType.LPStr)] string data, long len);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpba_clearData(IntPtr handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpba_freeHandle(IntPtr handle);

    //[DllImport(SertaintyCore2.dll,EntryPoint = "uxpba_setData", CharSet = CharSet.Ansi)]
    //public static extern void uxpba_setBytes(IntPtr handle, [MarshalAs(UnmanagedType.LPStr)] string data, long len);

#endregion

#region uxpvariable

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpvar_freeHandle(IntPtr var_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpvar_getName(IntPtr var_handle, IntPtr buffer);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpvar_toString(IntPtr var_handle, IntPtr buffer);

#endregion

#region uxplist

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxplist_newByteArrayList();

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxplist_newChallengeList();

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxplist_newVariableList();

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxplist_newVirtualFileList();

    [DllImport(SertaintyCore.dll)]
    public static extern void uxplist_freeList(IntPtr list_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxplist_clear(IntPtr list_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern int uxplist_count(IntPtr list_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxplist_addVirtualFile(IntPtr vfl_handle, IntPtr vf_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxplist_addChallenge(IntPtr chl_handle, IntPtr ch_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxplist_addString(IntPtr sl_handle, IntPtr ba_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxplist_getVariable(IntPtr handle, int element);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxplist_getVirtualFile(IntPtr handle, int element);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxplist_getChallenge(IntPtr handle, int element);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxplist_getByteArray(IntPtr handle, int element);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxplist_addVariable(IntPtr varl_handle, IntPtr var_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxplist_addByteArray(IntPtr bal_handle, IntPtr ba_handle);

#endregion

#region uxpfile

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpfile_newHandle();

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpfile_openNewFile(IntPtr handle, string data, string governance, long govtype, long mods, long flags);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpfile_addVirtualFromFile(IntPtr uxp_handle, string virName, string filespec, long pageSize, long cacheSize, long mods);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpfile_newVirtualFile(IntPtr uxp_handle, string virName, long pageSize, long cacheSize, long mods);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpfile_editVirtualFile(IntPtr uxp_handle, string virName, IntPtr outname_buffer, string filespec);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpfile_openVirtualFile(IntPtr uxp_handle, string fileSpec, Mode mode);

    [DllImport(SertaintyCore.dll)]
    public static extern long uxpfile_readVirtualFile(IntPtr uxp_handle, IntPtr vf_handle, IntPtr buffer, long mx);

    [DllImport(SertaintyCore.dll)]
    public static extern long uxpfile_writeVirtualFile(IntPtr uxp_handle, IntPtr vf_handle, IntPtr buffer);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpfile_closeVirtualFile(IntPtr uxp_handle, IntPtr vf_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern bool uxpfile_compareExternalFile(IntPtr uxp_handle, string vf_name, string ext_file_name);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpfile_exportVirtualFile(IntPtr uxp_handle, string vf_name, string ext_file_name, long mods);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpfile_close(IntPtr uxp_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpfile_openFile(IntPtr uxp_handle, string source, UxpMode mode);

    [DllImport(SertaintyCore.dll)]
    public static extern AuthorizationStatus uxpfile_authenticate(IntPtr uxp_handle, bool useSSO);

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpfile_getChallengeCount(IntPtr uxp_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpfile_getChallenge(IntPtr uxp_handle, int idx);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpfile_addResponse(IntPtr uxp_handle, IntPtr ch_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpfile_freeHandle(IntPtr uxp_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpfile_getVirtualFileInfo(IntPtr uxp_handle, string name);
  
    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpfile_getVirtualDirectory(IntPtr uxp_handle, string path, long mods);

#endregion

#region uxpvf

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpvf_newHandle();

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpvf_getName(IntPtr vf_handle, IntPtr ba_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpvf_getFullName(IntPtr vf_handle, IntPtr ba_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpvf_getReality(IntPtr vf_handle, IntPtr ba_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpvf_getType(IntPtr vf_handle);


#endregion

#region uxpid
    [DllImport(SertaintyCore.dll)]
    public static extern void uxpid_publishToBuffer(IntPtr status, IntPtr id, string doc);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpid_publishToFile(IntPtr status, string id, string doc, long mods);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpid_newHandle();

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpid_authenticate(IntPtr id_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpid_openUxpFromFile(IntPtr id_handle, IntPtr uxp_handle, string source, int mode);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpid_freeHandle(IntPtr id_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpid_getChallengeCount(IntPtr id_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpid_addResponse(IntPtr id_handle, IntPtr ch_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpid_getChallenges(IntPtr id_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpid_openSessionFromFile(IntPtr id_handle, string idFile, bool share, int time);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpid_getChallenge(IntPtr id_handle, int offset);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpid_closeSession(IntPtr uxp_handle);
#endregion
     
#region uxpch

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpch_newHandle ();

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpch_freeHandle(IntPtr ch_handle);
    
    [DllImport(SertaintyCore.dll)]
    public static extern void uxpch_getPrompt(IntPtr ch_handle, IntPtr ba_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpch_setName (IntPtr ch_handle, string name);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpch_setPrompt(IntPtr ch_handle, string prompt);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpch_setValueString(IntPtr ch_handle, string value);
    
    [DllImport(SertaintyCore.dll)]
    public static extern void uxpch_startTimer(IntPtr ch_handle);
    
    [DllImport(SertaintyCore.dll)]
    public static extern void uxpch_endTimer(IntPtr ch_handle);

#endregion

#region uxpsmex

    [DllImport (SertaintyCore.dll)]
    public static extern IntPtr uxpsmex_newHandle ();

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpsmex_initSession (IntPtr smex_handle, IntPtr ba_out_token);

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpsmex_initSessionKnownSMIDFile (IntPtr smex_handle, string localSmidFile,
                                  IntPtr list_response, IntPtr ba_outToken);

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpsmex_startSession (IntPtr smex_handle, IntPtr inToken, long length);

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpsmex_startSessionKnownSMIDFile (IntPtr smex_handle, string remoteSmidFile);

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpsmex_encode (IntPtr smex_handle, string clear, long length, IntPtr ba_out);

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpsmex_decode (IntPtr smex_handle, string encoded, long length, IntPtr ba_out);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpsmex_freeHandle (IntPtr smex_handle);
#endregion

#region uxpconfig
    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpcf_getCurrentConfig(bool useStale);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpcf_freeHandle(IntPtr cf_handle);
#endregion

#region uxplocation
    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpcf_getLocation(IntPtr cf_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxploc_getProperty(IntPtr loc_handle, string name);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxploc_getTable(IntPtr loc_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxploc_freeHandle(IntPtr loc_handle);

#endregion

#region uxlmsg

    [DllImport(SertaintyCore.dll, CharSet = CharSet.Ansi)]
    public static extern void uxlmsg_loadSecureText(IntPtr cs_handle, string domain, string file_name, IntPtr key, int key_size);

    [DllImport(SertaintyCore.dll, CharSet = CharSet.Ansi)]
    public static extern void uxlmsg_getSecureText(IntPtr cs_habdle, IntPtr out_buffer, string domain, int id);

#endregion

#region uxpmsg

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpmsgid_newHandle();

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpmsgid_setLinkedID(IntPtr msg_handle, IntPtr id_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpmsgid_openNewBuffer(IntPtr msg_handle, IntPtr outbuf, IntPtr exchange, int page_size, int cache_size, int mods);

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpmsgid_append(IntPtr msg_handle, IntPtr msg_file_handle, string in_buffer, long len);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpmsgid_close(IntPtr msg_handle, IntPtr msg_file_handle);

    [DllImport(SertaintyCore.dll)]
    public static extern void uxpmsgid_getProperties(IntPtr msg_handle, IntPtr msg_file_handle, IntPtr out_buffer);

    [DllImport(SertaintyCore.dll)]
    public static extern int uxpmsgid_read(IntPtr msg_handle, IntPtr msg_file_handle, IntPtr buf, int mx);

    [DllImport(SertaintyCore.dll)]
    public static extern IntPtr uxpmsgid_openBuffer(IntPtr msg_handle, IntPtr buffer, long len);

#endregion

#region uxpds

    [DllImport(SertaintyServices.dll)]
    public static extern IntPtr uxpds_newHandle();

    [DllImport(SertaintyServices.dll)]
    public static extern IntPtr uxpdl_newHandle(IntPtr service_handle);

    [DllImport(SertaintyServices.dll)]
    public static extern IntPtr uxpds_getService();

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpds_freeHandle(IntPtr ds_handle);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpdl_freeHandle(IntPtr dl_handle);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpds_initializeDatabase(IntPtr ds_handle, string spec, IntPtr id, string username, string dbmodule, string dbparams);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpds_openDatabase(IntPtr ds_handle, string spec, int readonlyInt);

    [DllImport(SertaintyServices.dll)]
    public static extern long uxpds_openSession(IntPtr ds_handle, string username);

    [DllImport(SertaintyServices.dll)]
    public static extern int uxpds_authenticate(IntPtr ds_handle, long session);

    [DllImport(SertaintyServices.dll)]
    public static extern int uxpds_getChallengeCount(IntPtr ds_handle, long session);

    [DllImport(SertaintyServices.dll)]
    public static extern IntPtr uxpds_getChallenge(IntPtr ds_handle, long session, int offset);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpds_addResponse(IntPtr ds_handle, long session, IntPtr response);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpds_closeSession(IntPtr ds_handle, long session);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpds_closeDatabase(IntPtr ds_handle);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpds_dropDatabase(IntPtr ds_handle, string spec, string dbmodule, string dbparams);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpds_setServer(IntPtr ds_handle, long session, string url);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpds_newUser(IntPtr ds_handle, long session, string username, string formalname, string description, string email, int privileges, string app_data1, string app_data2);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpds_newUserId(IntPtr ds_handle, long session, string username, string idname, int flags, string description, IntPtr uxpid, IntPtr source);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpdl_newDelegate(IntPtr dl_handle, long session, string owner, string name, string description, long expiration, int permit_offline, int offline_duration, int access_max, int flags);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpdl_subscribe(IntPtr dl_handle, long session, string owner, string name, string member, long expiration, int permit_offline, int offline_duration, int access_max, int access_count);

    [DllImport(SertaintyServices.dll)]
    public static extern IntPtr uxpdl_getDelegates(IntPtr dl_handle, long session, string owner);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpdl_getDelegate(IntPtr dl_handle, long session, string owner, string name, IntPtr description, ref long expiration, ref int permit_offline, ref int offline_duration, ref int access_max, ref int flags, IntPtr checksum, IntPtr uxpid);

    [DllImport(SertaintyServices.dll)]
    public static extern IntPtr uxpds_getUsers(IntPtr ds_handle, long session);
    
    [DllImport(SertaintyServices.dll)]
    public static extern void uxpds_getUser(IntPtr ds_handle, long session, string username, IntPtr formalname, IntPtr description, IntPtr email, ref int privileges, IntPtr app_data1, IntPtr app_data2);

    [DllImport(SertaintyServices.dll)]
    public static extern IntPtr uxpdl_getSubscribers(IntPtr dl_handle, long session, string owner, string name);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpdl_getSubscription(IntPtr dl_handle, long session, string owner, string name, string subscriber, ref long expiration, ref int permit_offline, ref int offline_duration, ref int access_max, ref int access_count);

    [DllImport(SertaintyServices.dll)]
    public static extern IntPtr uxpdl_getSubscriptions(IntPtr dl_handle, long session, string username);

    [DllImport(SertaintyServices.dll)]
    public static extern void uxpds_webService(IntPtr ds_handle, IntPtr request, IntPtr response, string callerIP);
#endregion
  }
}
