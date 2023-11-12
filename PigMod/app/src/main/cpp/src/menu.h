//
// Created by nguye on 11/12/2023.
//

#ifndef PIGMOD_MENU_H
#define PIGMOD_MENU_H

enum WidgetMenuBaseType {
    Switch = 1,
    InputInt = 2,
    SliderFloat = 3,
    ServerSwitch = 101,
    Call = 102,
};

template<typename T>
struct MenuMaxMinBaseTemplate {
    T valueMax;
    T valueMin;
    T valueStep;
    T valueStepFast;
    float valueWidth;
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
struct MenuItemCall: MenuItemBase {
    int delay;
    int interval;
    uint64_t saveTime;
};

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

    void loopTypeCall() {
        uint64_t ct;
        bool isCall;
        for (auto menuGroupItem: menu) {
            for (auto menuItem: menuGroupItem->items) {
                if (menuItem->type == WidgetMenuBaseType::Call) {
                    isCall = false;
                    auto sm = (MenuItemCall*) menuItem;
                    if (sm->delay > -1) {
                        ct = getMs();
                        if (ct - sm->saveTime > sm->delay) {
                            sm->delay = -1;
                            isCall = true;
                        }
                    } else if (sm->interval > -1) {
                        ct = getMs();
                        if (ct - sm->saveTime > sm->interval) {
                            isCall = true;
                        }
                    }
                    if (isCall) {
                        LibIj::runAction(menuItem->fullAction, *menuItem->argData);
                        sm->saveTime = ct;
                    }
                }
            }
        }
    }

    void runActionServer(MenuItemBase* menuItem) {
        json js;
        js["action"] = menuItem->fullAction;
        js["data"] = *menuItem->argData;
        g_Socket->send(STR_COMMAND_S_MENU_ACTION, js);
    }

    void runAction(MenuItemBase* menuItem) {
        switch (menuItem->type) {
            case WidgetMenuBaseType::ServerSwitch:
                runActionServer(menuItem);
                break;
            default:
                LibIj::runAction(menuItem->fullAction, *menuItem->argData);
                break;
        }
    }

    MenuItemBase* createMenuItem(const json& jsMenuItem, json* menuArgData) {
        MenuItemBase* menuItem = nullptr;

        WidgetMenuBaseType menuItemType = jsMenuItem["type"].template get<WidgetMenuBaseType>();
        switch (menuItemType) {
            case WidgetMenuBaseType::Switch:
            case WidgetMenuBaseType::ServerSwitch:
                menuItem = new MenuItemSwitch;
                break;
            case WidgetMenuBaseType::InputInt:
                menuItem = new MenuItemInputInt;
                break;
            case WidgetMenuBaseType::SliderFloat:
                menuItem = new MenuItemSliderFloat;
                break;
            case WidgetMenuBaseType::Call:
                menuItem = new MenuItemCall;
                break;
        }

        if (menuItem == nullptr) {
            LOG_E("Menu failed --------------- %i", menuItemType);
            return nullptr;
        }

        menuItem->argData = menuArgData;
        menuItem->type = menuItemType;
        if (jsMenuItem.contains("label")) {
            menuItem->label = jsMenuItem["label"].template get<std::string>();
        }
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
            case WidgetMenuBaseType::ServerSwitch:
            {
                auto sm = (MenuItemSwitch *) menuItem;
                if (jsMenuItem.contains("valueDefault")) {
                    sm->valueDefault = jsMenuItem["valueDefault"].template get<bool>();
                } else {
                    sm->valueDefault = false;
                }
                sm->value = sm->valueDefault;
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
                (*menuArgData)[sm->argName] = sm->value;
                break;
            }
            case WidgetMenuBaseType::Call:
            {
                auto sm = (MenuItemCall *) menuItem;
                if (jsMenuItem.contains("delay")) {
                    sm->delay = jsMenuItem["delay"].template get<int>();
                } else {
                    sm->delay = -1;
                }
                if (jsMenuItem.contains("interval")) {
                    sm->interval = jsMenuItem["interval"].template get<int>();
                } else {
                    sm->interval = -1;
                }
                sm->saveTime = 0;
                break;
            }
        }

        return menuItem;
    }

    bool canInit(const json& js) {
        bool isInit = !g_MenuInit;
        if (isInit || js.contains("versionHash")) {
            auto newHash = js["versionHash"].template get<std::string>();
            if (newHash != menuInfo.hash) {
                isInit = true;
            }
        }
        return isInit;
    }

    void release() {
        LOG_E("Release menu");
        for (auto menuGroupItem: menu) {
            for (auto menuItem: menuGroupItem->items) {
                for (auto menuArg: menuItem->args) {
                    delete menuArg;
                }
                if (menuItem->type == WidgetMenuBaseType::Switch) {
                    auto sm = (MenuItemSwitch* )menuItem;
                    if (sm->value) {
                        LOG_E("Disable %s", sm->fullAction.c_str());
                        (*sm->argData)[sm->argName] = false;
                        runAction(sm);
                    }
                }
                delete menuItem->argData;
                delete menuItem;
            }
            delete menuGroupItem;
        }
        menu.clear();
    }

    void init(const json& data) {
        g_MenuInit = true;

        release();

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
                if (menuItem == nullptr) {
                    continue;
                }
                menuItem->fullAction = menuGroup->action + "." + menuItem->action;
                menuItem->isNewLine = false;
                menuItem->idImguiVisible = "##" + menuItem->fullAction + menuItem->argName;
                menuGroup->items.push_back(menuItem);

                if (jsMenuItem.contains("items")) {
                    for (auto& jsMenuArg: jsMenuItem["items"]) {
                        auto menuArg = createMenuItem(jsMenuArg, menuArgData);
                        if (menuArg == nullptr) {
                            continue;
                        }
                        menuArg->isNewLine = true;
                        menuArg->fullAction = menuItem->fullAction;
                        menuArg->idImguiVisible = "##" + menuArg->fullAction + menuArg->argName;
                        menuItem->args.push_back(menuArg);
                    }
                }

                if (menuItem->type == WidgetMenuBaseType::Switch) {
                    auto sm = (MenuItemSwitch *)menuItem;
                    if (sm->value) {
                        runAction(sm);
                    }
                }
            }

            menu.push_back(menuGroup);
        }
    }
}
#endif //PIGMOD_MENU_H
