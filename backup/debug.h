//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_DEBUG_H
#define PIGMOD_DEBUG_H

namespace Debug {
    const uint32_t PAD_A = 0x12345;
    const uint32_t PAD_B = 0x54321;

    enum CommandName {
        IL2CPP_SHOW_MEMORY = 1
    };

    enum CommandStage {
        None = 1,
        Waiting = 2,
    };

    struct Command {
        uint32_t padA;
        uint32_t name;
        uint32_t size;
        uint32_t padB;
    } __attribute__((packed));


    struct CmdIl2CppShowMemory {
        uint64_t offset;
        uint32_t size;
    };

    CommandStage commandStage = CommandStage::None;
    Command commandCurrent;
    void *commandData;
    uintptr_t commandDataOffset;

    void handleMessage(DebugSocket::Client* client, char* buffer, size_t bufferSize) {
        DebugSocket::sendGlobalMessage(buffer, bufferSize);
        LOG_E("Buff size: %i", bufferSize);
        bool isOver = false;

        if (commandStage == CommandStage::Waiting) {
            if (commandDataOffset < commandCurrent.size) {
                if (commandDataOffset + bufferSize > commandCurrent.size) {
                    isOver = true;
                }
                memcpy(
                    (void*)((uintptr_t )commandData + commandDataOffset),
                    buffer,
                    bufferSize
                );
                commandDataOffset += bufferSize;
                return;
            }
        }

        Command* cmd = (Command*)buffer;
        if (cmd->padA == PAD_A && cmd->padB == PAD_B) {
            memcpy(&commandCurrent, buffer, sizeof(Command));
            LOG_E("%i %i", cmd->name, cmd->size);

            auto size = cmd->size;
            auto name = cmd->name;
            if (commandStage == CommandStage::None) {
                commandStage = CommandStage::Waiting;
                if (size > 0) {
                    commandData = new uint8_t[size];
                    commandDataOffset = 0;
                }
            } else {
                commandStage = CommandStage::None;
                DebugSocket::sendGlobalMessage(commandData, commandDataOffset);
                if (commandData) {
                    delete commandData;
                }
            }
        }

    }

    void init() {
        DebugSocket::handleMessage = Debug::handleMessage;
    }
}

#endif //PIGMOD_DEBUG_H
