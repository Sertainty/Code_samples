//
//  UXPContentView.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import SwiftUI

struct UXPContentView: View {
    
    @EnvironmentObject var appState:AppState
    
    var body: some View {
        NavigationView {
            VStack {
                FileClose()
                ScrollView{ ForEach(self.appState.virtualFileList!) { vfile in
                        UXPFileEntryView(vfile)
                    }
                    Spacer()
                }
            }
            .navigationBarTitle(Text("UXP File Contents"))
        }
    }
}

struct UXPContentView_Previews: PreviewProvider {
    static let appState = AppState(preview: true)
    static var previews: some View {
        appState.virtualFileList = [VirtualFileInfo(preview: true, name: "Test", type: .VirtualFolder),VirtualFileInfo(preview: true, name: "Test2", type: .VirtualUserFile)]
        return UXPContentView().environmentObject(appState)
    }
}
