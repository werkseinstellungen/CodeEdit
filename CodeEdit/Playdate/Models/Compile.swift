//
//  Compile.swift
//  CodeEdit
//
//  Created by Karl Ehrlich on 06.06.25.
//

import Foundation

class PlaydateCompiler {
    static func compile(_ rootURL: URL) {
        let fileManager = FileManager.default
        let gameName = rootURL.lastPathComponent
        let buildDir = rootURL.appendingPathComponent(".pdbuild")
        let outputDir = buildDir.appendingPathComponent("\(gameName).pdx")

        // Lookup bundle resources immediately
        guard let resourcesURL = Bundle.main.resourceURL else {
            print("Playdate Compiler: Could not find bundle resources")
            return
        }

        let pdcURL = resourcesURL.appending(path: "pdc")
        let simulatorAppURL = resourcesURL.appending(path: "Playdate Simulator.app")
        
        let simulatorExecURL = URL(fileURLWithPath: "/usr/bin/open")

        // Ensure build dir exists
        try? fileManager.createDirectory(at: buildDir, withIntermediateDirectories: true)

        // Compile with pdc
        let build = Process()
        build.executableURL = pdcURL
        build.arguments = [rootURL.path, outputDir.path]

        do {
            try build.run()
            build.waitUntilExit()
        } catch {
            print("Playdate Compiler: failed: \(error)")
            return
        }

        guard build.terminationStatus == 0 else {
            print("Playdate Compiler: pdc exited with status \(build.terminationStatus)")
            return
        }

        print("Playdate Compiler: Compiled to \(outputDir.path)")

        // Launch simulator
        let launch = Process()
        launch.executableURL = simulatorExecURL
        launch.arguments = ["-a", simulatorAppURL.path, outputDir.path]

        do {
            try launch.run()
            print("Playdate Simulator: Launched simulator with game.")
        } catch {
            print("Playdate Simulator: Failed to launch simulator: \(error)")
        }
    }
}
