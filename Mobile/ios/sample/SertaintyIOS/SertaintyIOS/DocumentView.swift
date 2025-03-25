//
//  DocumentView.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/30/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import SwiftUI

struct DocumentView: View {
    @EnvironmentObject var appState:AppState
    var body: some View {
        NavigationView {
            VStack{
                HStack{
                    Button(action:{
                        self.appState.viewPage = .File
                        if let fileToOpen = self.appState.fileToOpen {
                            if FileManager.default.fileExists(atPath: fileToOpen.path) {
                                do {
                                    try FileManager.default.removeItem(at: fileToOpen)
                                }
                                catch let error {
                                    print("Failed to delete file \(fileToOpen.path)")
                                    print("Error: \(error.localizedDescription)")
                                }
                            }
                        }
                    }){Text("Back")}.padding(.leading,20)
                    Spacer()
                    Button(action:{
                        if let fileToSave = self.appState.fileToOpen {
                            if let rootView = self.appState.rootViewController {
                                let controller = UIDocumentPickerViewController(url: fileToSave, in: UIDocumentPickerMode.exportToService)
                                rootView.present(controller,animated: true)
                            }
                        }
                    }){Text("Save")}.padding(.trailing,20)
                }
                WebView(request: URLRequest(url:self.appState.fileToOpen!))
            }.navigationBarTitle("Document Viewer")
        }
    }
}

struct DocumentView_Previews: PreviewProvider {
    static let appState = AppState(preview: true)
    static var previews: some View {
        DocumentView().environmentObject(appState)
    }
}
