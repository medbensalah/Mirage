#include "EditorLayer.h"

/* *******Entry point******* */
#include "Mirage/Core/EntryPoint.h"

namespace
{
    bool IsEditorWorkingDirectory(const std::filesystem::path& path)
    {
        return std::filesystem::exists(path / "assets")
            && std::filesystem::exists(path / "Resources")
            && std::filesystem::exists(path / "Sandbox project");
    }

    std::filesystem::path FindEditorWorkingDirectoryFrom(const std::filesystem::path& seed)
    {
        std::filesystem::path current = std::filesystem::absolute(seed);

        while (!current.empty())
        {
            if (IsEditorWorkingDirectory(current))
                return current;

            const std::filesystem::path nestedEditorDirectory = current / "Mirage Editor";
            if (IsEditorWorkingDirectory(nestedEditorDirectory))
                return nestedEditorDirectory;

            if (current == current.root_path())
                break;

            current = current.parent_path();
        }

        return {};
    }

    std::filesystem::path ResolveEditorWorkingDirectory(const Mirage::ApplicationCommandLineArgs& args)
    {
        if (std::filesystem::path workingDirectory = FindEditorWorkingDirectoryFrom(std::filesystem::current_path()); !workingDirectory.empty())
            return workingDirectory;

        if (args.Count > 0 && args.Args && args.Args[0])
        {
            const std::filesystem::path executablePath = std::filesystem::absolute(args[0]);
            if (std::filesystem::path workingDirectory = FindEditorWorkingDirectoryFrom(executablePath.parent_path()); !workingDirectory.empty())
                return workingDirectory;
        }

        const std::filesystem::path fallbackDirectory = std::filesystem::current_path();
        MRG_CORE_WARN("Failed to resolve Mirage Editor working directory. Falling back to '{}'", fallbackDirectory.string());
        return fallbackDirectory;
    }
}

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
    	specification.WorkingDirectory = ResolveEditorWorkingDirectory(args);
    	specification.CommandLineArgs = args;
 	
        return new MirageEditor(specification);
    }
}
