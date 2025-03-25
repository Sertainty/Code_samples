//
//  ChallengeList.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation

public class ChallengeList : UxpList {
    
    override init() {
        super.init()
        self.handle = uxplist_newChallengeList()
    }
    
    init(_ handle:UnsafeMutableRawPointer?) {
        super.init()
        self.handle = handle
    }
    
    func add(_ challenge: Challenge) {
        uxplist_addChallenge(handle, challenge.handle)
    }
    
    func get(_ idx: Int32) -> Challenge {
        return Challenge(uxplist_getChallenge(handle, idx))
    }
}
