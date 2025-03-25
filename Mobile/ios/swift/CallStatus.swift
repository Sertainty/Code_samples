//
//  CallStatus.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation

public class CallStatus {
    private(set) var handle:OpaquePointer
    private var derived = false
    init()
    {
        handle = uxpsys_newCallStatusHandle()
    }
    
    init(_ handle:OpaquePointer)
    {
        self.handle = handle
        self.derived = true
    }
    
    func hasError() -> Bool {
        uxpsys_hasError(handle) == 1
    }
    
    func getErrorMessage() -> String {
        String(cString:uxpsys_getErrorMessage(handle))
    }
    
    deinit {
        if !derived {
            uxpsys_freeCallStatusHandle(handle)
        }
    }
}
