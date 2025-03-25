//
//  List.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation

public class UxpList {
    var handle:UnsafeMutableRawPointer?
    
    func clear() {
        uxplist_clear(handle)
    }
    
    func count() -> Int32 {
        return uxplist_count(UnsafeMutableRawPointer(handle))
    }
    
    deinit {
        uxplist_freeList(handle)
    }
}
