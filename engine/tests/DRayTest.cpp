#include <iostream>
#include "../code/Devendra_Math.h"
#include "../code/DRay.h"
#include "../code/DBitmap.h"
#include <vector>

int main() {
    // Image
    const real32 AspectRatio = 16.0f / 9.0f;
    uint32 ImageWidth = 1280;
    uint32 ImageHeight = (int)(ImageWidth / AspectRatio);

    // World
    world World = {};
    World.Spheres = std::vector<sphere* >();
    WorldAddSpherers(&World, CreateSphere(vec(0.0f, 0.0f, -1.0f), 0.5f));
    WorldAddSpherers(&World, CreateSphere(vec(0.0f,-100.5f, 1.0f), 100.0f));

    // Camera
    real32 ViewportHeight = 2.0f;
    real32 ViewportWidth = AspectRatio * ViewportHeight;
    real32 FocalLength = 1.0f;

    vec3 Origin = vec(0.0f, 0.0f, 0.0f);
    vec3 Horizontal = vec(ViewportWidth, 0.0f, 0.0f);
    vec3 vertical = vec(0.0f, ViewportHeight, 0.0f);
    vec3 LowerLeftCorner = Origin - Horizontal/2 - vertical/2 - vec(0, 0, FocalLength);

    // Bitmap Image
    image Image = CreateImage(ImageWidth, ImageHeight);
    uint32* Out = Image.Pixels;

    for (uint32 j = 0; j < ImageHeight; ++j) {
        for (uint32 i = 0; i < ImageWidth; ++i) {
            real32 u = (real32) i / (real32)(ImageWidth-1);
            real32 v = (real32) j / (real32)(ImageHeight-1);
            ray r = {};
            r.origin = Origin;
            r.direction = LowerLeftCorner + u*Horizontal + v*vertical - Origin;
            vec3 Color = RayColor(&r, &World);
            *Out++ = GetOutputColor(Color);
        }
        printf("\rRaycasting %d%% ...", 100 * j/Image.Height);
        fflush(stdout);
    }
    WriteImage(Image, "test.bmp"); 
    printf("\nDone.\n");
    return 0;
}