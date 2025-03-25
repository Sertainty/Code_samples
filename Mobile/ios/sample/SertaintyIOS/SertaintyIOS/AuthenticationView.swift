//
//  AuthenticationView.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import SwiftUI

struct AuthenticationView : View {
    
    @EnvironmentObject var appState:AppState
    
    @State private var response: String = ""
    
    var body: some View {
        NavigationView {
            VStack {
                FileClose()
                HStack {
                    Text("Prompt").frame(width: 100, alignment: .leading)
                    //Spacer()
                    Text(self.appState.chPrompt)
                    Spacer()
                }
                HStack {
                    Text("Response").frame(width: 100, alignment: .leading)
                    //Spacer()
                    TextField("Response", text: self.$response, onEditingChanged: { edit in
                        if !edit {
                        self.appState.onSubmit(self.response)
                        self.response = ""
                        }
                    })
                    Spacer()
                }
                Button (action: {
                    //self.appState.chResponse = self.response
                    self.appState.onSubmit(self.response)
                    self.response = ""
                }) {
                    Text("Submit")
                }
                Spacer()
            }
            .padding(20.0)
            .navigationBarTitle(Text("UXP Authentication"))
        }
    }
}

struct AuthenticationView_Previews: PreviewProvider {
    static let appState = AppState()
    static var previews: some View {
        AuthenticationView().environmentObject(appState)
    }
}
