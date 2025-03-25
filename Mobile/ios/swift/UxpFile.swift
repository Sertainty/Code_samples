//
//  File.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation

public class UxpFile {
    
    public enum UxpStatus : Int32 {
        case StatusNotAuthorized        = 0x00001  /*!< Access to UXP is not authorized */
        case StatusInvalidUsername      = 0x00002  /*!< Invalid username was provided */
        case StatusFileMoved            = 0x00004  /*!< File has been moved */
        case StatusScheduleViolation    = 0x00008  /*!< Schedule violation */
        case StatusAuthorized           = 0x00010  /*!< User has been authorized */
        case StatusConfigNotFound       = 0x00020  /*!< Address configuration not reconized */
        case StatusLocationNotFound     = 0x00040  /*!< Address location not recognized */
        case StatusDeviceNotFound       = 0x00080  /*!< Address device not recognized */
        case StatusDeviceLocationFound  = 0x00100  /*!< Address location and device pair not recognized */
        case StatusChallenged           = 0x00200  /*!< User is challenged */
        case StatusPanic                = 0x00400  /*!< User has indicated a panic situation */
        case StatusGlobalSchedViolation = 0x00800  /*!< Global schedule violation */
        case StatusThreat               = 0x01000  /*!< Unauthoried access threat detected */
        case StatusCanceled             = 0x02000  /*!< User canceled access attempt */
        case StatusLdapViolation        = 0x04000  /*!< LDAP approval violation */
        case StatusConfigFound          = 0x08000  /*!< Address configuration was recognized */
        case StatusNoSingleSignOn       = 0x10000  /*!< Single sign-on attempt failed */
    }
    
    private(set) var handle:OpaquePointer
    private(set) var status:UxpStatus = UxpStatus.StatusNotAuthorized
    private(set) var url:URL?
    private var vFileInfoList : VirtualFileInfoList?
    
    init(){
        handle = uxpfile_newHandle()
    }
    
    init(_ handle:OpaquePointer, _ url:URL){
        self.handle = handle
        self.url = url
    }

    public class func create(_ fileName: URL,_ governance: String,_ govType: Int32,_ mods: Int32,_ flags: Int32) -> UxpFile {
        let handle:OpaquePointer = uxpfile_newHandle()
        uxpfile_openNewFile(handle,fileName.path, governance, govType, mods, flags)
        return UxpFile(handle, fileName)
    }
    
    func addVirtualFromFile(_ vfName: String, _ fileSpec: String, _ pageSize: Int64, _ cacheSize: Int64, _ mods: Int32) {
        uxpfile_addVirtualFromFile(handle, vfName, fileSpec, pageSize, cacheSize, mods)
    }
    
    func addVirtualFromBuffer(_ vfName: String, _ data: Data, _ pageSize: Int64, _ cacheSize: Int64, _ mods: Int32) {
        let uarray = [UInt8](data)
        let array = uarray.map {
            (item:UInt8) -> Int8 in
            Int8(bitPattern: item)
        }
        let ump = UnsafeMutablePointer(mutating:array)
        uxpfile_addVirtualFromBuffer(handle, vfName, ump, Int64(data.count), pageSize, cacheSize, mods)
    }
    
    func deleteVirtualFile(_ vfName: String, _ mods: Int32) {
        uxpfile_deleteVirtualFile(handle,vfName, mods)
    }
    
    func compareExternalFile(_ vfName: String, _ fileName: String) -> Bool {
        uxpfile_compareExternalFile(handle, vfName, fileName) == 1
    }
    
    func close() {
        uxpfile_close(handle)
        uxpfile_freeHandle(handle)
    }
    
    public class func open(_ source: URL, _ mode: UInt8) -> UxpFile {
        let handle: OpaquePointer = uxpfile_newHandle()
        uxpfile_openFile(handle, source.path, mode)
        return UxpFile(handle, source)
    }
    
    func authenticate() -> UxpStatus {
        let rawStatus = uxpfile_authenticate(handle,0)
        guard let authst = UxpStatus(rawValue: rawStatus) else {
            fatalError("Unresolved UXP Status returned from uxpfile_authenticate: \(rawStatus)")
        }
        status = authst
        return status
    }
    
    func getChallengeCount() -> Int32 {
        uxpfile_getChallengeCount(handle)
    }
    
    func getChallenge(_ offset: Int32) -> Challenge {
        Challenge(uxpfile_getChallenge(handle, offset))
    }
    
    func addResponse(_ challenge: Challenge) {
        uxpfile_addResponse(handle, challenge.handle)
    }
    
    func getChallenges() -> ChallengeList {
        ChallengeList(uxpfile_getChallenges(handle))
    }
    
    func isOpen() -> Bool {
        uxpfile_isOpen(handle,1) == 1
    }
    
    func isReadOnly() -> Bool {
        uxpfile_isReadOnly(handle,1) == 1
    }
    
    func isAuthorized() -> Bool {
        status == .StatusAuthorized
    }
    
    func readAllVirtualFile(_ name: String) -> ByteArray {
        let buffer = ByteArray()
        uxpfile_readAllVirtualFile(handle, name, buffer.handle)
        return buffer
    }
    
    var name : String? {
        let buffer = ByteArray()
        uxpfile_getName(handle, buffer.handle)
        return buffer.getString()
    }
    
    var fileSpecification : String? {
        let buffer = ByteArray()
        uxpfile_getFileSpecification(handle, buffer.handle)
        return buffer.getString()
    }
    
    func getVirtualFileInfo(_ name: String) -> VirtualFileInfo {
        return VirtualFileInfo(uxpfile_getVirtualFileInfo(handle, name))
    }
    
    func getVirtualDirectory(_ name: String, _ mods: Int32) -> VirtualFileInfoList? {
        if let vflist = self.vFileInfoList {
            vflist.clear()
        }
        self.vFileInfoList = VirtualFileInfoList(uxpfile_getVirtualDirectory(handle, name, mods))
        return self.vFileInfoList
    }
    
    func getVirtualDirectoryArray(_ name: String, _ mods: Int32) -> [VirtualFileInfo] {
        let ovflist = getVirtualDirectory(name, mods)
        var vfiles = [VirtualFileInfo]()
        
        if let vflist = ovflist {
            for i in 0..<vflist.count() {
                vfiles.append(vflist.get(i))
            }
        }
        return vfiles
    }
    
    func openVirtualFile(_ vfName: String, _ mode: UInt8) -> VirtualFile {
        return VirtualFile(self, uxpfile_openVirtualFile(handle, vfName, mode))
    }
    
    func getCallStatus() -> CallStatus {
        return CallStatus(handle)
    }
    
    deinit {
        uxpfile_freeHandle(handle)
    }
}
