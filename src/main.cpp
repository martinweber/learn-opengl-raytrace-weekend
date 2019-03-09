
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>

#include "StandardOutSink.h"
#include "RaytraceApp.h"

int main(int argc, char** argv)
{
	// init Logging
	//
	auto logWorker = g3::LogWorker::createLogWorker();
	const std::string logfilePath = ".";

	auto logHandle = logWorker->addDefaultLogger("Raytrace-in-a-Weekend", logfilePath);
	auto stdOutHandle = logWorker->addSink(std::make_unique<tachyon::StandardOutSink>(), &tachyon::StandardOutSink::ReceiveLogMessage);
	g3::initializeLogging(logWorker.get());
	std::future<std::string> logfileName = logHandle->call(&g3::FileSink::fileName);


	LOG(INFO) << "Raytrace-in-a-Weekend Started";

	RaytraceApp app;

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		LOG(WARNING) << e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nCmdShow)
{
	return main(__argc, __argv);
}