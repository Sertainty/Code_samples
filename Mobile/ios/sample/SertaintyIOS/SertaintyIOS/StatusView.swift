//
//  StatusView.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import SwiftUI

class StatusItem : ObservableObject {
    var name : String = ""
    var initmsg = "Not Run"
    var success = "Success"
    var failure = "Failure"
    @Published var status : Int = 0
    
    init(_ name:String)
    {
        self.name = name
    }
    convenience init(_ name:String, _ status: Int)
    {
        self.init(name)
        self.status = status
    }
    
    convenience init(_ name: String, _ initmsg: String, _ success: String, _ failure: String, _ status: Int)
    {
        self.init(name)
        self.initmsg = initmsg
        self.success = success
        self.failure = failure
        self.status = status
    }
    
    func setStatus(status: Int) {
        self.status = status
    }
    var statusText : String {
        if self.status == 0 {
            return initmsg
        }
        else if self.status == 1 {
            return success
        }
        else {
            return failure
        }
    }
    
    var statusColor : Color {
        if self.status == 0 {
            return Color.gray
        }
        else if self.status == 1 {
            return Color.green
        }
        else {
            return Color.red
        }
    }
}

struct StatusView: View {
    @ObservedObject var item:StatusItem
    init(_ item: StatusItem)
    {
        self.item = item
    }
    var body: some View {
        HStack {
            Text(self.item.name)
            Spacer()
            Text(verbatim: self.item.statusText)
                .fontWeight(Font.Weight.bold)
                .foregroundColor(self.item.statusColor)
        }
    }
}

struct StatusView_Previews: PreviewProvider {
    static var previews: some View {
        StatusView(StatusItem("Test Item"))
    }
}
