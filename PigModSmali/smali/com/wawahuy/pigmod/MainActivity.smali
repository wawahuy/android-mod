.class public Lcom/wawahuy/pigmod/MainActivity;
.super Landroid/app/Activity;
.source "MainActivity.java"


# direct methods
.method static constructor <clinit>()V
    .locals 1

    const-string v0, "pigmod"

    .line 17
    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    return-void
.end method

.method public constructor <init>()V
    .locals 0

    .line 15
    invoke-direct {p0}, Landroid/app/Activity;-><init>()V

    return-void
.end method

.method public static hookEvent(Landroid/view/InputEvent;)Z
    .locals 2

    .line 44
    instance-of v0, p0, Landroid/view/MotionEvent;

    if-eqz v0, :cond_0

    .line 45
    check-cast p0, Landroid/view/MotionEvent;

    invoke-virtual {p0}, Landroid/view/MotionEvent;->getX()F

    move-result v0

    invoke-virtual {p0}, Landroid/view/MotionEvent;->getY()F

    move-result v1

    invoke-virtual {p0}, Landroid/view/MotionEvent;->getAction()I

    move-result p0

    invoke-static {v0, v1, p0}, Lcom/wawahuy/pigmod/NativeMethods;->hookEvent(FFI)Z

    move-result p0

    return p0

    :cond_0
    const/4 p0, 0x1

    return p0
.end method

.method static synthetic lambda$startMain$0(Landroid/content/Context;)V
    .locals 0

    .line 28
    invoke-static {p0}, Lcom/wawahuy/pigmod/MainActivity;->startMenu(Landroid/content/Context;)V

    return-void
.end method

.method public static startMain(Landroid/content/Context;)V
    .locals 4

    .line 27
    new-instance v0, Landroid/os/Handler;

    invoke-static {}, Landroid/os/Looper;->getMainLooper()Landroid/os/Looper;

    move-result-object v1

    invoke-direct {v0, v1}, Landroid/os/Handler;-><init>(Landroid/os/Looper;)V

    .line 28
    new-instance v1, Lcom/wawahuy/pigmod/MainActivity$$ExternalSyntheticLambda0;

    invoke-direct {v1, p0}, Lcom/wawahuy/pigmod/MainActivity$$ExternalSyntheticLambda0;-><init>(Landroid/content/Context;)V

    const-wide/16 v2, 0x1f4

    invoke-virtual {v0, v1, v2, v3}, Landroid/os/Handler;->postDelayed(Ljava/lang/Runnable;J)Z

    return-void
.end method

.method public static startMenu(Landroid/content/Context;)V
    .locals 10

    .line 32
    move-object v0, p0

    check-cast v0, Landroid/app/Activity;

    invoke-virtual {v0}, Landroid/app/Activity;->getWindowManager()Landroid/view/WindowManager;

    move-result-object v0

    .line 33
    new-instance v9, Landroid/view/WindowManager$LayoutParams;

    const/4 v2, -0x1

    const/4 v3, -0x1

    const/4 v4, 0x0

    const/4 v5, 0x0

    const/4 v6, 0x2

    const/16 v7, 0x18

    const/4 v8, -0x2

    move-object v1, v9

    invoke-direct/range {v1 .. v8}, Landroid/view/WindowManager$LayoutParams;-><init>(IIIIIII)V

    const/16 v1, 0x31

    .line 38
    iput v1, v9, Landroid/view/WindowManager$LayoutParams;->gravity:I

    .line 39
    new-instance v1, Lcom/wawahuy/pigmod/GLViewWrapper;

    invoke-direct {v1, p0}, Lcom/wawahuy/pigmod/GLViewWrapper;-><init>(Landroid/content/Context;)V

    .line 40
    invoke-interface {v0, v1, v9}, Landroid/view/WindowManager;->addView(Landroid/view/View;Landroid/view/ViewGroup$LayoutParams;)V

    return-void
.end method


# virtual methods
.method protected onCreate(Landroid/os/Bundle;)V
    .locals 0

    .line 22
    invoke-super {p0, p1}, Landroid/app/Activity;->onCreate(Landroid/os/Bundle;)V

    .line 23
    invoke-static {p0}, Lcom/wawahuy/pigmod/MainActivity;->startMain(Landroid/content/Context;)V

    return-void
.end method
