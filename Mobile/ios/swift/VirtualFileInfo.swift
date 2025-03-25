//
//  VirtualFileInfo.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation

public class VirtualFileInfo : Identifiable {
    
    enum VirtualFileType:Int32 {
        case VirtualUserFile  = 1 //     (1)   /*!< Virtual file type is user data */
            case VirtualFolder  = 2 //       (2)   /*!< Virtual file type is a folder */
            case VirtualReality = 3 //      (3)   /*!< Virtual file type is a reality */
            case VirtualSQL = 18 //           (18)  /*!< Virtual file type is interactive SQL */
    }
    
    private(set) var handle:OpaquePointer
    private(set) var preview:Bool = false
    private(set) var type:VirtualFileType?
    private(set) var name:String?
    private(set) var fullName: String?
    private(set) var typeName: String?
    private(set) var reality: String?
    
    init()
    {
        self.handle = uxpvf_newHandle()
    }
    
    convenience init(preview: Bool, name: String, type: VirtualFileType)
    {
        self.init()
        self.preview = preview
        if preview {
            self.name = name
            self.fullName = name
            self.type = type
        }
    }
    
    private func readProperties(_ handle: OpaquePointer) {
        let buffer = ByteArray()
        uxpvf_getName(handle, buffer.handle)
        name = buffer.getString()
        
        uxpvf_getFullName(handle, buffer.handle)
        fullName = buffer.getString()
        
        type = VirtualFileType(rawValue: uxpvf_getType(handle))
        
        uxpvf_getTypeName(handle, buffer.handle)
        typeName = buffer.getString()
        
        uxpvf_getReality(handle, buffer.handle)
        reality = buffer.getString()
    }
    
    init(_ handle:OpaquePointer)
    {
        self.handle = handle
        readProperties(handle)
    }
    
    var permissions: Int32 {
        return uxpvf_getPermissions(handle)
    }
    
    class func getTypeName(_ type: Int32) -> String? {
        let buffer = ByteArray()
        uxpvf_getTypeNameStatic(type, buffer.handle)
        return buffer.getString()
    }
    
    var members: VirtualFileInfoList {
        let listHandle = UnsafeMutableRawPointer(uxpvf_getMembers(handle))
        return VirtualFileInfoList(listHandle)
    }
    
    deinit {
        uxpvf_freeHandle(handle)
    }
}
