#include "material.h"

#include <stdexcept>

Material &Material::operator=(Material &&other) {
  this->name = std::move(other.name);
  this->quality.store(other.quality);
  this->basePrice.store(other.basePrice);

  return *this;
}

Material &Material::operator=(const Material &other) {
  this->name = other.name;
  this->quality.store(other.quality);
  this->basePrice.store(other.basePrice);

  return *this;
}

Material::Material(Material &&other) {
  *this = std::move(other);
}

Material::Material(const Material &other) {
  *this = other;
}

float Material::getQuality() const {
    std::shared_lock sl(mutex);
    return quality;
}

bool Material::setQuality(float newQuality) {
    std::lock_guard lg(mutex);

  if (requireValidQuality(newQuality, 0, 10))
      this->quality = newQuality;
  else
    invalidQuality = true;
  return !invalidQuality;
}

float Material::getBasePrice() const {
    std::shared_lock sl(mutex);
    return basePrice;
}

bool Material::requireValidQuality(float Value, int min, int max) {
  return !(Value < min || Value > max);
}

void Material::setBasePrice(float newBasePrice) {
    std::lock_guard lg(mutex);
  this->basePrice = newBasePrice;
}

void Material::setName(const std::string &newName) {
  std::lock_guard lg(mutex);
  this->name = newName;
}

float Material::getFullPrice() const{
    std::shared_lock sl(mutex);
    return quality * basePrice;
}

std::string Material::getName() const {
    std::shared_lock sl(mutex);
  return name;
}

bool Material::getValidQuality() {
  return invalidQuality;
}
