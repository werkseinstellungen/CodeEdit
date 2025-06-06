//
//  StartTaskToolbarButton.swift
//  CodeEdit
//
//  Created by Austin Condiff on 8/4/24.
//

import SwiftUI

struct StartTaskToolbarButton: View {
    @Environment(\.controlActiveState)
    private var activeState

    @UpdatingWindowController var windowController: CodeEditWindowController?

    @ObservedObject var taskManager: TaskManager
    @EnvironmentObject var workspace: WorkspaceDocument

    var utilityAreaCollapsed: Bool {
        windowController?.workspace?.utilityAreaModel?.isCollapsed ?? true
    }

    var body: some View {
        HStack {
            Button {
                if let workspaceURL = workspace.fileURL {
                    PlaydateCompiler.compile(workspaceURL)
                }
            } label: {
                Label("Build for Playdate", systemImage: "hammer.fill")
                    .labelStyle(.iconOnly)
                    .opacity(activeState == .inactive ? 0.5 : 1.0)
                    .font(.system(size: 18, weight: .regular))
                    .frame(width: 28)
                    .offset(CGSize(width: 0, height: 2.5))
            }
            .keyboardShortcut("B", modifiers: .command)
            Button {
                taskManager.executeActiveTask()
                if utilityAreaCollapsed {
                    CommandManager.shared.executeCommand("open.drawer")
                }
                workspace.utilityAreaModel?.selectedTab = .debugConsole
                taskManager.taskShowingOutput = taskManager.selectedTaskID
            } label: {
                Label("Start", systemImage: "play.fill")
                    .labelStyle(.iconOnly)
                    .opacity(activeState == .inactive ? 0.5 : 1.0)
                    .font(.system(size: 18, weight: .regular))
                    .help("Start selected task")
                    .frame(width: 28)
                    .offset(CGSize(width: 0, height: 2.5))
            }
            .keyboardShortcut("R", modifiers: .command)
        }
    }
}
