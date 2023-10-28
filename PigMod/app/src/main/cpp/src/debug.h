//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_DEBUG_H
#define PIGMOD_DEBUG_H

namespace Debug {
    enum CommandType {
        Il2CppWatch = 1
    };

    struct CommandData {
        CommandType command;
    };

    struct CommandDataIl2CppWatch: CommandData {
        uintptr_t offset;
        uint32_t size;
    };

    std::vector<CommandData *> commandDataList;

    void onMessageHook(const json& js) {
        CommandType cmd = js["cmd"].template get<CommandType>();
        CommandData *commandData;
        LOG_E("Echo: %i", cmd);
        switch (cmd) {
            case CommandType::Il2CppWatch:
                CommandDataIl2CppWatch* c = new CommandDataIl2CppWatch();
                c->offset = hexStrToPtr(js["offset"].template get<std::string>());
                c->size = js["size"].template get<uint32_t>();
                commandData = (CommandData*) c;
                LOG_E("Add Il2CppWatch - offset: %p - size: %i", c->offset,  c->size);
                break;
        }
        if (commandData != nullptr) {
            commandData->command = cmd;
            commandDataList.push_back(commandData);
        }
    }

    void init() {
        DebugSocket::onMessageHook = onMessageHook;
    }
}

#endif //PIGMOD_DEBUG_H
