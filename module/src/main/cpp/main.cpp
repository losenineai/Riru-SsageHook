#include <jni.h>
#include <sys/types.h>
#include <riru.h>
#include <malloc.h>
#include <cstring>
#include <config.h>
#include <pthread.h>
#include "utilities/Tools.h"
#include "Hook.h"

static void forkAndSpecializePre(
        JNIEnv *env, jclass clazz, jint *uid, jint *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
        jintArray *fdsToClose, jintArray *fdsToIgnore, jboolean *is_child_zygote,
        jstring *instructionSet, jstring *appDataDir, jboolean *isTopApp, jobjectArray *pkgDataInfoList,
        jobjectArray *whitelistedDataInfoList, jboolean *bindMountAppDataDirs, jboolean *bindMountAppStorageDirs) {
    // 通过Zygote 在App启动前调用本区域内容
    // 参数都是指针 如果你想要 你可以改变他们这些参数的值
    // 一些参数在安卓老版本中不存在 这种情况下 他们为 null 或者 0
    enable_hack = isGame(env, *appDataDir);// 获取目标应用pid
}

static void forkAndSpecializePost(JNIEnv *env, jclass clazz, jint res) {
    // 通过Zygote 在App启动前调用本区域内容
    // "res" 参数是nativeForkAndSpecialize的返回值

    if (res == 0) {
        // 在 app 进程中

        if (enable_hack) {// 判断是否存在目标进程
            int ret;
            pthread_t ntid;
            if ((ret = pthread_create(&ntid, nullptr, hack_thread, nullptr))) {
                // 启动自定义线程 hack_thread
                LOGE("无法创建线程: %s\n", strerror(ret));
            }
        }

        // 当 unload allowed 为 true 时, 本模块将被Riru卸载(dlclose)
        // 如果这个模块已经有了hook，不要把它设置为true，否则会有SIGSEGV
        // 该值将在调用 pre 函数之前自动重置为false
        riru_set_unload_allowed(false);
    } else {
        // 在 zygote 进程中
    }
}

static void specializeAppProcessPre(
        JNIEnv *env, jclass clazz, jint *uid, jint *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
        jboolean *startChildZygote, jstring *instructionSet, jstring *appDataDir,
        jboolean *isTopApp, jobjectArray *pkgDataInfoList, jobjectArray *whitelistedDataInfoList,
        jboolean *bindMountAppDataDirs, jboolean *bindMountAppStorageDirs) {
    // 通过Zygote 在App启动前调用本区域内容
    // 参数都是指针 如果你想要 你可以改变他们这些参数的值
    // 一些参数在安卓老版本中不存在 这种情况下 他们为 null 或者 0
}

static void specializeAppProcessPost(
        JNIEnv *env, jclass clazz) {
    // 通过Zygote 在App启动后调用本区域内容

    // 当 unload allowed 为 true 时, 本模块将被Riru卸载(dlclose)
    // 如果这个模块已经有了hook，不要把它设置为true，否则会有SIGSEGV
    // 该值将在调用 pre 函数之前自动重置为false
    riru_set_unload_allowed(true);
}

static void forkSystemServerPre(
        JNIEnv *env, jclass clazz, uid_t *uid, gid_t *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jlong *permittedCapabilities, jlong *effectiveCapabilities) {
    // 通过Zygote 在App启动前调用本区域内容
    // 参数都是指针 如果你想要 你可以改变他们这些参数的值
    // 一些参数在安卓老版本中不存在 这种情况下 他们为 null 或者 0
}

static void forkSystemServerPost(JNIEnv *env, jclass clazz, jint res) {
    // 通过Zygote 在App启动后调用本区域内容

    if (res == 0) {
        // 在 system server (系统服务) 进程中
    } else {
        // 在 zygote 进程中
    }
}

static void onModuleLoaded() {
    // 当库文件被加载并且被riru隐藏(详情见 Riru's hide.cpp)的时候 调用本区域内容
    // 如果你想要使用线程 在这里开启他们要远优于在构造函数中启动线程
    // __attribute__((constructor)) or constructors of static variables,
    // ?__attribute__((constructor)) (构造函数的属性) or constructors (构造函数) 的静态变量
    // ?or the "hide" will cause SIGSEGV
}

extern "C" {

int riru_api_version;
const char *riru_magisk_module_path = nullptr;
int *riru_allow_unload = nullptr;

static auto module = RiruVersionedModuleInfo{
        .moduleApiVersion = riru::moduleApiVersion,
        .moduleInfo= RiruModuleInfo{
                .supportHide = true,
                .version = riru::moduleVersionCode,
                .versionName = riru::moduleVersionName,
                .onModuleLoaded = onModuleLoaded,
                .forkAndSpecializePre = forkAndSpecializePre,
                .forkAndSpecializePost = forkAndSpecializePost,
                .forkSystemServerPre = forkSystemServerPre,
                .forkSystemServerPost = forkSystemServerPost,
                .specializeAppProcessPre = specializeAppProcessPre,
                .specializeAppProcessPost = specializeAppProcessPost
        }
};

RiruVersionedModuleInfo *init(Riru *riru) {
    auto core_max_api_version = riru->riruApiVersion;
    riru_api_version = core_max_api_version <= riru::moduleApiVersion ? core_max_api_version : riru::moduleApiVersion;
    module.moduleApiVersion = riru_api_version;

    riru_magisk_module_path = strdup(riru->magiskModulePath);
    if (riru_api_version >= 25) {
        riru_allow_unload = riru->allowUnload;
    }
    return &module;
}
}
