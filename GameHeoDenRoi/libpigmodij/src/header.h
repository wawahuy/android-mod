


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

struct Il2CppObject
{
    Il2CppClass *klass;
    void *monitor;
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

struct System_String_VTable {
	VirtualInvokeData _0_Equals;
	VirtualInvokeData _1_Finalize;
	VirtualInvokeData _2_GetHashCode;
	VirtualInvokeData _3_ToString;
	VirtualInvokeData _4_CompareTo;
	VirtualInvokeData _5_Clone;
	VirtualInvokeData _6_GetTypeCode;
	VirtualInvokeData _7_System_IConvertible_ToBoolean;
	VirtualInvokeData _8_System_IConvertible_ToChar;
	VirtualInvokeData _9_System_IConvertible_ToSByte;
	VirtualInvokeData _10_System_IConvertible_ToByte;
	VirtualInvokeData _11_System_IConvertible_ToInt16;
	VirtualInvokeData _12_System_IConvertible_ToUInt16;
	VirtualInvokeData _13_System_IConvertible_ToInt32;
	VirtualInvokeData _14_System_IConvertible_ToUInt32;
	VirtualInvokeData _15_System_IConvertible_ToInt64;
	VirtualInvokeData _16_System_IConvertible_ToUInt64;
	VirtualInvokeData _17_System_IConvertible_ToSingle;
	VirtualInvokeData _18_System_IConvertible_ToDouble;
	VirtualInvokeData _19_System_IConvertible_ToDecimal;
	VirtualInvokeData _20_System_IConvertible_ToDateTime;
	VirtualInvokeData _21_ToString;
	VirtualInvokeData _22_System_IConvertible_ToType;
	VirtualInvokeData _23_System_Collections_IEnumerable_GetEnumerator;
	VirtualInvokeData _24_CompareTo;
	VirtualInvokeData _25_System_Collections_Generic_IEnumerable_System_Char__GetEnumerator;
	VirtualInvokeData _26_Equals;
};

struct System_String_c {
	Il2CppClass_1 _1;
	struct System_String_StaticFields* static_fields;
	Il2CppRGCTXData* rgctx_data;
	Il2CppClass_2 _2;
	System_String_VTable vtable;
};

struct System_String_Fields {
	int32_t m_stringLength;
	uint16_t m_firstChar;
};

struct System_String_o {
	System_String_c *klass;
	void *monitor;
	System_String_Fields fields;
};


struct System_Collections_Generic_List_string__VTable {
	VirtualInvokeData _0_Equals;
	VirtualInvokeData _1_Finalize;
	VirtualInvokeData _2_GetHashCode;
	VirtualInvokeData _3_ToString;
	VirtualInvokeData _4_unknown;
	VirtualInvokeData _5_set_Item;
	VirtualInvokeData _6_IndexOf;
	VirtualInvokeData _7_Insert;
	VirtualInvokeData _8_unknown;
	VirtualInvokeData _9_unknown;
	VirtualInvokeData _10_System_Collections_Generic_ICollection_T__get_IsReadOnly;
	VirtualInvokeData _11_Add;
	VirtualInvokeData _12_unknown;
	VirtualInvokeData _13_Contains;
	VirtualInvokeData _14_CopyTo;
	VirtualInvokeData _15_Remove;
	VirtualInvokeData _16_System_Collections_Generic_IEnumerable_T__GetEnumerator;
	VirtualInvokeData _17_System_Collections_IEnumerable_GetEnumerator;
	VirtualInvokeData _18_System_Collections_IList_get_Item;
	VirtualInvokeData _19_System_Collections_IList_set_Item;
	VirtualInvokeData _20_System_Collections_IList_Add;
	VirtualInvokeData _21_System_Collections_IList_Contains;
	VirtualInvokeData _22_Clear;
	VirtualInvokeData _23_System_Collections_IList_get_IsReadOnly;
	VirtualInvokeData _24_System_Collections_IList_get_IsFixedSize;
	VirtualInvokeData _25_System_Collections_IList_IndexOf;
	VirtualInvokeData _26_System_Collections_IList_Insert;
	VirtualInvokeData _27_System_Collections_IList_Remove;
	VirtualInvokeData _28_RemoveAt;
	VirtualInvokeData _29_System_Collections_ICollection_CopyTo;
	VirtualInvokeData _30_unknown;
	VirtualInvokeData _31_System_Collections_ICollection_get_SyncRoot;
	VirtualInvokeData _32_System_Collections_ICollection_get_IsSynchronized;
	VirtualInvokeData _33_get_Item;
	VirtualInvokeData _34_get_Count;
};

struct System_Collections_Generic_List_string__c {
	Il2CppClass_1 _1;
	struct System_Collections_Generic_List_string__StaticFields* static_fields;
	Il2CppRGCTXData* rgctx_data;
	Il2CppClass_2 _2;
	System_Collections_Generic_List_string__VTable vtable;
};

struct System_Collections_Generic_List_string__Fields {
	struct System_String_array* _items;
	int32_t _size;
	int32_t _version;
	Il2CppObject* _syncRoot;
};

struct System_Collections_Generic_List_string__o {
	System_Collections_Generic_List_string__c *klass;
	void *monitor;
	System_Collections_Generic_List_string__Fields fields;
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