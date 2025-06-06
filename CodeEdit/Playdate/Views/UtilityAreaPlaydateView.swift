//
//  UtilityAreaPlaydateView.swift
//  CodeEdit
//
//  Created by Karl Ehrlich on 05.06.25.
//

import SwiftUI

struct UtilityAreaPlaydateView: View {
    @EnvironmentObject private var utilityAreaViewModel: UtilityAreaViewModel
    var body: some View {
        UtilityAreaTabView(model: utilityAreaViewModel.tabViewModel) { _ in
            HStack {
                Image("playdate-simulator")
                    .resizable()
                    .frame(width: 128, height: 128)
                VStack(alignment: .leading) {
                    Text("Playdate SDK")
                        .font(.title2)
                    HStack {
                        Text("Build & Run")
                        HStack(spacing: 5) {
                            Image(systemName: "command")
                            Text("B")
                        }
                        .font(.subheadline)
                        .foregroundStyle(.secondary)
                    }
                }
            } .frame(maxWidth: .infinity)
        }
    }
}
