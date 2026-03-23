#ifndef UI_ENTITY_H
#define UI_ENTITY_H

#include <cstdint>

using EntityId = uint32_t;
static constexpr EntityId INVALID_ENTITY = 0;

class Entity
{
public:
    explicit Entity(EntityId id = INVALID_ENTITY) : id_(id) {}
    EntityId id() const { return id_; }
    bool valid() const { return id_ != INVALID_ENTITY; }
    bool operator==(const Entity& o) const { return id_ == o.id_; }
    bool operator!=(const Entity& o) const { return id_ != o.id_; }

private:
    EntityId id_;
};

#endif // UI_ENTITY_H
