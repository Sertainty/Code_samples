//
//  Challenge.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation

public class Challenge {
    private(set) var handle: OpaquePointer
    
    init(){
        self.handle = uxpch_newHandle()
    }
    
    init(_ handle: OpaquePointer)
    {
        self.handle = handle
    }
    
    var prompt : String? {
        get {
            let buffer = ByteArray()
            uxpch_getPrompt(handle, buffer.handle)
            return buffer.getString()
        }
        set {
            uxpch_setPrompt(handle, newValue)
        }
    }
    
    var name : String? {
        get {
            let buffer = ByteArray()
            uxpch_getName(handle, buffer.handle)
            return buffer.getString()
        }
        set {
            uxpch_setName(handle, newValue)
        }
    }
    
    func setValueString(_ value: String) {
        uxpch_setValueString(handle, value)
    }
    
    func startTimer() {
        uxpch_startTimer(handle)
    }
    
    func endTimer() {
        uxpch_endTimer(handle)
    }
    
    deinit {
        uxpch_freeHandle(handle)
    }
}
