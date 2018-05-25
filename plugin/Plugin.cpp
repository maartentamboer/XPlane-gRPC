#include "XPLMPlugin.h"
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include <cstring>
#include <string>
#include <sstream>

#include "Server.h"
#include "DataRefManager.h"

namespace
{
CServer srv;
}




float FlightLoopCB(float elapsedMe, float elapsedSim, int counter, void * refcon)
{
  CDataRefManager::GetInstance().FlightLoop();
	return -1;
}

PLUGIN_API int XPluginStart(char * outName, char * outSig, char * outDesc) {
    // Plugin details
	strcpy(outName, "gRPC for XPlane");
	strcpy(outSig, "com.maartentamboer.XPlane-gRPC");
	strcpy(outDesc, "More information https://github.com/maartentamboer/XPlane-gRPC");

    // You probably want this on
	XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);

	XPLMRegisterFlightLoopCallback(FlightLoopCB, 1.0, NULL);

	srv.Start();

	return 1;
}

PLUGIN_API void	XPluginStop(void) {
	srv.Stop();
	XPLMUnregisterFlightLoopCallback(FlightLoopCB, NULL);
}

PLUGIN_API void XPluginDisable(void) {
}

PLUGIN_API int XPluginEnable(void) {
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID, intptr_t inMessage, void * inParam) {
}
