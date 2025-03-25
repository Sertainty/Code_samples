//
//  VirtualFile.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation

public class VirtualFile {
    private(set) var handle: OpaquePointer
    private var fileHandle: OpaquePointer?
    
    init(_ file: UxpFile, _ vfHandle: OpaquePointer)
    {
        self.fileHandle = file.handle
        self.handle = vfHandle
    }
    
    func read (_ buffer: ByteArray, _ mx: Int64) -> Int64 {
        return uxpfile_readVirtualFile(fileHandle, handle, buffer.handle, mx)
    }
    
    func close() {
        uxpfile_closeVirtualFile(fileHandle, handle)
    }
}
