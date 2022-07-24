#ifndef TANKS_HITTABLE_H
#define TANKS_HITTABLE_H

namespace Tanks {
class Damageable {
  public:
    virtual ~Damageable() = default;
    virtual void damage(int damage) = 0;
};
} // namespace Tanks

#endif /* TANKS_HITTABLE_H */
