//
//  Sys.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/17/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation

public class Sys {
    public class func getAppRoot() -> String? {
        return uxpsys_getAppRoot().map {
            (ptr: UnsafePointer<Int8>) -> String in
            String(cString: ptr)
        }
    }
    
    public class func setAppRoot(_ path :String) {
        uxpsys_setAppRoot(path)
    }
    
    public class func initLibrary(baError: ByteArray,
                           appKey: String,
                           prefix: String,
                           version: String,
                           onInit: @escaping ()->Void,
                           onFail: @escaping ()->Void) {
        let argv = UnsafeMutablePointer<UnsafeMutablePointer<Int8>?>.allocate(capacity: 1)
        let argv1 = UnsafeMutablePointer<Int8>(mutating: appKey)
        argv.assign(repeating: argv1, count: 1)
        
        let dispatchQueue = DispatchQueue(label: "InitializeLibrary")
        dispatchQueue.async {
            if uxpsys_initLibrary(baError.handle, 1, argv, "sertainty.lic", "SertintyONE", prefix, version) == 1 {
                DispatchQueue.main.async {
                    onInit()
                }
            } else {
                DispatchQueue.main.async {
                    onFail()
                }
            }
        }
    }
    
    public class func installLicense(baError: ByteArray,
                                     licenseFile: String,
                                     onInstall: @escaping ()->Void,
                                     onFail: @escaping () -> Void)
    {
        DispatchQueue(label: "InstallLicense").async {
            if uxpsys_installLicense(baError.handle, licenseFile) == 1 {
                DispatchQueue.main.async {
                    onInstall()
                }
            } else {
                DispatchQueue.main.async {
                    onFail()
                }
            }
        }
    }
    
    public class func fileReadAll(_ callStatus: CallStatus, _ fileSpec: String, _ buffer: ByteArray)
    {
        uxpsys_fileReadAll(callStatus.handle,fileSpec,buffer.handle)
    }
    
    public class func fileWriteAll(_ callStatus: CallStatus, _ fileSpec: String, _ data: Data, _ mods: Int)
    {
        let array = data.map{
            (item: UInt8) -> Int8 in
            Int8(item)
        }
        uxpsys_fileWriteAll(callStatus.handle,fileSpec,array,uxp_int64( data.count),Int32(mods))
    }
}
