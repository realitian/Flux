#pragma once
#ifndef SSAO_RENDERING_H
#define SSAO_RENDERING_H

#include "Texture.h"

#include <vector>

namespace Flux
{
    class Framebuffer;
    class Vector3f;

    class SsaoInfo
    {
    public:
        SsaoInfo() : noiseTexture(0) {}

        Framebuffer* getCurrentBuffer() const;
        void switchBuffers();
        void createBuffers(unsigned int width, unsigned int height);
        void generate();

        std::vector<Vector3f> kernel;
        std::vector<Vector3f> noise;
        Texture* noiseTexture;
    private:
        int bufferIndex = 0;

        std::vector<Framebuffer*> buffers;
    };
}

#endif /* SSAO_RENDERING_H */
