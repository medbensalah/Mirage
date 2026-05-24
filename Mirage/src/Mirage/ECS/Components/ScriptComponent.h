#pragma once

namespace Mirage
{	
	struct ScriptComponent
	{
		std::string ClassName = "Script";

		
		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
		ScriptComponent(const std::string& name)
		: ClassName(name) {}
	};
}
