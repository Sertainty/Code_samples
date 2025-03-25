//
//  Id.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation

public class Id {
    public class func publishToFile(_ callStatus: CallStatus, _ idFileSpec: String, _ doc: String, _ mods: Int) {
        uxpid_publishToFile(callStatus.handle, idFileSpec, doc, Int32(mods))
    }
}
