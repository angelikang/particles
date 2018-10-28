#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData3D.h>

#include <vector>
#include <cstdlib>

#include "Particle.h"
#include "ParticleMesh.h"

using std::vector;

using namespace Magnum;
using namespace Magnum::Math::Literals;

class Application : public Platform::Application {
private:

    ParticleMesh _particleMesh;
    vector<Particle> _particles;

    Matrix4 _projection;

    Color3 _color;

    Vector2i _previousMousePosition;
    Uint32 _previousTime;

public:
    explicit Application(const Arguments &arguments):
        Platform::Application{arguments, Configuration{}.setTitle("Particles")} {

            GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
            GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

            _color = Color3::fromHsv(35.0_degf, 1.0f, 1.0f);

            _projection =
                    Matrix4::perspectiveProjection(
                            35.0_degf,
                            Vector2{GL::defaultFramebuffer.viewport().size()}.aspectRatio(),
                            0.1f,
                            1000.0f) *
                    Matrix4::rotationZ(180.0_degf) *
                    Matrix4::rotationX(90.0_degf) *
                    Matrix4::translation(Vector3::yAxis(-100.0f));
    }


private:

    void tickEvent() override {
        auto now = SDL_GetTicks();
        auto secondsElapsed = (now - _previousTime) / 1000.0f;
        _previousTime = now;

        if(_particles.size() < 1000){
            for(int i = 0; i < 10; i++) {
                _particles.emplace_back(Particle());
            }
        }

        for(uint i = 0; i < _particles.size(); i++) {
            _particles[i].update(secondsElapsed);
            if (_particles[i].isDead()) {
                _particles[i] = Particle();
            }
        }
    }

    void drawEvent() override {

        GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

        for(auto & particle : _particles) {
             _particleMesh.draw(_projection, particle.translation(), _color);
        }
        swapBuffers();

        redraw();
    }

    void mousePressEvent(MouseEvent &event) override  {
        if (event.button() != MouseEvent::Button::Left) return;

        _previousMousePosition = event.position();
        event.setAccepted();
    }

    void mouseReleaseEvent(MouseEvent &event) override  {
        _color = Color3::fromHsv(_color.hue() + 50.0_degf, 1.0f, 1.0f);

        event.setAccepted();
        redraw();
    }


    void mouseMoveEvent(MouseMoveEvent &event) override {
        if (!(event.buttons() & MouseMoveEvent::Button::Left)) return;

        const Vector2 delta = 3.0f *
                              Vector2{event.position() - _previousMousePosition} /
                              Vector2{GL::defaultFramebuffer.viewport().size()};

        _projection =
                _projection *
                Matrix4::rotationZ(Rad{delta.x()}) *
                Matrix4::rotationX(Rad{delta.y()});

        _previousMousePosition = event.position();

        event.setAccepted();
        redraw();
    }
};

MAGNUM_APPLICATION_MAIN(Application);


