//
//  AppState.swift
//  SampleSwiftUI
//
//  Created by Karim Lalani on 1/21/20.
//  Copyright © 2020 Sertainty. All rights reserved.
//

import Foundation
import UIKit
import SwiftUI
import WebKit

class DocumentPickerDelegate : NSObject, UIDocumentPickerDelegate {
    var appState:AppState?
    func registerAppState(appState:AppState)
    {
        self.appState = appState
    }
    func documentPicker(_ controller: UIDocumentPickerViewController, didPickDocumentAt url: URL) {
        print("On Document Picker")
        print(controller.documentPickerMode)
        print(url)
        guard let appState = self.appState else {
            fatalError("App State not assigned")
        }
        if controller.documentPickerMode == .import {
            print("Imported: \(url.path)")
            print("Sandbox: \(appState.getSandbox().path)")
            do {
                let newUrl = appState.getSandbox().appendingPathComponent(url.lastPathComponent)
                if FileManager.default.fileExists(atPath: newUrl.path) {
                    try FileManager.default.removeItem(at: newUrl)
                }
                try FileManager.default.moveItem(at: url, to: appState.getSandbox().appendingPathComponent(url.lastPathComponent))
            }
            catch let error
            {
                print(error.localizedDescription)
                return
            }
            appState.openUxp(url: appState.getSandbox().appendingPathComponent(url.lastPathComponent))
            print("Opening: \(appState.uxpFile!.url!)")
            guard let file = appState.uxpFile else  {
                fatalError("Failed to open UXP File")
            }
            
            let callStatus = file.getCallStatus()
            if callStatus.hasError() {
                print("Unable to open file: \(callStatus.getErrorMessage())")
            }
            else {
                appState.viewPage = .Auth
                appState.processAuth()
            }
        } else if controller.documentPickerMode == .exportToService {
            appState.uxpFile = nil
            appState.virtualFileList = nil
            appState.viewPage = .Home
        }
    }
}

class VirtualFilePickerDelegate : NSObject, UIDocumentPickerDelegate {
    var appState:AppState?
    func registerAppState(appState:AppState)
    {
        self.appState = appState
    }
    func documentPicker(_ controller: UIDocumentPickerViewController, didPickDocumentAt url: URL) {
        print("On Document Picker")
        print(controller.documentPickerMode)
        print(url)
        guard let appState = self.appState else {
            fatalError("App State not assigned")
        }
        if controller.documentPickerMode == .import {
            appState.addVirtualFile(vfileUrl: url)
        }
    }
}

class AppState : ObservableObject {
    
    @Published private(set) var newIdStatus = 0
    private var newIdStatus_internal = 0
    @Published private(set) var newFileStatus = 0
    @Published private(set) var compareFilesStatus = 0
    
    @Published private(set) var libraryStatus = 0
    
    @Published var loading: Bool = true
    @Published var loadingText: String = ""
    
    private var challenge:Challenge?
    @Published private(set) var chidx:Int32 = 0
    @Published var chPrompt: String = ""
    @Published var virtualDir: String = "/"
    @Published var viewPage:Page = .Home
    @Published private(set) var fileToOpen: URL?
    
    var documentPickerDelegate = DocumentPickerDelegate()
    var virtualFilePickerDelegate = VirtualFilePickerDelegate()
    
    private func setLoading(status:Bool) {
        DispatchQueue.main.async {
            self.loading = status
        }
    }
    
    private func setLoadingText(text:String) {
        DispatchQueue.main.async {
            self.loadingText = text
        }
    }
    
    private func setNewIdStatus(status:Int) {
        self.newIdStatus_internal = status
        DispatchQueue.main.async {
            self.newIdStatus = status
        }
    }
    
    private func setNewFileStatus(status:Int){
        DispatchQueue.main.async {
            self.newFileStatus = status
        }
    }
    
    private func setCompareFilesStatus(status:Int) {
        DispatchQueue.main.async {
            self.compareFilesStatus = status
        }
    }
    
    private var exampleRoot: String = "";
    private var idXmlSpec: String {
        return "\(exampleRoot)/sampleid.xml"
    }
    private var idFileSpec: String {
        return "\(exampleRoot)/sampleid.iic"
    }
    private var uxpFileSpec: String {
        return "\(exampleRoot)/sample.uxp"
    }
    private var dataPdfSpec: String {
        return "\(exampleRoot)/data.pdf"
    }
    private var dataPdf2Spec: String {
        return "\(exampleRoot)/data2.pdf"
    }
    private var copy1Spec: String {
        return "\(exampleRoot)/copy1.pdf"
    }
    private var copy2Spec: String {
        return "\(exampleRoot)/copy2.pdf"
    }
    private var licenseFile: String {
        return "\(exampleRoot)/sertainty.lic"
    }
    
    var uxpFile: UxpFile?
    var virtualFileList: [VirtualFileInfo]?
    var rootViewController: UIViewController?
    
    private let runSample = DispatchQueue(label:"Run Sample")
    
    init(){
        
    }
    
    init(preview:Bool) {
        if (preview) {
            self.loading = false
            self.fileToOpen = getLocalStorage().appendingPathComponent("/example/data.pdf")
            print(self.fileToOpen)
        }
    }
    
    func getLocalStorage() -> URL {
        guard let storage = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first else {
            fatalError("Unable to resolve .documentDirectory in .userDomainMask")
        }
        return storage
    }
    
    func getSandbox() -> URL {
        return getLocalStorage().appendingPathComponent("Sandbox/Items", isDirectory: true)
    }
    
    func openUxp(url:URL){
        uxpFile = UxpFile.open(url, 1)
    }
    
    func closeUxp() {
        guard
            let rootView = rootViewController,
            let file = uxpFile,
            let fileUrl = file.url
        else {
            return
        }
        if FileManager.default.fileExists(atPath: fileUrl.path) {
            print("Pre close: File not found locally: \(fileUrl)")
        }
        
        print("Closing: \(fileUrl.path)")
        if file.isOpen() {
            file.close()
        }
        
        virtualFileList = nil
        
        if FileManager.default.fileExists(atPath: fileUrl.path) {
            let controller = UIDocumentPickerViewController(url: fileUrl, in: .exportToService)
            documentPickerDelegate.registerAppState(appState: self)
            controller.delegate = documentPickerDelegate
            rootView.present(controller,animated: true)
        } else {
            print("File not found locally: \(fileUrl)")
        }
    }
    
    private func installFiles() {
        self.loadingText = "Installing Assets"
        var isDirectory = ObjCBool(true)
        let localStorage = getLocalStorage()
        let sertainty = localStorage.appendingPathComponent("Sertainty")
        let sandbox = getSandbox()
        if !FileManager.default.fileExists(atPath: sertainty.path, isDirectory: &isDirectory) {
            //print("Sertainty doesn't exist in Documents")
            do
            {
                guard let resourceUrl = Bundle.main.resourceURL else {
                    fatalError("Unable to resolve Bundle.main.resourceURL")
                }
                
                let sertaintyBundle = resourceUrl.appendingPathComponent("Sertainty")
                try FileManager.default.copyItem(atPath: sertaintyBundle.path, toPath: sertainty.path)
            }
            catch let error
            {
                print(error.localizedDescription)
            }
        }
        
        if !FileManager.default.fileExists(atPath: sandbox.path, isDirectory: &isDirectory) {
            do {
            try FileManager.default.createDirectory(at: sandbox, withIntermediateDirectories: true, attributes: nil)
                print("Created Sandbox")
            }
            catch let error
            {
                print(error.localizedDescription)
            }
        }
        
        //listContents(at: sertainty)
    }
    
    private func listContents(at: URL) {
        //print("At: \(at.lastPathComponent)")
        do {
            for item in try FileManager.default.contentsOfDirectory(at: at, includingPropertiesForKeys: []){
                //print("Found: \(item.lastPathComponent)")
                if item.hasDirectoryPath {
                    listContents(at: item)
                }
            }
        }
        catch let error as NSError
        {
            print(error.localizedDescription)
        }
    }
    
    func initLibrary () {
        self.loading = true
        self.installFiles()
        self.loadingText = "Initializing Library"
        guard Sys.getAppRoot() != nil else {
            fatalError("App Root not set for Sertainty")
        }
        //print("App Root: \(appRoot)")
                    
        let localStorage = getLocalStorage()
        
        exampleRoot = localStorage.appendingPathComponent("/Sertainty/example").path
            
        let baError = ByteArray()
        
        Sys.initLibrary(baError: baError, appKey: "IOS App",prefix: "IOS App",version: "1.0.0", onInit: {
            //print("Library Initialized")
            self.libraryStatus = 1
            self.loading = false
        }, onFail: {
            self.libraryStatus = -1
            print("Library not Initialized")
            print(baError.getString())
            self.loading = false
        })
    }
    
    func createNewID () {
        runSample.async {
            //print("Running ID")
            var status = -1
            self.setLoading(status:true)
            self.setLoadingText(text:"Creating New ID")
            
            if(self.libraryStatus == 1)
            {
                let callStatus = CallStatus()
                let buffer = ByteArray()
                Sys.fileReadAll(callStatus,self.idXmlSpec,buffer)
                if callStatus.hasError()
                {
                    print("Error reading file: \(self.idXmlSpec), \(callStatus.getErrorMessage())")
                }
                else
                {
                    //print("File read: \(self.idXmlSpec)")
                    guard let doc = buffer.getString() else {
                        return
                    }
                    
                    //print("Creating Id: \(self.idFileSpec) from \(self.idXmlSpec)")
                    Id.publishToFile(callStatus, self.idFileSpec, doc, 1)
                    
                    if callStatus.hasError() {
                        print("Error creating ID: \(self.idFileSpec), \(callStatus.getErrorMessage())")
                    }
                    else
                    {
                        status = 1
                        //print("Id file: \(self.idFileSpec) created")
                    }
                }
            }
            
            self.setNewIdStatus(status: status)
            
            self.setLoading(status:false)
        }
    }
    
    func createNewFile() {
        
        runSample.async {
            //print("Running File")
            var status = -1
            var compareStatus = 0
            self.setLoading(status:true)
            self.setLoadingText(text:"Create new File")
            
            if self.newIdStatus_internal == 1 {
                let file = UxpFile.create(URL(fileURLWithPath: self.uxpFileSpec), self.idFileSpec, 3, 1, 0)
                let callStatus = file.getCallStatus()
                
                if callStatus.hasError() {
                    //print("Error creating file: \(self.uxpFileSpec), \(callStatus.hasError())")
                }
                else{
                    //print("File: \(self.uxpFileSpec) created")
                    file.addVirtualFromFile("data.pdf", self.dataPdfSpec, -1, -1, 8)
                    
                    if callStatus.hasError() {
                        //print("Error creating virtual file: \(self.dataPdfSpec), \(callStatus.hasError())")
                    }
                    else{
                        //print("Virtual File: data.pdf created")
                        file.addVirtualFromFile("data2.pdf", self.dataPdf2Spec, -1, -1, 8)
                        
                        if callStatus.hasError() {
                            //print("Error creating virtual file: \(self.dataPdf2Spec), \(callStatus.hasError())")
                        }
                        else{
                            //print("Virtual File: data2.pdf created")
                        
                            status = 1
                            compareStatus = -1
                            self.setLoadingText(text: "Comparing Contents")
                            let vfHandle = file.openVirtualFile("data.pdf", 1)
                            
                            if callStatus.hasError() {
                                //print("Error opening virtual file: \(callStatus.getErrorMessage())")
                            }
                            else {
                                //print("Virtual file opened")
                                
                                let manager = FileManager.default
                                
                                if manager.fileExists(atPath: self.copy1Spec) {
                                    do {
                                        try manager.removeItem(atPath: self.copy1Spec)
                                        //print("copy1.pdf deleted")
                                        
                                    }
                                    catch let error {
                                        print("error deleting copy1.pdf")
                                        print("Error: \(error.localizedDescription)")
                                    }
                                }
                                
                                let fileBuffer = ByteArray()
                                let tmpBuffer = ByteArray()
                                while vfHandle.read(tmpBuffer, 10000) > 0 {
                                    fileBuffer.appendByteArray(tmpBuffer)
                                }
                                
                                vfHandle.close()
                                
                                manager.createFile(atPath: self.copy1Spec, contents: fileBuffer.getData(), attributes: nil)
                                
                                if file.compareExternalFile("data.pdf", self.copy1Spec) {
                                    //print("Comparison of data.pdf to copy1.pdf successful")
                                    compareStatus = 1
                                }
                                else {
                                    print("Comparison of data.pdf to copy1.pdf failed")
                                }
                            }
                        }
                    }
                }
                
                if file.isOpen() {
                    file.close()
                }
                
                
            }
            self.setNewFileStatus(status:status)
            self.setCompareFilesStatus(status: compareStatus)
            self.setLoading(status:false)
            
            DispatchQueue.main.async {
                self.saveFile()
            }
        }
    }
    
    public func openFile() {
        
        guard let rootView = self.rootViewController else { return }
        
        self.uxpFile = nil
        
        let controller = UIDocumentPickerViewController(documentTypes: ["public.item"], in: .import)
        controller.allowsMultipleSelection = false
        documentPickerDelegate.registerAppState(appState: self)
        controller.delegate = documentPickerDelegate
        rootView.present(controller,animated: true)
    }
    
    public func addVirtualFile(vfileUrl: URL) -> Bool
    {
        guard let file = self.uxpFile else { return false }
        file.addVirtualFromFile(vfileUrl.lastPathComponent, vfileUrl.path, -1, -1, 8)
        self.virtualFileList = file.getVirtualDirectoryArray(self.virtualDir, 0)
        self.viewPage = .File
        return true
    }
    
    public func saveFile() {
        guard
            let rootView = self.rootViewController
        else {
            return
        }
        let fileUrl = URL(fileURLWithPath: uxpFileSpec)
        let controller = UIDocumentPickerViewController(url: fileUrl, in: UIDocumentPickerMode.exportToService)
        rootView.present(controller, animated: true)
    }
    
    public func processAuth() {
        guard let file = uxpFile else {
            print("UXP not open")
            return
        }
        
        let status = file.authenticate()
        
        if file.isAuthorized() {
            //print("Authorized")
            self.viewPage = .File
            self.virtualFileList = self.uxpFile!.getVirtualDirectoryArray(self.virtualDir, 0)
        }
        else {
            if status != .StatusChallenged {
                print("Not authorized")
            } else {
                setChidx(0)
                loadNextChallenge()
            }
        }
    }
    
    func setChidx(_ idx: Int32) {
        self.chidx = idx
    }
    
    func loadNextChallenge() {
        guard let file = self.uxpFile else {
            print("UXP not open")
            return
        }
        challenge = file.getChallenge(chidx)
        guard let ch = challenge else {
            fatalError("Unable to retrieve challenge")
        }
        
        guard let prompt = ch.prompt else {
            fatalError("Unabl to retrieve challenge prompt")
        }
        
        self.chPrompt = prompt
        //print("Prompt: \(prompt)")
        
        ch.startTimer()
    }
    
    func onSubmit(_ response: String) {
        guard let file = self.uxpFile else {
            return
        }
        
        if file.isOpen() && !file.isAuthorized() {
            guard let ch = challenge else {
                print("No active challenge challenge")
                return
            }
            
            //print("Response: \(response)")
            ch.setValueString(response)
            ch.endTimer()
            file.addResponse(ch)
            
            if chidx < file.getChallengeCount() - 1 {
                setChidx(chidx + 1)
                loadNextChallenge()
            }
            else {
                processAuth()
            }
        }
    }
    
    func reset() {
        // Delete local files
        self.newIdStatus = 0
        self.newFileStatus = 0
        self.compareFilesStatus = 0
    }
    
    func installLicense() {
        self.loading = true
        self.loadingText = "Installing License"
        let buffer = ByteArray()
        
        Sys.installLicense(baError: buffer, licenseFile: licenseFile, onInstall: {
            //print("License Installed")
            self.initLibrary()
        }, onFail: {
            print("Unable to Install License: \(String(describing: buffer.getString()))")
            self.loading = false
        })
    }
    
    func openVirtualFile(vfile: VirtualFileInfo) {
        guard let ufile = self.uxpFile else {
            return
        }
        let manager = FileManager.default
        
        let storage =  manager.temporaryDirectory
        let vFilePathUrl = storage.appendingPathComponent(vfile.name!)
        let vFilePath = vFilePathUrl.path
        if manager.fileExists(atPath: vFilePath) {
            do {
                try manager.removeItem(at: vFilePathUrl)
            } catch let error {
                print("error deleting file")
                print("Error: \(error.localizedDescription)")
                return
            }
        }
        
        let data = ufile.readAllVirtualFile(vfile.fullName!)
        
        manager.createFile(atPath: vFilePath, contents: data.getData(), attributes: nil)
        
        if UIApplication.shared.canOpenURL(vFilePathUrl) {
            self.fileToOpen = vFilePathUrl
            self.viewPage = .URL
        }
    }
}
