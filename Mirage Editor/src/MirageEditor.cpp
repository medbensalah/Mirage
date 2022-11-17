#include "EditorLayer.h"

/* *******Entry point******* */
#include "Mirage/Core/EntryPoint.h"

namespace Mirage
{
    class MirageEditor : public Application
    {
    public:
        MirageEditor(ApplicationCommandLineArgs args)
            : Application("Mirage Editor", args)
        {
            PushLayer(new EditorLayer());
        }

        ~MirageEditor()
        {
        }
    };

    Application* CreateApplication(ApplicationCommandLineArgs args)
    {
        return new MirageEditor(args);
    }
}
