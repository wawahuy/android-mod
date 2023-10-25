.class public Lcom/wawahuy/pigmod/NativeMethods;
.super Ljava/lang/Object;
.source "NativeMethods.java"


# direct methods
.method public constructor <init>()V
    .locals 0

    .line 3
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method

.method public static native hookEvent(FFI)Z
.end method

.method public static native onDrawFrame()V
.end method

.method public static native onSurfaceChanged(II)V
.end method

.method public static native onSurfaceCreated()V
.end method
