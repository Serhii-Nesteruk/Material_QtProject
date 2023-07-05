#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H


#include <atomic>
#include <mutex>
#include <shared_mutex>

#include <QMetaType>

class Material
{
public:
    Material() = default;
    Material(const Material &other);
    Material& operator=(const Material &other);
    Material &operator=(Material &&other);
    Material(Material &&other);
    ~Material() = default;

    float getQuality() const;
    bool setQuality(float newQuality);
    float getBasePrice() const;

    static bool requireValidQuality(float Value, int min, int max);

    void setBasePrice(float newBasePrice);
    void setName(const std::string &newName);
    float getFullPrice() const;
    std::string getName() const;

    bool getValidQuality();
private:
    bool invalidQuality = false;
    std::atomic<float> quality = 0.f;
    std::atomic<float> basePrice = 0.f;
    std::string name;
    mutable std::shared_mutex mutex;
};

Q_DECLARE_METATYPE(Material);

#endif // MATERIAL_H
