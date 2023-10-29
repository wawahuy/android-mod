//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_DEBUG_H
#define PIGMOD_DEBUG_H

namespace Debug {
    enum CommandType {
        VIl2CppWatch = 1,
        VIl2CppMapList = 2,

        EIl2CppWrite = 100,

        Reset = 1000
    };

    struct CommandData {
        CommandType command;
    };

    struct CommandDataVIl2CppWatch: CommandData {
        uintptr_t offset;
        uint32_t size;
    };

    struct CommandDataEIl2CppWrite: CommandData {
        uintptr_t offset;
        std::vector<unsigned char> bytes;
        std::vector<unsigned char> backupBytes;
    };

    std::vector<CommandData *> commandDataList;

    void CommandReset() {
        for (auto it = commandDataList.rbegin(); it != commandDataList.rend(); ++it) {
            auto command = *it;
            switch (command->command) {
                case CommandType::EIl2CppWrite:
                {
                    CommandDataEIl2CppWrite* c = (CommandDataEIl2CppWrite*) command;
                    Game::il2cppWriteMemory(c->offset, &c->backupBytes[0], c->bytes.size());
                    LOG_E("Restore EIl2CppWrite - offset: %p - size: %i", c->offset,  c->bytes.size());
                    break;
                }
            }
        }
        commandDataList.clear();
    }

    void onMessageHook(const json& js) {
        CommandType cmd = js["cmd"].template get<CommandType>();
        CommandData *commandData;
        LOG_E("Echo: %i", cmd);
        switch (cmd) {
            case CommandType::VIl2CppWatch:
            {
                CommandDataVIl2CppWatch* c = new CommandDataVIl2CppWatch();
                c->offset = hexStrToPtr(js["offset"].template get<std::string>());
                c->size = js["size"].template get<uint32_t>();
                commandData = (CommandData*) c;
                LOG_E("Add VIl2CppWatch - offset: %p - size: %i", c->offset,  c->size);
                break;
            }
            case CommandType::VIl2CppMapList:
                break;
            case CommandType::EIl2CppWrite:
            {
                CommandDataEIl2CppWrite* c = new CommandDataEIl2CppWrite();
                c->offset = hexStrToPtr(js["offset"].template get<std::string>());
                for (const auto& element : js["bytes"]) {
                    if (element.is_number()) {
                        c->bytes.push_back(element.get<unsigned char>());
                    } else {
                        LOG_E("Add EIl2CppWrite - push failed");
                    }
                }

                commandData = (CommandData*) c;
                // backup
                c->backupBytes = std::vector<unsigned char>(c->bytes.size());
                Game::il2cppCopyMemory2Vector(c->backupBytes, c->offset, c->bytes.size());

                // write
                Game::il2cppWriteMemory(c->offset, &c->bytes[0], c->bytes.size());
                LOG_E("Add EIl2CppWrite - offset: %p - size: %i", c->offset,  c->bytes.size());
                break;
            }
            case CommandType::Reset:
            {
                CommandReset();
                return;
            }
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
