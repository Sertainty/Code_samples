//
//  VirtualFileInfoList.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation

public class VirtualFileInfoList : UxpList {
    override init()
    {
        super.init()
        self.handle = uxplist_newVirtualFileList()
    }
    init(_ handle: UnsafeMutableRawPointer?)
    {
        super.init()
        self.handle = handle
    }
    
    func add(_ vfInfo: VirtualFileInfo) {
        uxplist_addVirtualFile(handle, vfInfo.handle)
    }
    
    func get(_ idx: Int32) -> VirtualFileInfo {
        return VirtualFileInfo(uxplist_getVirtualFile(handle, idx))
    }
    
}
