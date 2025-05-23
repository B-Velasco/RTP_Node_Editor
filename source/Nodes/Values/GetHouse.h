#pragma once
#include "GetItem.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <random>

class Node_GetHouse : public Node_GetItem
{


public:
    Node_GetHouse(int id) : Node_GetItem(id) { // Call the base class constructor  
		ID = id;
		Name = "Get House";
		Color = ImColor(150, 200, 255, 150);
		Outputs.emplace_back(rand(), "House", PinType::Item);
		Outputs.back().Kind = PinKind::Output;
		Outputs.back().Node = this;

		pinataNames = csv::Load("Pinatas", 0);
		for (auto& pinata : pinataNames) {
			pinata = pinata + " House";
		}
    }

	void Render() {
		util::BlueprintNodeBuilder builder(g_HeaderTexture, 128, 128);
		builder.Begin(ID);
		float pinStartX = ImGui::GetCursorPosX();
		// Header with node-specific color
		builder.Header(Color);
		ImGui::TextUnformatted(Name.c_str());
		builder.EndHeader();
		ImGui::SetCursorPosX(pinStartX);
		std::string preview = pinataNames[Data];
		ImGui::Text("%s ", preview.c_str());
		for (auto& input : Inputs) {
			builder.Input(input.ID);
			ImGui::PushStyleColor(ImGuiCol_Text, GetPinColor(input.Type));
			ImGui::TextUnformatted(("| " + input.Name).c_str());
			ImGui::PopStyleColor();
			builder.EndInput();
		}
		for (auto& output : Outputs) {
			builder.Output(output.ID);
			ImGui::PushStyleColor(ImGuiCol_Text, GetPinColor(output.Type));
			ImGui::TextUnformatted((output.Name + " >").c_str());
			ImGui::PopStyleColor();
			builder.EndOutput();
		}
		builder.End();
	}

	void RenderDetails() override {
		if (ImGui::BeginCombo("Pinata", pinataNames[Data].c_str())) {
			for (int i = 0; i < pinataNames.size(); ++i) {
				bool isSelected = (Data == i);
				if (ImGui::Selectable(pinataNames[i].c_str(), isSelected)) {
					Data = i;
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	std::string GetSelectedPinata() {
		return pinataNames[Data];
	}

private:
    std::vector<std::string> pinataNames;
};