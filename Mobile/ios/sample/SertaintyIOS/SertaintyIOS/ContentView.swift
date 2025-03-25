//
//  ContentView.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/12/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import SwiftUI

struct ContentView: View {
    @EnvironmentObject var appState:AppState
    @State var showAuth = false
     
    var body: some View {
        GeometryReader { geometry in
            NavigationView {
                ZStack(alignment: .center) {
                    VStack(spacing: 20.0) {
                        StatusView(StatusItem("Library Status","Not Initialized","Initialized","Initialize Failed",self.appState.libraryStatus))
                        StatusView(StatusItem("Create New ID",self.appState.newIdStatus))
                        StatusView(StatusItem("Create New File",self.appState.newFileStatus))
                        StatusView(StatusItem("Compare Contents",self.appState.compareFilesStatus))
                        HStack {
                            Button(action: {
                                self.appState.reset()
                                self.appState.createNewID()
                                self.appState.createNewFile()
                            }) {
                                Text("Run Sample")
                                Spacer()
                            }
                            .frame(width: 100.0)
                            .disabled(self.appState.libraryStatus != 1)
                            Spacer()
                            Button(action: {
                                self.appState.openFile()
                            }) {
                                Text("Open File")
                            }
                            .frame(width: 100.0)
                            .disabled(self.appState.libraryStatus != 1)
                            Spacer()
                            Button(action: {
                                self.appState.reset()
                            }) {
                                Spacer()
                                Text("Reset")
                            }
                            .frame(width: 100.0)
                        }
                        Button(action: {
                            self.appState.installLicense()
                        }) {
                            Text("Install New License")
                        }
                        Spacer()
                    }
                    .padding(.all)
                    .disabled(self.appState.loading)
                    .blur(radius: self.appState.loading ? 3 : 0)
                    VStack {
                        Text("\(self.appState.loadingText)...")
                            .padding(10)
                            .font(.system(size:30))
                    }
                    .frame(width: geometry.size.width / 2,
                           height: geometry.size.height / 5)
                    .background(Color.gray)
                    .cornerRadius(20)
                    .shadow(radius: 20)
                    .opacity(self.appState.loading ? 1:0)
                }.navigationBarTitle(Text("Sertainty iOS Demo"))
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static let appState = AppState(preview: true)
    static var previews: some View {
        ContentView().environmentObject(appState)
    }
}
