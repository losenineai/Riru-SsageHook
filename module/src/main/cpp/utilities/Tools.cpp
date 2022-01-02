//
// Created by 泓清 on 2021/12/30.
//

#include "Tools.h"

int isGame(JNIEnv *env, jstring appDataDir) {
    if (!appDataDir)
        return 0;
    const char *app_data_dir = env->GetStringUTFChars(appDataDir, nullptr);
    int user = 0;
    static char package_name[256];
    if (sscanf(app_data_dir, "/data/%*[^/]/%d/%s", &user, package_name) != 2) {
        if (sscanf(app_data_dir, "/data/%*[^/]/%s", package_name) != 1) {
            package_name[0] = '\0';
            LOGW("无法解析 %s", app_data_dir);
            return 0;
        }
    }
    if (strcmp(package_name, GamePackageName) == 0) {
        LOGD("检测到游戏: %s", package_name);
        game_data_dir = new char[strlen(app_data_dir) + 1];
        strcpy(game_data_dir, app_data_dir);
        env->ReleaseStringUTFChars(appDataDir, app_data_dir);
        return 1;
    } else {
        env->ReleaseStringUTFChars(appDataDir, app_data_dir);
        return 0;
    }
}

uint64_t getModuleBase(const char *module_name) {
    uint64_t addr = 0;
    char line[1024];
    uint64_t start = 0;
    uint64_t end = 0;
    char flags[5];
    char path[PATH_MAX];

    FILE *fp = fopen("/proc/self/maps", "r");
    if (fp != nullptr) {
        while (fgets(line, sizeof(line), fp)) {
            strcpy(path, "");
            sscanf(line, "%" PRIx64"-%" PRIx64" %s %*" PRIx64" %*x:%*x %*u %s\n", &start, &end,
                    flags, path);
#if defined(__aarch64__)
            if (strstr(flags, "x") == 0) //TODO
                continue;
#endif
            if (strstr(path, module_name)) {
                addr = start;
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}

uint64_t getBssModuleBase(const char *module_name) {
    uint64_t addr = 0;
    char line[1024];
    uint64_t start = 0;
    uint64_t end = 0;
    int cnt = 0;
    char flags[5];
    char path[PATH_MAX];

    FILE *fp = fopen("/proc/self/maps", "r");

    if (fp != nullptr) {
        while (fgets(line, sizeof(line), fp)) {
            if (cnt == 1){
                if (strstr(line,"[anon:.bss]")){
                    strcpy(path, "");
                    sscanf(line, "%" PRIx64"-%" PRIx64" %s %*" PRIx64" %*x:%*x %*u %s\n", &start, &end,
                           flags, path);
#if defined(__aarch64__)
            if (strstr(flags, "x") == 0) //TODO
                continue;
#endif
                    if (strstr(path, "[anon:.bss]")) {
                        addr = start;
                        break;
                    }
                } else {
                    cnt = 0;
                }
            }
            if (strstr(line,module_name)){
                cnt = 1;
            }
        }
        fclose(fp);
    }
    return addr;
}

bool isLibraryLoaded(const char *libraryName) {
    char line[512] = {0};
    FILE *fp = fopen("/proc/self/maps", "rt");
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, libraryName))
                return true;
        }
        fclose(fp);
    }
    return false;
}
