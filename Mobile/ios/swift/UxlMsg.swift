//
//  Id.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation

public class UxlMsg {
    public class func loadSecureText(_ callStatus: CallStatus, _ domain: String, _ filename: String, _ key: [Int8]) {
        uxlmsg_loadSecureText(callStatus.handle, domain, filename, key, Int32(key.count))
    }
    
    public class func getSecureText(_ callStatus: CallStatus, _ domain: String, _ id: Int16) -> String? {
        let outbuffer = ByteArray()
        uxlmsg_getSecureText(callStatus.handle, outbuffer.handle, domain, id)
        return outbuffer.getString()
    }
    
}
