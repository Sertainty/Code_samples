//
//  UXPFileEntryView.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import SwiftUI
import UIKit

struct UXPFileEntryView: View {
    
    @EnvironmentObject var appState: AppState
    
    private var vfile:VirtualFileInfo
    init(_ vfile:VirtualFileInfo)
    {
        self.vfile = vfile
    }
    var body: some View {
        HStack {
            Button(action:{
                if self.vfile.type! != .VirtualUserFile {
                    self.appState.virtualDir = self.vfile.fullName!
                } else {
                    self.appState.openVirtualFile(vfile: self.vfile)
                }
            }){
                Text("\(self.vfile.name!) (\(self.vfile.type! == .VirtualUserFile ? "File" : "Folder"))")
            }
            Spacer()
            if self.vfile.type == .VirtualUserFile {
                Button(action:{
                    guard let uxpFile = self.appState.uxpFile else {
                        return
                    }
                    uxpFile.deleteVirtualFile(self.vfile.fullName!, 0)
                    self.appState.virtualFileList = uxpFile.getVirtualDirectoryArray(self.appState.virtualDir, 0)
                    self.appState.viewPage = .File
                }){
                    Text("Delete")
                }
            }
        }.padding()
    }
}

struct UXPFileEntryView_Previews: PreviewProvider {
    static var previews: some View {
        UXPFileEntryView(VirtualFileInfo(preview: true, name: "My File" , type: .VirtualUserFile))
    }
}
