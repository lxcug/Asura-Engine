#include "RenderCommand.h"


namespace Asura {
    Scope<RendererAPI> RenderCommand::s_rendererAPI = RendererAPI::createRendererAPI();
}