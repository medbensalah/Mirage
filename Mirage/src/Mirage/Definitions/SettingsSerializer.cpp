#include "MrgPch.h"

#include <yaml-cpp/yaml.h>
#include "SettingsSerializer.h"

#include "Paths.h"
#include "Physics.h"
#include "Mirage/Core/Application.h"

namespace Mirage
{
	void SettingsSerializer::Serialize(const std::filesystem::path& path)
	{
		// serialize settings into yaml file
		YAML::Emitter out;
		out << YAML::BeginMap;
			out << YAML::Key << "Settings";
			out << YAML::Value << YAML::BeginMap;
		
				out << YAML::Key << "Physics";
				out << YAML::Value << YAML::BeginMap;
					out << YAML::Key << "UpdateEveryFrame" << YAML::Value << Physics2D::UpdateEveryFrame;
					out << YAML::Key << "FPS" << YAML::Value << Physics2D::FPS;
					out << YAML::Key << "Gravity" << YAML::Value << Physics2D::Gravity;
					out << YAML::Key << "VelocityIterations" << YAML::Value << Physics2D::VelocityIterations;
					out << YAML::Key << "PositionIterations" << YAML::Value << Physics2D::PositionIterations;
				out << YAML::EndMap;
		
				out << YAML::Key << "Window";
				out << YAML::Value << YAML::BeginMap;
					out << YAML::Key << "VSync" << YAML::Value << Application::Get().GetWindow().IsVSync();
				out << YAML::EndMap;
			out << YAML::EndMap;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}

	void SettingsSerializer::Deserialize(const std::filesystem::path& path)
	{
		if (exists(path))
		{
			// deserialize settings from yaml file
			YAML::Node data = YAML::LoadFile(path.string());
			auto settings = data["Settings"];

			if (settings)
			{
				auto physics = settings["Physics"];
				if (physics)
				{
					auto updateEveryFrame = physics["UpdateEveryFrame"];
					if (updateEveryFrame)
						Physics2D::UpdateEveryFrame = updateEveryFrame.as<bool>();

					auto fps = physics["FPS"];
					if (fps)
						Physics2D::FPS = fps.as<float>();

					auto gravity = physics["Gravity"];
					if (gravity)
						Physics2D::Gravity = gravity.as<float>();

					auto velocityIterations = physics["VelocityIterations"];
					if (velocityIterations)
						Physics2D::VelocityIterations = velocityIterations.as<int>();

					auto positionIterations = physics["PositionIterations"];
					if (positionIterations)
						Physics2D::PositionIterations = positionIterations.as<int>();
				}
				auto window = settings["Window"];
				if (window)
				{
					auto vsync = window["VSync"];
					if (vsync)
						Application::Get().GetWindow().SetVSync(vsync.as<bool>());
				}
			}
		}
	}
}
