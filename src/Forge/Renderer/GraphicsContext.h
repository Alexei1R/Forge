//
// Created by toor on 2024-07-24 .
//

#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#include <memory>

namespace Forge {

class GraphicsContext {
   public:
    virtual ~GraphicsContext() = default;
    static std::unique_ptr<GraphicsContext> Create();
};

}  // namespace Forge

#endif
