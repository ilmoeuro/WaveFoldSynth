//------------------------------------------------------------------------
// Copyright(c) 2021 Ilmo Euro.
//------------------------------------------------------------------------

#include "WaveFoldSynthController.h"
#include "WaveFoldSynthCIDs.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "pluginterfaces/base/ustring.h"
#include "base/source/fstreamer.h"

using namespace Steinberg;

namespace IlmoEuro {

//------------------------------------------------------------------------
// WaveFoldSynthController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthController::initialize (FUnknown* context)
{
	// Here the Plug-in will be instanciated

	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	// Here you could register some parameters
    Vst::Parameter *param;

    param = new Vst::RangeParameter(USTRING("Timbre"), kParamTimbre, USTRING(""), 0.f, 1.f, 0.1f);
    param->setPrecision(3);
    parameters.addParameter(param);

    param = new Vst::RangeParameter(USTRING("Timbre Attack"), kParamTimbreA, USTRING("s"), 0.f, 1.f, 0.1f);
    param->setPrecision(3);
    parameters.addParameter(param);

    param = new Vst::RangeParameter(USTRING("Timbre Decay"), kParamTimbreD, USTRING("s"), 0.f, 1.f, 0.1f);
    param->setPrecision(3);
    parameters.addParameter(param);

    param = new Vst::RangeParameter(USTRING("Timbre Sustain"), kParamTimbreS, USTRING(""), 0.f, 1.f, 0.1f);
    param->setPrecision(3);
    parameters.addParameter(param);

    param = new Vst::RangeParameter(USTRING("Timbre Release"), kParamTimbreR, USTRING("s"), 0.f, 1.f, 0.1f);
    param->setPrecision(3);
    parameters.addParameter(param);

    param = new Vst::RangeParameter(USTRING("Gain Attack"), kParamGainA, USTRING("s"), 0.f, 1.f, 0.1f);
    param->setPrecision(3);
    parameters.addParameter(param);

    param = new Vst::RangeParameter(USTRING("Gain Decay"), kParamGainD, USTRING("s"), 0.f, 1.f, 0.1f);
    param->setPrecision(3);
    parameters.addParameter(param);

    param = new Vst::RangeParameter(USTRING("Gain Sustain"), kParamGainS, USTRING(""), 0.f, 1.f, 0.1f);
    param->setPrecision(3);
    parameters.addParameter(param);

    param = new Vst::RangeParameter(USTRING("Gain Release"), kParamGainR, USTRING("s"), 0.f, 1.f, 0.1f);
    param->setPrecision(3);
    parameters.addParameter(param);

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthController::terminate ()
{
	// Here the Plug-in will be de-instanciated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthController::setComponentState (IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state)
		return kResultFalse;

    IBStreamer s(state, kLittleEndian);

    float timbre = 0.f;
    if (s.readFloat(timbre) == false)
    {
        return kResultFalse;
    }
    setParamNormalized(kParamTimbre, timbre);

    float timbreA = 0.f;
    if (s.readFloat(timbreA) == false)
    {
        return kResultFalse;
    }
    setParamNormalized(kParamTimbreA, timbreA);

    float timbreD = 0.f;
    if (s.readFloat(timbreD) == false)
    {
        return kResultFalse;
    }
    setParamNormalized(kParamTimbreD, timbreD);

    float timbreS = 0.f;
    if (s.readFloat(timbreS) == false)
    {
        return kResultFalse;
    }
    setParamNormalized(kParamTimbreS, timbreS);

    float timbreR = 0.f;
    if (s.readFloat(timbreR) == false)
    {
        return kResultFalse;
    }
    setParamNormalized(kParamTimbreR, timbreR);

    float gainA = 0.f;
    if (s.readFloat(gainA) == false)
    {
        return kResultFalse;
    }
    setParamNormalized(kParamGainA, gainA);

    float gainD = 0.f;
    if (s.readFloat(gainD) == false)
    {
        return kResultFalse;
    }
    setParamNormalized(kParamGainD, gainD);

    float gainS = 0.f;
    if (s.readFloat(gainS) == false)
    {
        return kResultFalse;
    }
    setParamNormalized(kParamGainS, gainS);

    float gainR = 0.f;
    if (s.readFloat(gainR) == false)
    {
        return kResultFalse;
    }
    setParamNormalized(kParamGainR, gainR);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthController::setState (IBStream* state)
{
	// Here you get the state of the controller
    IBStreamer s(state, kLittleEndian);

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthController::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor
    IBStreamer s(state, kLittleEndian);

	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API WaveFoldSynthController::createView (FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
        auto* view = new VSTGUI::VST3Editor (this, "view", "EDITOR.UIDESC");
		return view;
	}
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthController::setParamNormalized (Vst::ParamID tag, Vst::ParamValue value)
{
	// called by host to update your parameters
	tresult result = EditControllerEx1::setParamNormalized (tag, value);
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthController::getParamStringByValue (Vst::ParamID tag, Vst::ParamValue valueNormalized, Vst::String128 string)
{
	// called by host to get a string for given normalized value of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamStringByValue (tag, valueNormalized, string);
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthController::getParamValueByString (Vst::ParamID tag, Vst::TChar* string, Vst::ParamValue& valueNormalized)
{
	// called by host to get a normalized value from a string representation of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamValueByString (tag, string, valueNormalized);
}

//------------------------------------------------------------------------
} // namespace MyCompanyName
