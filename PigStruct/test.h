struct int32_t {};
struct int8_t {};
struct int16_t {};
struct int64_t {};
struct uint32_t {};
struct uint8_t {};
struct uint16_t {};
struct uint64_t {};
struct intptr_t {};
struct uintptr_t {};
struct size_t {};

typedef void(*Il2CppMethodPointer)();

struct MethodInfo;

struct Il2CppClass;

struct VirtualInvokeData
{
    Il2CppMethodPointer methodPtr;
    const MethodInfo* method;
};


struct Il2CppType
{
    void* data;
    unsigned int bits;
};

struct Il2CppRuntimeInterfaceOffsetPair
{
    Il2CppClass* interfaceType;
    int32_t offset;
};

struct Il2CppClass_1
{
    void* image;
    void* gc_desc;
    const char* name;
    const char* namespaze;
    Il2CppType byval_arg;
    Il2CppType this_arg;
    Il2CppClass* element_class;
    Il2CppClass* castClass;
    Il2CppClass* declaringType;
    Il2CppClass* parent;
    void *generic_class;
    void* typeMetadataHandle;
    void* interopData;
    Il2CppClass* klass;
    void* fields;
    void* events;
    void* properties;
    void* methods;
    Il2CppClass** nestedTypes;
    Il2CppClass** implementedInterfaces;
    Il2CppRuntimeInterfaceOffsetPair* interfaceOffsets;
};

struct Il2CppClass_2
{
    Il2CppClass** typeHierarchy;
    void *unity_user_data;
    uint32_t initializationExceptionGCHandle;
    uint32_t cctor_started;
    uint32_t cctor_finished;
    size_t cctor_thread;
    void* genericContainerHandle;
    uint32_t instance_size;
    uint32_t actualSize;
    uint32_t element_size;
    int32_t native_size;
    uint32_t static_fields_size;
    uint32_t thread_static_fields_size;
    int32_t thread_static_fields_offset;
    uint32_t flags;
    uint32_t token;
    uint16_t method_count;
    uint16_t property_count;
    uint16_t field_count;
    uint16_t event_count;
    uint16_t nested_type_count;
    uint16_t vtable_count;
    uint16_t interfaces_count;
    uint16_t interface_offsets_count;
    uint8_t typeHierarchyDepth;
    uint8_t genericRecursionDepth;
    uint8_t rank;
    uint8_t minimumAlignment;
    uint8_t naturalAligment;
    uint8_t packingSize;
    uint8_t bitflags1;
    uint8_t bitflags2;
};


struct UserData_VTable {
	VirtualInvokeData _0_Equals;
	VirtualInvokeData _1_Finalize;
	VirtualInvokeData _2_GetHashCode;
	VirtualInvokeData _3_ToString;
	VirtualInvokeData _4_get_PicFrameId;
	VirtualInvokeData _5_set_PicFrameId;
	VirtualInvokeData _6_get_FlagId;
	VirtualInvokeData _7_set_FlagId;
	VirtualInvokeData _8_SetPlayerData;
	VirtualInvokeData _9_SetPlayerData;
};

union Il2CppRGCTXData
{
    void* rgctxDataDummy;
    const MethodInfo* method;
    const Il2CppType* type;
    Il2CppClass* klass;
};

struct MethodInfo
{
    Il2CppMethodPointer methodPointer;
    void* invoker_method;
    const char* name;
    Il2CppClass *klass;
    const Il2CppType *return_type;
    const void* parameters;
    union
    {
        const Il2CppRGCTXData* rgctx_data;
        const void* methodMetadataHandle;
    };
    union
    {
        const void* genericMethod;
        const void* genericContainerHandle;
    };
    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint8_t parameters_count;
    uint8_t bitflags;
};


struct Il2CppClass
{
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[255];
};

struct PlayerBaseData_Fields {
        int32_t _Uid_k__BackingField;
        struct System_String_o* _Name_k__BackingField;
        int32_t _HeadDefaultId_k__BackingField;
        struct System_String_o* _HeadUrl_k__BackingField;
        bool _IsVip_k__BackingField;
        int32_t _PicFrameId_k__BackingField;
        int32_t _FlagId_k__BackingField;
        int32_t _StarNum_k__BackingField;
        int32_t _Sex_k__BackingField;
        int64_t _FamilyId_k__BackingField;
        int32_t _familyIcon;
        struct System_String_o* _FamilyName_k__BackingField;
        int32_t _FamilyPosition_k__BackingField;
        int32_t _PlatformType_k__BackingField;
        bool _isSVIP;
};

struct UserData_Fields : PlayerBaseData_Fields {
        int32_t MaxIsland;
        struct System_String_o* Mtkey;
        struct System_String_o* Skey;
        struct System_String_o* CryptKey;
        struct System_String_o* FeedTag;
        struct System_String_o* Platform;
        struct System_String_o* Device;
        struct System_String_o* Channel;
        struct System_Collections_Generic_List_string__o* SocketUrlList;
        struct WheelInfoData_o* WheelInfoData;
        struct IslandData_o* IslandData;
        struct BetData_o* BetData;
        int64_t LoginTimeStamp;
        struct System_String_o* HabbitPurchaseType;
        bool IsDiligent;
        bool IsBackUser;
        struct LitJson_JsonData_o* _LoginRspJson_k__BackingField;
        struct Forevernine_Com_Planet_Proto_GameLoginRsp_o* _LoginRspPb_k__BackingField;
        bool _HasVipReward_k__BackingField;
        bool _IsCodeExchangeEnable_k__BackingField;
        struct System_Collections_Generic_List_string__o* FeedbackReplyMsgIdList;
        int32_t _Guide_k__BackingField;
        int32_t _RegisterDay_k__BackingField;
        int32_t _RecommendCount_k__BackingField;
        int32_t _RefuseCount_k__BackingField;
        struct System_String_o* _country;
        struct System_String_o* _DeliveryUrl_k__BackingField;
        struct LitJson_JsonData_o* _WXConfig_k__BackingField;
        int64_t _serverTime;
        bool _HasBindReward_k__BackingField;
        struct System_Collections_Generic_Dictionary_string__UserPropertyData__o* UserPropertyDict;
        struct LitJson_JsonData_o* _ChatJson_k__BackingField;
        int32_t _newguestShowBind;
};

struct UserData_c {
	Il2CppClass_1 _1;
	void* static_fields;
	Il2CppRGCTXData* rgctx_data;
	Il2CppClass_2 _2;
	UserData_VTable vtable;
};

struct UserData_o {
        UserData_c *klass;
        void *monitor;
        UserData_Fields fields;
};