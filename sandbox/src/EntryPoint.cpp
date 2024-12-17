#include "Editor/Editor.h"
#include "Forge.h"
#include <memory>

int main(int argc, char* argv[]) {
    Forge::Forge forgeApp;

    std::shared_ptr<Forge::Editor> editor = std::make_shared<Forge::Editor>();
    forgeApp.PushModule(editor);
    forgeApp.Run();

    return 0;
}
