.class public Lcom/wawahuy/pigmod/GLViewWrapper;
.super Landroid/opengl/GLSurfaceView;
.source "GLViewWrapper.java"

# interfaces
.implements Landroid/opengl/GLSurfaceView$Renderer;


# direct methods
.method public constructor <init>(Landroid/content/Context;)V
    .locals 7

    .line 13
    invoke-direct {p0, p1}, Landroid/opengl/GLSurfaceView;-><init>(Landroid/content/Context;)V

    const/4 p1, 0x3

    .line 14
    invoke-virtual {p0, p1}, Lcom/wawahuy/pigmod/GLViewWrapper;->setEGLContextClientVersion(I)V

    const/16 v1, 0x8

    const/16 v2, 0x8

    const/16 v3, 0x8

    const/16 v4, 0x8

    const/16 v5, 0x10

    const/4 v6, 0x0

    move-object v0, p0

    .line 15
    invoke-virtual/range {v0 .. v6}, Lcom/wawahuy/pigmod/GLViewWrapper;->setEGLConfigChooser(IIIIII)V

    .line 16
    invoke-virtual {p0}, Lcom/wawahuy/pigmod/GLViewWrapper;->getHolder()Landroid/view/SurfaceHolder;

    move-result-object p1

    const/4 v0, -0x2

    invoke-interface {p1, v0}, Landroid/view/SurfaceHolder;->setFormat(I)V

    const/4 p1, 0x1

    .line 17
    invoke-virtual {p0, p1}, Lcom/wawahuy/pigmod/GLViewWrapper;->setZOrderOnTop(Z)V

    .line 18
    invoke-virtual {p0, p0}, Lcom/wawahuy/pigmod/GLViewWrapper;->setRenderer(Landroid/opengl/GLSurfaceView$Renderer;)V

    return-void
.end method


# virtual methods
.method public onDrawFrame(Ljavax/microedition/khronos/opengles/GL10;)V
    .locals 0

    .line 33
    invoke-static {}, Lcom/wawahuy/pigmod/NativeMethods;->onDrawFrame()V

    return-void
.end method

.method public onSurfaceChanged(Ljavax/microedition/khronos/opengles/GL10;II)V
    .locals 0

    .line 28
    invoke-static {p2, p3}, Lcom/wawahuy/pigmod/NativeMethods;->onSurfaceChanged(II)V

    return-void
.end method

.method public onSurfaceCreated(Ljavax/microedition/khronos/opengles/GL10;Ljavax/microedition/khronos/egl/EGLConfig;)V
    .locals 0

    .line 23
    invoke-static {}, Lcom/wawahuy/pigmod/NativeMethods;->onSurfaceCreated()V

    return-void
.end method
