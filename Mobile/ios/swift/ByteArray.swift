//
//  ByteArray.swift
//  CoreLibrary
//
//  Created by Karim Lalani on 1/15/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation

public class ByteArray {
    
    private(set) var handle:OpaquePointer;
    
    public init(){
        handle = uxpba_newHandle()
    }
    
    public init(_ bahandle: OpaquePointer)
    {
        handle = bahandle
    }
    
    public func getSize() -> Int64 {
        uxpba_getSize(handle)
    }
    
    public func setData(_ data: Data) {
        let uarray = [UInt8](data)
        let array = uarray.map {
            (item:UInt8) -> Int8 in
            Int8(bitPattern: item)
        }
        uxpba_setData(handle,array,Int32(data.count))
    }
    
    public func appendData(_ data: Data) {
        let uarray = [UInt8](data)
        let array = uarray.map {
            (item:UInt8) -> Int8 in
            Int8(bitPattern: item)
        }
        uxpba_appendData(handle,array,Int32(data.count))
    }
    
    public func appendByteArray(_ buffer: ByteArray) {
        if let data = buffer.getData() {
            self.appendData(data)
        }
    }
    
    public func getData() -> Data? {
        
        let size = getSize()
        
        return uxpba_getData(handle).map({
            (data:UnsafePointer<Int8>) -> Data in
            let buffer = UnsafeBufferPointer(start: data, count: Int(size))
            let ubuffer = buffer.map {
                (item:Int8) -> UInt8 in
                UInt8(bitPattern: item)
            }
            return Data(bytes: ubuffer, count: Int(size))
        })
    }
    
    public func getString() -> String? {
        return uxpba_getData(handle).map {
            (pstr:UnsafePointer<Int8>) -> String in
            return String(cString:pstr)
        }
    }
    
    public func clear() {
        uxpba_clearData(handle)
    }
    
    public func setString(_ data:String) {
        uxpba_setData(handle,data,Int32(data.count))
    }
    
    public func appendString(_ data:String) {
        uxpba_appendData(handle, data, Int32(data.count))
    }
    
    deinit {
        uxpba_freeHandle(handle)
    }
}
