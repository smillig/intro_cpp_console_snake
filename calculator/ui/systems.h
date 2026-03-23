#ifndef UI_SYSTEMS_H
#define UI_SYSTEMS_H

#include "entity.h"
#include "components.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include "../core/calc_api.h" // adjust include path as needed
#include <SFML/Graphics.hpp>

// Very small EntityManager to manage IDs and component presence
class EntityManager
{
public:
    Entity create()
    {
        EntityId id = ++lastId_;
        return Entity(id);
    }

private:
    EntityId lastId_ = INVALID_ENTITY;
};

// Component storage (sparse, maps entity id -> component)
template<typename Comp>
class ComponentStorage
{
public:
    void add(Entity e, const Comp& c) { storage_[e.id()] = c; }
    void remove(Entity e) { storage_.erase(e.id()); }
    bool has(Entity e) const { return storage_.count(e.id()) != 0; }
    Comp* get(Entity e)
    {
        auto it = storage_.find(e.id());
        if (it == storage_.end()) return nullptr;
        return &it->second;
    }
    std::vector<Entity> entities() const
    {
        std::vector<Entity> out;
        out.reserve(storage_.size());
        for (auto const& kv : storage_) out.emplace_back(Entity(kv.first));
        return out;
    }

private:
    std::unordered_map<EntityId, Comp> storage_;
};

// Systems
class ParseSystem
{
public:
    ParseSystem(History& hist) : history_(hist) {}
    // Parses input text into AST and stores shared_ptr into history and plot config
    std::optional<std::string> parseAndAttach(const std::string& text, PlotConfigComponent& plot)
    {
        auto [astPtr, perr] = parseExpressionAPI(text);
        if (perr) return *perr;
        std::shared_ptr<ASTNode> sharedAst(std::move(astPtr));
        // create function wrapper
        plot.ast = sharedAst;
        plot.fun = [sharedAst](double x){ EvalContext ctx; ctx.x = x; return sharedAst->evaluate(ctx); };
        history_.push(sharedAst, text);
        return std::nullopt;
    }

private:
    History& history_;
};

class EvalSystem
{
public:
    // Samples the plot's function and fills the renderable vertices
    static void samplePlot(const PlotConfigComponent& plot, RenderableComponent& rend)
    {
        rend.clear();
        if (!plot.visible || !plot.fun) return;
        double x0 = plot.xMin;
        double x1 = plot.xMax;
        size_t n = std::max<size_t>(2, plot.samples);
        rend.vertices.reserve(n);
        for (size_t i = 0; i < n; ++i)
        {
            double t = static_cast<double>(i) / static_cast<double>(n - 1);
            double x = x0 + t * (x1 - x0);
            double y = plot.fun(x);
            rend.vertices.emplace_back(sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)),
                                                 plot.color));
        }
    }
};

class RenderSystem
{
public:
    RenderSystem(sf::RenderWindow& win) : window_(win) {}

    // draw a single renderable with transform applied
    void draw(const RenderableComponent& r, const TransformComponent& tr)
    {
        if (r.vertices.empty()) return;

        // convert logical vertices -> screen space
        std::vector<sf::Vertex> screenVerts;
        screenVerts.reserve(r.vertices.size());
        sf::Vector2u winSize = window_.getSize();
        float cx = static_cast<float>(winSize.x) * 0.5f;
        float cy = static_cast<float>(winSize.y) * 0.5f;

        for (const auto& v : r.vertices)
        {
            float sx = cx + static_cast<float>(v.position.x * tr.scale + tr.offsetX);
            float sy = cy - static_cast<float>(v.position.y * tr.scale + tr.offsetY);
            screenVerts.emplace_back(sf::Vector2f(sx, sy), v.color);
        }

        // draw as line strip
        window_.draw(screenVerts.data(), static_cast<std::size_t>(screenVerts.size()), sf::LinesStrip);
    }

private:
    sf::RenderWindow& window_;
};

#endif // UI_SYSTEMS_H
