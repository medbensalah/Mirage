#include "EditorLayer.h"

/* *******Entry point******* */
#include "Mirage/Core/EntryPoint.h"

namespace Mirage
{
    class MirageEditor : public Application
    {
    public:
        MirageEditor()
            : Application("Mirage Editor")
        {
            PushLayer(new EditorLayer());
        }

        ~MirageEditor()
        {
        }
    };

    Mirage::Application* CreateApplication()
    {
        return new MirageEditor();
    }
}
