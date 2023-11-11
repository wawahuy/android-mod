//
// Created by nguye on 11/12/2023.
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
    T valueDefault;
    T value;
};

struct MenuItemBaseAbstract {
    std::string label;
    std::string action;
    std::string fullAction;
    WidgetMenuBaseType type;
    std::string argName;
    json* argData;
    bool isNewLine;
    std::string idImguiVisible;
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

namespace Menu {
    static std::vector<MenuGroup*> menu;

    static struct MenuInfo {
        std::string hash;
    } menuInfo;

    MenuItemBase* createMenuItem(const json& jsMenuItem, json* menuArgData) {
        MenuItemBase* menuItem;

        WidgetMenuBaseType menuItemType = jsMenuItem["type"].template get<WidgetMenuBaseType>();
        switch (menuItemType) {
            case WidgetMenuBaseType::Switch:
                menuItem = new MenuItemSwitch;
                break;
            case WidgetMenuBaseType::InputInt:
                menuItem = new MenuItemInputInt;
                break;
            case WidgetMenuBaseType::SliderFloat:
                menuItem = new MenuItemSliderFloat;
                break;
        }

        menuItem->argData = menuArgData;
        menuItem->label = jsMenuItem["label"].template get<std::string>();
        if (jsMenuItem.contains("action")) {
            menuItem->action = jsMenuItem["action"].template get<std::string>();
        }
        if (jsMenuItem.contains("arg")) {
            menuItem->argName = jsMenuItem["arg"].template get<std::string>();
        } else {
            menuItem->argName = "this";
        }

        switch (menuItemType) {
            case WidgetMenuBaseType::Switch:
            {
                auto sm = (MenuItemSwitch *) menuItem;
                sm->valueDefault = true;
                sm->value = sm->valueDefault;
                sm->type = WidgetMenuBaseType::Switch;
                (*menuArgData)[sm->argName] = sm->value;
                break;
            }
            case WidgetMenuBaseType::InputInt:
            {
                auto sm = (MenuItemInputInt *) menuItem;
                sm->valueMin = jsMenuItem["valueMin"].template get<int>();
                sm->valueMax = jsMenuItem["valueMax"].template get<int>();
                sm->valueStep = jsMenuItem["valueStep"].template get<int>();
                sm->valueStepFast = jsMenuItem["valueStepFast"].template get<int>();
                sm->valueDefault = jsMenuItem["valueDefault"].template get<int>();
                sm->valueWidth = jsMenuItem["valueWidth"].template get<float>();
                sm->value = sm->valueDefault;
                sm->type = WidgetMenuBaseType::InputInt;
                (*menuArgData)[sm->argName] = sm->value;
                break;
            }
            case WidgetMenuBaseType::SliderFloat:
            {
                auto sm = (MenuItemSliderFloat *) menuItem;
                sm->valueMin = jsMenuItem["valueMin"].template get<float>();
                sm->valueMax = jsMenuItem["valueMax"].template get<float>();
                sm->valueStep = jsMenuItem["valueStep"].template get<float>();
                sm->valueStepFast = jsMenuItem["valueStepFast"].template get<float>();
                sm->valueWidth = jsMenuItem["valueWidth"].template get<float>();
                sm->valueDefault = jsMenuItem["valueDefault"].template get<float>();
                sm->value = sm->valueDefault;
                sm->type = WidgetMenuBaseType::SliderFloat;
                (*menuArgData)[sm->argName] = sm->value;
                break;
            }
        }

        return menuItem;
    }

    void init(const json& data) {
        g_MenuInit = true;

        if (data.contains("versionHash")) {
            menuInfo.hash = data["versionHash"].template get<std::string>();
        }

        auto jsMenu = data["menu"];
        for (auto& jsMenuGroupItem: jsMenu) {
            auto menuGroupItemLabel = jsMenuGroupItem["label"].template get<std::string>();
            auto menuGroupItemAction = jsMenuGroupItem["action"].template get<std::string>();
            auto menuGroupItemItems = jsMenuGroupItem["items"];
            auto menuGroup = new MenuGroup;
            menuGroup->label = menuGroupItemLabel;
            menuGroup->action = menuGroupItemAction;

            for (auto& jsMenuItem: menuGroupItemItems) {
                json* menuArgData = new json;

                auto menuItem = createMenuItem(jsMenuItem, menuArgData);
                menuItem->fullAction = menuGroup->action + "." + menuItem->action;
                menuItem->isNewLine = false;
                menuItem->idImguiVisible = "##" + menuItem->fullAction + menuItem->argName;
                menuGroup->items.push_back(menuItem);

                if (jsMenuItem.contains("items")) {
                    for (auto& jsMenuArg: jsMenuItem["items"]) {
                        auto menuArg = createMenuItem(jsMenuArg, menuArgData);
                        menuArg->isNewLine = true;
                        menuArg->fullAction = menuItem->fullAction;
                        menuArg->idImguiVisible = "##" + menuArg->fullAction + menuArg->argName;
                        menuItem->args.push_back(menuArg);
                    }
                }

                if (menuItem->type == WidgetMenuBaseType::Switch) {
                    auto sm = (MenuItemSwitch *)menuItem;
                    if (sm->value) {
                        LibIj::runAction(sm->fullAction, *sm->argData);
                    }
                }
            }

            menu.push_back(menuGroup);
        }
    }
}
#endif //PIGMOD_MENU_H
