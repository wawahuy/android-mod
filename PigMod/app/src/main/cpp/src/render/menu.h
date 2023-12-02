//
// Created by nguye on 10/30/2023.
//

#ifndef PIGMOD_MENU_RENDER_H
#define PIGMOD_MENU_RENDER_H

namespace MenuRenderer {
#ifdef IS_DEBUG
    static bool isFloating = false;
#else
    static bool isFloating = true;
#endif

    void renderMenuItem(MenuItemBaseAbstract* menuItem) {
        const std::string& fullAction = menuItem->fullAction;
        const std::string& argName = menuItem->argName;
        json& dataArgs = *menuItem->argData;

        switch (menuItem->type) {
            case WidgetMenuBaseType::Switch:
            case WidgetMenuBaseType::ServerSwitch:
            {
                MenuItemSwitch* mis = (MenuItemSwitch *) menuItem;
                if (!mis->label.empty()) {
                    ImGui::BulletText("%s", mis->label.c_str());
                }
                if (!mis->isNewLine) {
                    ImGui::SameLine();
                }

                bool active = mis->value;
                if (ToggleButton(mis->label.c_str(), &active)) {
                    mis->value = active;
                    dataArgs[argName] = mis->value;
                    Menu::runAction(mis);
                }
                break;
            }
            case WidgetMenuBaseType::Text:
            {
                MenuItemText* mis = (MenuItemText *) menuItem;
                ImGui::Text("%s", mis->label.c_str());
                break;
            }
            case WidgetMenuBaseType::Button:
            {
                MenuItemSwitch* mis = (MenuItemSwitch *) menuItem;
                if (ImGui::Button(mis->label.c_str())) {
                    Menu::runAction(mis);
                }
                break;
            }
            case WidgetMenuBaseType::InputInt:
            {
                MenuItemInputInt* mis = (MenuItemInputInt *) menuItem;
                if (!mis->isNewLine) {
                    float y = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(y + 10.0f);
                }
                if (!mis->label.empty()) {
                    ImGui::BulletText("%s", mis->label.c_str());
                }
                if (!mis->isNewLine) {
                    ImGui::SameLine();
                    float y = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(y - 10.0f);
                }

                ImGui::PushItemWidth(mis->valueWidth);
                int value = mis->value;
                bool isChange = false;
                if (ImGui::InputInt(mis->idImguiVisible.c_str(), &value, mis->valueStep, mis->valueStepFast)) {
                    isChange = true;
                }
                ImGui::PopItemWidth();

                if (isChange && (value < mis->valueMin || value > mis->valueMax)) {
                    isChange = false;
                }

                if (isChange) {
                    mis->value = value;
                    dataArgs[argName] = mis->value;
                    Menu::runAction(mis);
                }
                break;
            }
            case WidgetMenuBaseType::SliderFloat:
            {
                MenuItemSliderFloat* mis = (MenuItemSliderFloat *) menuItem;
                if (!mis->isNewLine) {
                    float y = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(y + 10.0f);
                }
                if (!mis->label.empty()) {
                    ImGui::BulletText("%s", mis->label.c_str());
                }
                if (!mis->isNewLine) {
                    ImGui::SameLine();
                    float y = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(y - 10.0f);
                }
                ImGui::PushItemWidth(mis->valueWidth);
                if (ImGui::SliderFloat(mis->idImguiVisible.c_str(), &mis->value, mis->valueMin, mis->valueMax)) {
                    dataArgs[argName] = mis->value;
                    Menu::runAction(mis);
                }
                ImGui::PopItemWidth();
                break;
            }
            case WidgetMenuBaseType::SliderInt:
            {
                MenuItemSliderInt* mis = (MenuItemSliderInt *) menuItem;
                if (!mis->isNewLine) {
                    float y = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(y + 10.0f);
                }
                if (!mis->label.empty()) {
                    ImGui::BulletText("%s", mis->label.c_str());
                }
                if (!mis->isNewLine) {
                    ImGui::SameLine();
                    float y = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(y - 10.0f);
                }
                ImGui::PushItemWidth(mis->valueWidth);
                if (ImGui::SliderInt(mis->idImguiVisible.c_str(), &mis->value, mis->valueMin, mis->valueMax)) {
                    dataArgs[argName] = mis->value;
                    Menu::runAction(mis);
                }
                ImGui::PopItemWidth();
                break;
            }
        }

        if (menuItem->isSameLine) {
            ImGui::SameLine();
        }
    }

    void RenderFloatButton() {
        ImGui::SetNextWindowPos({ g_ScreenWidth / 2.0f - 50.0f, 50.0f });
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::Begin("", &isFloating, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
        ImGui::SetNextWindowSize(ImVec2(300, 200));
        if (ImGui::Button(STR_BUTTON_FLOAT)) {
            isFloating = false;
        }
        ImGui::End();
    }

    void Render() {
        Menu::loopTypeCall();

        if (isFloating) {
            RenderFloatButton();
            return;
        }
        ImGui::SetNextWindowPos({ 25, 25 });
        ImGui::SetNextWindowSize(ImVec2(g_ScreenWidth - 50, 0));
        ImGui::Begin(STR_WINDOWS_MENU);
        renderSystemMessage();

        if (ImGui::Button(STR_BUTTON_HIDE_MENU)) {
            isFloating = true;
        }

        ImGui::SameLine();
        if (ImGui::Button(STR_BUTTON_LOGOUT)) {
            Socket::handleLogout();
        }

        if (g_MenuInit) {
            for (auto menuGroupItem: Menu::menu) {
                ImGui::SeparatorText(menuGroupItem->label.c_str());
                for (auto menuItem: menuGroupItem->items) {
                    renderMenuItem(menuItem);
                    if (menuItem->args.size() == 0) {
                        continue;
                    }
                    if ((menuItem->type == WidgetMenuBaseType::Switch || menuItem->type == WidgetMenuBaseType::ServerSwitch) &&
                        !((MenuItemSwitch*)menuItem)->value) {
                        continue;
                    }

                    auto p = ImGui::GetCursorScreenPos();
                    p.x += 50;
                    ImGui::SetCursorScreenPos(p);
                    ImGui::BeginGroup();
                    ImGui::NewLine();
                    for (auto menuItemArg: menuItem->args) {
                        renderMenuItem(menuItemArg);
                    }
                    ImGui::NewLine();
                    ImGui::EndGroup();
                    auto min = ImGui::GetItemRectMin();
                    min.x -= 30;
                    auto max = ImGui::GetItemRectMax();
                    max.x += 30;
                    ImGui::GetWindowDrawList()->AddRect(min, max, IM_COL32_WHITE);
                    ImGui::NewLine();
                }
            }

            ImGui::Separator();
            ImGui::Text("%s", Menu::menuInfo.hash.c_str());

        } else {
            ImGui::Text(STR_MENU_STARTING);
        }

        ImGui::End();
    }
}

#endif //PIGMOD_MENU_RENDER_H
