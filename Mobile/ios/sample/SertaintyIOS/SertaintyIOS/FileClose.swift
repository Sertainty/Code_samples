//
//  FileClose.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/30/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import SwiftUI

struct FileClose: View {
    @EnvironmentObject var appState:AppState
    var body: some View {
        HStack {
            if self.appState.viewPage == .File {
                Button(action:{
                    guard
                        let file = self.appState.uxpFile,
                        let rootView = self.appState.rootViewController
                    else {
                        return
                    }
                    
                    let controller = UIDocumentPickerViewController(documentTypes: ["public.item"], in: .import)
                    controller.delegate = self.appState.virtualFilePickerDelegate
                    self.appState.virtualFilePickerDelegate.registerAppState(appState: self.appState)
                    rootView.present(controller, animated: true) {
                        print("On root view present")
                    }
                }){Text("Add File")}
            }
            Spacer()
            Button(action:{
                self.appState.viewPage = .Home
                self.appState.closeUxp()
            }){
                Text("Close")
            }
        }.padding()
    }
}

struct FileClose_Previews: PreviewProvider {
    static let appState = AppState(preview: true)
    static var previews: some View {
        FileClose().environmentObject(appState)
    }
}
