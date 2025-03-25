//
//  WebView.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/30/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import SwiftUI
import WebKit
  
struct WebView : UIViewRepresentable {
      
    let request: URLRequest
      
    func makeUIView(context: Context) -> WKWebView  {
        return WKWebView()
    }
      
    func updateUIView(_ uiView: WKWebView, context: Context) {
        uiView.load(request)
    }
}
  
#if DEBUG
struct WebView_Previews : PreviewProvider {
    static var previews: some View {
        WebView(request: URLRequest(url: URL(string: "https://www.apple.com")!))
    }
}
#endif
