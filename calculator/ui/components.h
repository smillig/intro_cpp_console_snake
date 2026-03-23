#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <optional>
#include <mutex>
#include "../core/expression.h" // adjust include path as needed
#include <SFML/Graphics.hpp>

// Transform component: simple 2D transform for pan/zoom
struct TransformComponent
{
    double offsetX = 0.0;
    double offsetY = 0.0;
    double scale = 50.0; // pixels per unit
};

// Renderable component: color & vertex buffer
struct RenderableComponent
{
    sf::Color color = sf::Color::White;
    float thickness = 2.0f;
    std::vector<sf::Vertex> vertices;
    void clear() { vertices.clear(); }
};

// Plot configuration: holds shared AST and sampling parameters
struct PlotConfigComponent
{
    std::shared_ptr<ASTNode> ast;                     // shared AST for this plot
    std::function<double(double)> fun;                // convenience wrapper (created from ast)
    sf::Color color = sf::Color::White;
    double xMin = -5.0, xMax = 5.0;
    size_t samples = 800;
    bool visible = true;
};

// Input buffer component for UI text input
struct InputBufferComponent
{
    std::string text;
    bool dirty = false;
};

// Simple tag component for identifying graph entities
struct GraphTag {};

// Simple mutex-protected history component storing representations
struct HistoryComponent
{
    size_t capacity = 10;
    std::vector<std::pair<std::string, std::shared_ptr<ASTNode>>> entries;
    mutable std::mutex mtx;
};

#endif // UI_COMPONENTS_H
