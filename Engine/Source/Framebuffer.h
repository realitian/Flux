#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Texture.h"
#include "TextureLoader.h"
#include "Cubemap.h"
#include "Util/Log.h"

#include <glad/glad.h>
#include <vector>

namespace Flux {
    class Framebuffer {
    public:
        Framebuffer(const unsigned int width, const unsigned int height)
            : width(width)
            , height(height)
        {
            glGenFramebuffers(1, &handle);
        }

        void bind() const {
            glBindFramebuffer(GL_FRAMEBUFFER, handle);
        }

        void release() const {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        const Texture& getColorTexture(int attachment) const {
            return *colorTexture[attachment];
        }

        void addColorTexture(int colorAttachment, Texture* texture) {
            colorTexture[colorAttachment] = texture;
            GLint attachment = GL_COLOR_ATTACHMENT0 + colorAttachment;
            setTexture(attachment, *texture);
            addDrawBuffer(attachment);
        }

        void addDepthTexture() {
            depthTexture = TextureLoader::create(width, height, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, CLAMP);
            setTexture(GL_DEPTH_ATTACHMENT, *depthTexture);
        }

        void addDepthTexture(Texture* texture) {
            setTexture(GL_DEPTH_ATTACHMENT, *texture);
        }

        void setTexture(GLuint attachment, Texture& texture) {
            glFramebufferTexture(GL_FRAMEBUFFER, attachment, texture.handle, 0);
        }

        void setDepthCubemap(Cubemap* cubemap, unsigned int face, int mipmapLevel) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, cubemap->getHandle(), mipmapLevel);
        }

        void setCubemap(GLuint texture, unsigned int face, int mipmapLevel) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, texture, mipmapLevel);
        }

        void addDrawBuffer(GLenum target) {
            drawBuffers.push_back(target);
            glDrawBuffers((GLsizei) drawBuffers.size(), drawBuffers.data());
        }

        void enableColor(int target) {
            glReadBuffer(target);
            glDrawBuffer(target);
        }

        void disableColor() {
            glReadBuffer(GL_NONE);
            glDrawBuffer(GL_NONE);
        }

        void validate() const {
            GLuint error = glCheckFramebufferStatus(GL_FRAMEBUFFER);

            if (error != GL_FRAMEBUFFER_COMPLETE) {
                switch (error) {
                case GL_FRAMEBUFFER_UNDEFINED:
                    Log::error("Target is the default framebuffer, but the default framebuffer does not exist"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    Log::error("Any of the framebuffer attachment points are framebuffer incomplete"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    Log::error("The framebuffer does not have any texture attached to it"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                    Log::error("The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                    Log::error("GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER"); break;
                case GL_FRAMEBUFFER_UNSUPPORTED:
                    Log::error("The combination of internal formats of the attached textures violates an implementation-dependent set of restrictions."); break;
                case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                    Log::error("The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers"); break;
                default:
                    Log::error("There is a problem with the framebuffer");
                }
            }
        }
    private:
        unsigned int width;
        unsigned int height;
        GLuint handle;

        Texture* colorTexture[8];
        Texture* depthTexture;

        std::vector<GLenum> drawBuffers;
    };
}

#endif /* FRAMEBUFFER_H */
