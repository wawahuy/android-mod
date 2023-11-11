//
// Created by nguye on 10/30/2023.
//

#ifndef PIGMOD_MENU_H
#define PIGMOD_MENU_H

enum WidgetMenuBaseType {
    Switch = 1,
    InputInt = 2,
    SliderFloat = 3,
};

template<typename T>
struct MenuMaxMinBaseTemplate {
    T valueMax;
    T valueMin;
    T valueStep;
    T valueStepFast;
    T valueWidth;
};

template<typename T>
struct MenuValueBaseTemplate {
    T defaultValue;
    T value;
    bool isNewLine;
    std::string idImguiVisible;
};

struct MenuItemBaseAbstract {
    std::string label;
    std::string action;
    std::string fullAction;
    WidgetMenuBaseType type;
    std::string argName;
    json* argData;
};

struct MenuItemBase: MenuItemBaseAbstract {
    std::vector<MenuItemBaseAbstract*> args;
};

template<typename T>
struct MenuItemTemplate: MenuItemBase, MenuValueBaseTemplate<T> {
};

struct MenuItemSwitch: MenuItemTemplate<bool> {};
struct MenuItemInputInt: MenuItemTemplate<int>, MenuMaxMinBaseTemplate<int> {};
struct MenuItemSliderFloat: MenuItemTemplate<float>, MenuMaxMinBaseTemplate<float> {};

struct MenuGroup {
    std::string label;
    std::string action;
    std::vector<MenuItemBase*> items;
};

namespace MenuRenderer {
#ifdef IS_DEBUG
    static bool isFloating = false;
#else
    static bool isFloating = true;
#endif

    static std::vector<MenuGroup*> menu;

    void renderMenuItem(MenuItemBaseAbstract* menuItem) {
        const std::string& fullAction = menuItem->fullAction;
        const std::string& argName = menuItem->argName;
        json& dataArgs = *menuItem->argData;

        switch (menuItem->type) {
            case WidgetMenuBaseType::Switch:
            {
                MenuItemSwitch* mis = (MenuItemSwitch *) menuItem;
                ImGui::BulletText("%s", mis->label.c_str());
                if (!mis->isNewLine) {
                    ImGui::SameLine();
                }

                bool active = mis->value;
                if (ToggleButton(mis->label.c_str(), &active)) {
                    mis->value = active;
                    dataArgs[argName] = mis->value;
                    LibIj::runAction(fullAction, dataArgs);
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
                ImGui::BulletText("%s", mis->label.c_str());
                if (!mis->isNewLine) {
                    ImGui::SameLine();
                    float y = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(y - 10.0f);
                }

                ImGui::PushItemWidth(mis->valueWidth);
                int value = mis->value;
                if (ImGui::InputInt(mis->idImguiVisible.c_str(), &value, mis->valueStep, mis->valueStepFast)) {
                    dataArgs[argName] = mis->value;
                    LibIj::runAction(fullAction, dataArgs);
                }
                ImGui::PopItemWidth();

                if (value >= mis->valueMin && value <= mis->valueMax) {
                    mis->value = value;
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
                ImGui::BulletText("%s", mis->label.c_str());
                if (!mis->isNewLine) {
                    ImGui::SameLine();
                    float y = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(y - 10.0f);
                }
                ImGui::PushItemWidth(mis->valueWidth);
                if (ImGui::SliderFloat(mis->idImguiVisible.c_str(), &mis->value, mis->valueMin, mis->valueMax)) {
                    dataArgs[argName] = mis->value;
                    LibIj::runAction(fullAction, dataArgs);
                }
                ImGui::PopItemWidth();
                break;
            }
        }
    }

    void init() {
        auto js = new json();
        auto menuItem1 = new MenuItemSwitch;

        auto menuItem1c2 = new MenuItemSliderFloat;
        menuItem1c2->label = "Slider";
        menuItem1c2->argName = "value";
        menuItem1c2->fullAction = "test_group.item_switch";
        menuItem1c2->defaultValue = 5;
        menuItem1c2->valueMin = 1;
        menuItem1c2->valueMax = 10;
        menuItem1c2->valueStep = 0.1f;
        menuItem1c2->valueStepFast = 0.5f;
        menuItem1c2->valueWidth = 300.0f;
        menuItem1c2->value = menuItem1c2->defaultValue;
        menuItem1c2->type = WidgetMenuBaseType::SliderFloat;
        menuItem1c2->idImguiVisible = "##" + menuItem1c2->fullAction + menuItem1c2->argName;
        menuItem1c2->isNewLine = true;
        menuItem1c2->argData = js;

        menuItem1->label = "Item switch";
        menuItem1->action = "item_switch";
        menuItem1->argName = "this";
        menuItem1->fullAction = "test_group.item_switch";
        menuItem1->defaultValue = true;
        menuItem1->value = menuItem1->defaultValue;
        menuItem1->type = WidgetMenuBaseType::Switch;
        menuItem1->args.push_back(menuItem1c2);
        menuItem1->argData = js;

        auto menuGroup = new MenuGroup;
        menuGroup->label = "Test Group";
        menuGroup->action = "test_group";
        menuGroup->items.push_back((MenuItemBase*)menuItem1);

        menu.push_back(menuGroup);
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
        if (isFloating) {
            RenderFloatButton();
            return;
        }
        ImGui::SetNextWindowPos({ 25, 25 });
        ImGui::SetNextWindowSize(ImVec2(g_ScreenWidth - 50, 0));
        ImGui::Begin(STR_WINDOWS_MENU);
        renderSystemMessage();

        for (auto menuGroupItem: menu) {
            ImGui::SeparatorText(menuGroupItem->label.c_str());
            for (auto menuItem: menuGroupItem->items) {
                renderMenuItem(menuItem);
                if (menuItem->args.size() == 0) {
                    continue;
                }
                if (menuItem->type == WidgetMenuBaseType::Switch && !((MenuItemSwitch*)menuItem)->value) {
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

        if (ImGui::Button(STR_BUTTON_HIDE_MENU)) {
            isFloating = true;
        }

        ImGui::SameLine();
        if (ImGui::Button(STR_BUTTON_LOGOUT)) {
            Game::release();
            g_AuthStage = AuthStage::None;
        }

        if (g_MenuInit) {
            for (auto &ggp: Game::guiGroupPatchArray) {
                ImGui::SeparatorText(ggp->name.c_str());
                for (auto &gp: ggp->dataPatchArray) {
                    ImGui::BulletText("%s", gp->name.c_str());

                    ImGui::SameLine();
                    bool active = gp->active;
                    if (ToggleButton(gp->name.c_str(), &active)) {
                        if (active) {
                            Game::patch(gp);
                        } else {
                            Game::unpatch(gp);
                        }
                    }
                }
            }

            ImGui::Separator();
            ImGui::Text("%s", Game::menuInfo.hash.c_str());

        } else {
            ImGui::Text(STR_MENU_STARTING);
        }

        ImGui::End();
    }
}

#endif //PIGMOD_MENU_H
