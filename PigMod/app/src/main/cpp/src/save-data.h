//
// Created by nguye on 11/17/2023.
//

#ifndef PIGMOD_SAVE_DATA_H
#define PIGMOD_SAVE_DATA_H

namespace SaveData {
    static std::string pathSaveData;
    static json saveData;

    void init() {
        pathSaveData = "/data/data/" + getPackageName() +  "/lpm";
        std::ifstream t(pathSaveData);
        if (t.is_open()) {
            std::string str((std::istreambuf_iterator<char>(t)),
                            std::istreambuf_iterator<char>());
            try
            {
                saveData = json::parse(str);
                LOG_E("Save Data Parse OKE");
            } catch (json::parse_error& ex) {
                LOG_E("Err: %i", ex.byte);
            }
            t.close();
        }
    }

    bool contains(const char* name) {
        return saveData.contains(name);
    }

    std::string getString(const char* name) {
        if (!saveData.contains(name) || !saveData[name].is_string()) {
            return "";
        }
        return saveData[name].template get<std::string>();
    }

    bool getBool(const char* name) {
        if (!saveData.contains(name) || !saveData[name].is_boolean()) {
            return false;
        }
        return saveData[name].template get<bool>();
    }

    void _save() {
        std::string data = saveData.dump();
        LOG_E("saveAll %s", data.c_str());
        std::ofstream tempFile(pathSaveData, std::ios::binary);
        tempFile.write(&data[0], data.size());
        tempFile.close();
    }

    void saveString(const char* name, const std::string& data) {
        LOG_E("saveString %s %s", name, data.c_str());
        saveData[name] = data;
        _save();
    }

    void saveString(const char* name, const char* data) {
        LOG_E("saveString %s %s", name, data);
        saveData[name] = std::string(data);
        _save();
    }

    void saveBool(const char* name, bool data) {
        LOG_E("saveBool %s %i", name, data);
        saveData[name] = data;
        _save();
    }
}

#endif //PIGMOD_SAVE_DATA_H
