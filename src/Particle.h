#ifndef PARTICLE_H

using namespace Magnum;

const Vector3 GRAVITY = Vector3(0.0f,0.0f,-10.0f);

class Particle {
private:
    Matrix4 _translation;
    Vector3 _position;
    Vector3 _speed;

    float _lifeTime;
    float _lifeLength;

public:
    Particle():
            _speed(randomFloat(-3.0f, 3.0f), randomFloat(-3.0f, 3.0f), randomFloat(10.0f, 20.0f)),
            _lifeLength(randomFloat(3.0f, 5.0f)){

        _lifeTime = _lifeLength;
        update(0);
    }

    void update(double secondsElapsed) {
        _position = _position + (_speed + GRAVITY / 2.0f * secondsElapsed) * secondsElapsed;
        _speed = _speed + GRAVITY * secondsElapsed;

        auto size = (1.0f - std::pow((_lifeTime / _lifeLength - 0.5f ) * 2.0f, 2.0f)) * 0.4f;

        _translation =
                Matrix4::scaling(Vector3(size)) *
                Matrix4::translation(_position / size ) ;

        _lifeTime -= secondsElapsed;
    }

    Matrix4 & translation() {
        return _translation;
    }

    bool isDead(){
        return _lifeTime < 0.0f;
    };

private:
    float randomFloat(float min, float max)
    {
        float r = float(rand()) / RAND_MAX;
        return r * (max - min) + min;
    }
};

#define PARTICLE_H
#endif //PARTICLE_H
