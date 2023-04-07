#include "EditorLayer.h"

/* *******Entry point******* */
#include "Mirage/Core/EntryPoint.h"

namespace Mirage
{
    class MirageEditor : public Application
    {
    public:
        MirageEditor(ApplicationSpecification specification)
            : Application(specification)
        {
            PushLayer(new EditorLayer());
        }

        ~MirageEditor()
        {
        }
    };

    Application* CreateApplication(ApplicationCommandLineArgs args)
    {
    	ApplicationSpecification specification;
	
    	specification.Name = "Mirage Editor";
    	specification.WorkingDirectory = std::filesystem::current_path();
    	specification.CommandLineArgs = args;
	
        return new MirageEditor(specification);
    }
}
