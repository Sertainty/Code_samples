//
//  AppView.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/22/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import SwiftUI

enum Page {
    case Home
    case Auth
    case File
    case URL
}

struct AppView: View {
    @EnvironmentObject var appState:AppState
    
    var body: some View {
        Group {
            if self.appState.viewPage  == .Auth {
                AuthenticationView().onAppear {
                    guard let file = self.appState.uxpFile else {
                        print("Switching to Home from Auth")
                        self.appState.viewPage = .Home
                        return
                    }
                    if file.isAuthorized() {
                        print("Switching to File from Auth")
                        self.appState.viewPage = .File
                    }
                }
            }
            else if self.appState.viewPage == .File {
                UXPContentView()
            }
            else if self.appState.viewPage == .URL {
                DocumentView()
            }
            else {
                ContentView()
            }
            
        }.navigationViewStyle(StackNavigationViewStyle())
    }
}

struct AppView_Previews: PreviewProvider {
    static let appState = AppState(preview: true)
    static var previews: some View {
        AppView().environmentObject(appState)
    }
}
