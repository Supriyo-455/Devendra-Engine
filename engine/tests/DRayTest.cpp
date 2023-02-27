#include <iostream>
#include "../code/windows/Devendra_Math.h"
#include "../code/windows/DRay.h"
#include "../code/windows/DBitmap.h"
#include <vector>

int main()
{
    // Image
    const real32 AspectRatio = 16.0f / 9.0f;
    const uint32 ImageWidth = 980;
    const uint32 ImageHeight = (int)(ImageWidth / AspectRatio);
    const uint32 SamplesPerPixel = 5;

    // World
    world World;
    World.add(make_shared<sphere>(sphere(vec(0.0f, 0.0f, -1.0f), 0.5f)));
    World.add(make_shared<sphere>(sphere(vec(0.0f, -100.5f, 1.0f), 100.0f)));

    // Camera
    camera Camera(AspectRatio);

    // Bitmap Image
    image Image = CreateImage(ImageWidth, ImageHeight);
    uint32 *Out = Image.Pixels;

    for (uint32 j = 0; j < ImageHeight; ++j)
    {
        for (uint32 i = 0; i < ImageWidth; ++i)
        {
            vec3 PixelColor = {};
            for (uint32 SampleIndex = 0; SampleIndex < SamplesPerPixel; ++SampleIndex)
            {
                real32 u = (i + Random()) / (ImageWidth - 1);
                real32 v = (j + Random()) / (ImageHeight - 1);

                ray r = Camera.getRay(u, v);
                PixelColor = PixelColor + RayColor(r, World);
            }
            *Out++ = GetOutputColor(PixelColor, SamplesPerPixel);
        }
        printf("\rRaycasting %d%% ...", 100 * j / Image.Height);
        fflush(stdout);
    }
    WriteImage(Image, "test.bmp");
    printf("\nDone.\n");
    return 0;
}