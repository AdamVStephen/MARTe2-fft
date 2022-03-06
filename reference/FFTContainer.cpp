//******************************************************************************
//
//      $Log: FFTContainer.cpp,v $
//      Revision 1.3  2012/03/12 14:25:34  dalves
//      Added simple check
//
//      Revision 1.2  2011/02/10 15:04:26  dalves
//      Changed http directory resource name
//
//      Revision 1.1  2011/01/14 10:07:05  dalves
//      Added the FFTContainer and made output signals optional
//
//
//******************************************************************************

#include "FFTContainer.h"
#include "CDBExtended.h"

#include "DDBInputInterface.h"
#include "DDBOutputInterface.h"

bool FFTContainer::Initialise(ConfigurationDataBase& cdbData) {

    CDBExtended cdb(cdbData);

    // Load the output gain if specified
    if(!cdb.ReadInt32(NFFT, "FFTNumberOfPoints")) {
        AssertErrorCondition(InitialisationError, "FFTContainer %s::Initialise: FFTNumberOfPoints entry not found", Name());
    	return False;
    } else {
        if(NFFT <= 0) {
            AssertErrorCondition(InitialisationError, "FFTContainer %s::Initialise: FFTNumberOfPoints = %d <= 0", Name(), NFFT);
            return False;
        }
    }

    if(!cdb.ReadInt32(NCYCLES, "PerformFFTEveryNCycles", 1)) {
        AssertErrorCondition(Warning, "FFTContainer %s::Initialise: PerformFFTEveryNCycles entry not found, assuming = %d", Name(), NCYCLES);
    } else if(NCYCLES < 1) {
        AssertErrorCondition(Warning, "FFTContainer %s::Initialise: PerformFFTEveryNCycles < 1", Name());
	return False;
    }

    ///
    if(!cdb.ReadInt32(Fs, "SamplingFrequency")) {
        AssertErrorCondition(InitialisationError, "FFTContainer %s::Initialise: SamplingFrequency entry not found", Name());
	return False;
    }

    freqRes = ((float)(Fs))/((float)(NFFT));

    ///
    if(!cdb.ReadInt32(numberOfTopFreqs2Display, "NumberOfTopFreqs2Display", 10)) {
        AssertErrorCondition(Warning, "FFTContainer %s::Initialise: NumberOfTopFreqs2Display entry not found, assuming %d", Name(), numberOfTopFreqs2Display);
    }

    if(!cdb.ReadInt32(httpRefreshMsecTime, "HttpRefreshMsecTime", 250)) {
        AssertErrorCondition(Warning, "FFTContainer %s::Initialise: HttpRefreshMsecTime entry not found, assuming %d", Name(), httpRefreshMsecTime);
    }

    ////////////////////////////////////////////////////
    //                Add interfaces to DDB           //
    ////////////////////////////////////////////////////
    if(!AddInputInterface(input, "InputInterface")){
        AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s failed to add input interface",Name());
        return False;
    }

    ////////////////////////////////////////////////////
    //                Add input signals               //
    ////////////////////////////////////////////////////
    if(!cdb->Move("InputSignals")){
        AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s did not specify InputSignals entry",Name());
        return False;
    }

    if(cdb->NumberOfChildren() != 1) {
        AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s ObjectLoadSetup Failed NumberOfChildren() = %d, must be 1", Name(), input->BufferWordSize());
        return False;
    }

    if(!input->ObjectLoadSetup(cdb,NULL)){
        AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s ObjectLoadSetup Failed DDBInterface %s ",Name(),input->InterfaceName());
        return False;
    }

    const DDBSignalDescriptor *descriptor = input->SignalsList();
    if((inputSignalNumberOfPoints = descriptor->SignalSize()) < 1) {
        AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s ObjectLoadSetup Failed SignalSize() < 1", Name());
        return False;
    }

    cdb->MoveToFather();

    ////////////////////////////////////////////////////
    //                Add output signals              //
    ////////////////////////////////////////////////////

    FString signalName;
    FString signalType;
    FString newSignalName;

    if(!cdb->Move("OutputSignals")) {
        AssertErrorCondition(Warning,"FFTContainer::Initialise: %s did not specify OutputSignals entry",Name());
    } else {
	if(!cdb->Move("OutputReal")){
	    AssertErrorCondition(Warning,"FFTContainer::Initialise: %s did not specify OutputReal inside OutputSignals",Name());
	} else {
	    signalName.SetSize(0);
	    if(!cdb.ReadFString(signalName, "SignalName")) {
		AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s did not specify OutputReal: SignalName missing",Name());
		return False;
	    }
	    signalType.SetSize(0);
	    if(!cdb.ReadFString(signalType, "SignalType")) {
		AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s did not specify OutputReal: SignalType missing",Name());
		return False;
	    }
	    newSignalName.SetSize(0);
	    newSignalName.Printf("%s[%d]", signalName.Buffer(), NFFT/2+1);
	    /// Add output interface
	    if(!AddOutputInterface(outputReal, "OutputRealInterface")){
		AssertErrorCondition(Warning,"FFTContainer::Initialise: %s failed to add outputReal",Name());
	    }
	    outputReal->AddSignal(newSignalName.Buffer(), signalType.Buffer());

	    cdb->MoveToFather();
	}
	    		
	
	if(!cdb->Move("OutputImag")){
	    AssertErrorCondition(Warning,"FFTContainer::Initialise: %s did not specify OutputImag inside OutputSignals",Name());
	} else {
	    signalName.SetSize(0);
	    if(!cdb.ReadFString(signalName, "SignalName")) {
		AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s did not specify OutputImag: SignalName missing",Name());
		return False;
	    }
	    signalType.SetSize(0);
	    if(!cdb.ReadFString(signalType, "SignalType")) {
		AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s did not specify OutputImag: SignalType missing",Name());
		return False;
	    }
	    newSignalName.SetSize(0);
	    newSignalName.Printf("%s[%d]", signalName.Buffer(), NFFT/2+1);
	    // Add output interface
	    if(!AddOutputInterface(outputImag, "OutputImagInterface")){
		AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s failed to add outputImag",Name());
		return False;
	    }
	    outputImag->AddSignal(newSignalName.Buffer(), signalType.Buffer());

	    cdb->MoveToFather();
	}
	
	if(!cdb->Move("OutputPow")){
	    AssertErrorCondition(Warning,"FFTContainer::Initialise: %s did not specify OutputPow inside OutputSignals",Name());
	} else {
	    signalName.SetSize(0);
	    if(!cdb.ReadFString(signalName, "SignalName")) {
		AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s did not specify OutputPow: SignalName missing",Name());
		return False;
	    }
	    signalType.SetSize(0);
	    if(!cdb.ReadFString(signalType, "SignalType")) {
		AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s did not specify OutputPow: SignalType missing",Name());
		return False;
	    }
	    newSignalName.SetSize(0);
	    newSignalName.Printf("%s[%d]", signalName.Buffer(), NFFT/2+1);
	    // Add output interface
	    if(!AddOutputInterface(outputPow, "OutputPowInterface")){
		AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s failed to add outputPow",Name());
		return False;
	    }
	    outputPow->AddSignal(newSignalName.Buffer(), signalType.Buffer());

	    cdb->MoveToFather();
	}

	if(!cdb->Move("OutputPhase")){
	    AssertErrorCondition(Warning,"FFTContainer::Initialise: %s did not specify OutputPhase inside OutputSignals",Name());
	} else {
	    signalName.SetSize(0);
	    if(!cdb.ReadFString(signalName, "SignalName")) {
		AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s did not specify OutputPhase: SignalName missing",Name());
		return False;
	    }
	    signalType.SetSize(0);
	    if(!cdb.ReadFString(signalType, "SignalType")) {
		AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s did not specify OutputPhase: SignalType missing",Name());
		return False;
	    }
	    newSignalName.SetSize(0);
	    newSignalName.Printf("%s[%d]", signalName.Buffer(), NFFT/2+1);
	    // Add output interface
	    if(!AddOutputInterface(outputPhase, "OutputPhaseInterface")){
		AssertErrorCondition(InitialisationError,"FFTContainer::Initialise: %s failed to add outputPhase",Name());
		return False;
	    }
	    outputPhase->AddSignal(newSignalName.Buffer(), signalType.Buffer());

	    cdb->MoveToFather();
	}
    }
	
// fftw memory management functions aren't working ...    
// in  = (float *)fftw_malloc(sizeof(float)*NFFT);
// out = (fftwf_complex *)fftw_malloc(sizeof(fftwf_complex)*NFFT);
    in  = (float *)malloc(sizeof(float)*NFFT);
    out = (fftwf_complex *)malloc(sizeof(fftwf_complex)*NFFT);
    
    fftPlan = fftwf_plan_dft_r2c_1d(NFFT, in, out, fftFlags);
    
    return True;
}

bool FFTContainer::Execute(GAM_FunctionNumbers functionNumber) {
    
    // Get input and output data pointers
    float *inputData      = (float *)input->Buffer();
    
    input->Read();
    
    if(inputSignalNumberOfPoints >= NFFT) {
    	memcpy(in, inputData, NFFT*sizeof(float));
    } else {
    	memmove(in, in+inputSignalNumberOfPoints, (NFFT-inputSignalNumberOfPoints)*sizeof(float));
	memcpy(in+NFFT-inputSignalNumberOfPoints, inputData, inputSignalNumberOfPoints*sizeof(float));
    }
    
    if(functionNumber == GAMPrepulse) {
	delayIndex = 1;
    }
     
    if(delayIndex == NCYCLES) {
        fftwf_execute(fftPlan); /* repeat as needed */
        float *outputRealData;
        if(outputReal) {
            outputRealData = (float *)outputReal->Buffer();
        }
        float *outputImagData;
        if(outputImag) {
            outputImagData = (float *)outputImag->Buffer();
        }
        float *outputPowData;
        if(outputPow) {
            outputPowData = (float *)outputPow->Buffer();
        }
        float *outputPhaseData;
        if(outputPhase) {
            outputPhaseData = (float *)outputPhase->Buffer();
        }
        for(int i = 0 ; i < NFFT/2+1 ; i++) {
            if(outputReal) {
                outputRealData[i] = out[i][0];
            }
            if(outputImag) {
                outputImagData[i] = out[i][1];
            }
            if(outputPow) {
                outputPowData[i] = out[i][0]*out[i][0]+out[i][1]*out[i][1];
            }
            if(outputPhase) {
                outputPhaseData[i] = atan2f(out[i][1], out[i][0]);
            }
        }
        delayIndex = 1;
    } else {
        delayIndex++;
    }
    
    if(outputReal) {
	outputReal->Write();
    }
    if(outputReal) {
	outputImag->Write();
    }
    if(outputPow) {
	outputPow->Write();
    }
    if(outputPhase) {
	outputPhase->Write();
    }
    
    return True;
}

bool FFTContainer::ProcessHttpMessage(HttpStream &hStream) {
    int32 DISPLAY_N_FREQS = numberOfTopFreqs2Display;

    const uint32 CANVAS_WIDTH  = 950;
    const uint32 CANVAS_HEIGHT = 600;
    const uint32 N_HORZ_TICKS  = 10;
    const uint32 N_VERT_TICKS  = 10;

    float *bubu;
    bubu = (float *)malloc((NFFT/2+1)*sizeof(float));

    float *dispFreq;
    float *dispPower;

    dispFreq  = (float *)malloc(DISPLAY_N_FREQS*sizeof(float));
    dispPower = (float *)malloc(DISPLAY_N_FREQS*sizeof(float));

    for(int i = 0 ; i < DISPLAY_N_FREQS ; i++) {
	dispPower[i] =  0;
	dispFreq[i]  = -1;
    }
    
    /* Build data string */
    FString dataStringArray = "var data = [";
    FString auxDataStringArray;
    FString freqStringArray = "var freq = [";
    FString auxFreqStringArray;
    FString powStringArray  = "var pow  = [";
    FString auxPowStringArray;
    for(int i = 0 ; i < NFFT/2+1 ; i++) {
	bubu[i] = 2.0*sqrt(out[i][0]*out[i][0] + out[i][1]*out[i][1])/((float)(NFFT));

	if(i < NFFT/2) {
	    auxDataStringArray.SetSize(0);
	    auxDataStringArray.Printf("%f, ", bubu[i]);
	    dataStringArray += auxDataStringArray;
	    auxDataStringArray.Seek(0);
	}

	for(int j = DISPLAY_N_FREQS-1 ; j >= 0 ; j--) {
	    if(bubu[i] > dispPower[j]) {
		for(int k = 0 ; k < j ; k++) {
		    dispPower[k] = dispPower[k+1];
		    dispFreq[k] = dispFreq[k+1];
		}
		dispPower[j] = bubu[i];
		dispFreq[j]  = i*freqRes;
		break;
	    }
	}
    }
    auxDataStringArray.SetSize(0);
    auxDataStringArray.Printf("%f];", bubu[NFFT/2]);
    dataStringArray += auxDataStringArray;
    auxDataStringArray.Seek(0);

    for(int i = DISPLAY_N_FREQS-1 ; i > 0 ; i--) {
	auxFreqStringArray.SetSize(0);
	auxFreqStringArray.Printf("%f, ", dispFreq[i]);
	auxFreqStringArray.Seek(0);
	freqStringArray += auxFreqStringArray;

	auxPowStringArray.SetSize(0);
	auxPowStringArray.Printf("%f, ", dispPower[i]);
	auxPowStringArray.Seek(0);
	powStringArray += auxPowStringArray;
    }
    auxFreqStringArray.SetSize(0);
    auxFreqStringArray.Printf("%f];", dispFreq[0]);
    freqStringArray += auxFreqStringArray;
    auxFreqStringArray.Seek(0);

    auxPowStringArray.SetSize(0);
    auxPowStringArray.Printf("%f];", dispPower[0]);
    powStringArray += auxPowStringArray;
    auxPowStringArray.Seek(0);

    /* Build xticks labels string */
    FString xTicks = "var labels = [";
    FString xTicksAux;
    for(int i = 0 ; i < N_HORZ_TICKS ; i++) {
	xTicksAux.Printf("%d,", i*Fs/2/N_HORZ_TICKS);
	xTicks += xTicksAux;
	xTicksAux.Seek(0);
    }
    xTicksAux.Printf("%d];", Fs/2);
    xTicks += xTicksAux;
    xTicksAux.Seek(0);

    /* Check refresh state */
    FString refreshState;
    refreshState.SetSize(0);
    hStream.Seek(0);
    if(hStream.Switch("InputCommands.Refresh")){
        hStream.Seek(0);
        hStream.GetToken(refreshState, "");
        hStream.Switch((uint32)0);
    }
    if(refreshState.Size() > 0) { 
    	REFRESH = (strcmp(refreshState.Buffer(), "0") != 0);
    }

    /* Check yMin */
    FString sYMin;
    sYMin.SetSize(0);
    hStream.Seek(0);
    if(hStream.Switch("InputCommands.minY")) {
        hStream.Seek(0);
        hStream.GetToken(sYMin, "");
        hStream.Switch((uint32)0);
	if(sYMin.Size() > 0) {
	    if(yMin == NULL) {
		yMin = (float *)malloc(sizeof(float));
	    }
	    yMin[0] = atof(sYMin.Buffer());
	} else {
	    if(yMin != NULL) {
		free((void*&)yMin);
		yMin = NULL;
	    }
      	}
    }

    /* Check yMax */
    FString sYMax;
    sYMax.SetSize(0);
    hStream.Seek(0);
    if(hStream.Switch("InputCommands.maxY")) {
        hStream.Seek(0);
        hStream.GetToken(sYMax, "");
        hStream.Switch((uint32)0);
	if(sYMax.Size() > 0) {
	    if(yMax == NULL) {
		yMax = (float *)malloc(sizeof(float));
	    }
	    yMax[0] = atof(sYMax.Buffer());
	} else {
	    if(yMax != NULL) {
		free((void*&)yMax);
		yMax = NULL;
	    }
	}
    }

    /* Check refresh data */
    FString ajaxString;
    ajaxString.Printf("%s\n%s\n%s", dataStringArray.Buffer(), freqStringArray.Buffer(), powStringArray.Buffer());
    FString refreshData;
    refreshData.SetSize(0);
    hStream.Seek(0);
    if(hStream.Switch("InputCommands.RefreshData")){
        hStream.Seek(0);
        hStream.GetToken(refreshData, "");
        hStream.Switch((uint32)0);
	hStream.Printf("%s\n", ajaxString.Buffer());
	hStream.WriteReplyHeader(True);
	return True;
    }


    hStream.SSPrintf("OutputHttpOtions.Content-Type","text/html");
    hStream.keepAlive = False;
    //copy to the client
    hStream.WriteReplyHeader(False);

    /* Start html document */
    hStream.Printf("<html>\n");

    /* Page head */
    hStream.Printf("<head>\n");

    /* Page title */
    hStream.Printf("<title>FFTContainer</title>\n");

    /* Include relevant RGraph libraries */
    hStream.Printf("<script src=\"/RGRAPH_LIB_DIR/RGraph.common.core.js\"></script> \n");
    hStream.Printf("<script src=\"/RGRAPH_LIB_DIR/RGraph.common.zoom.js\"></script> \n");
    hStream.Printf("<script src=\"/RGRAPH_LIB_DIR/RGraph.line.js\"></script> \n");

    /* End page head */
    hStream.Printf("</head>\n");

    hStream.Printf("<body onload=\"timedUpdate();\">\n");

    hStream.Printf("<canvas id=\"myLine\" width=\"%d\" height=\"%d\" style=\"position:absolute; left:225px; top:150px\"></canvas>\n", CANVAS_WIDTH, CANVAS_HEIGHT);

    hStream.Printf("<script>\n");

    hStream.Printf("var request = new XMLHttpRequest();\n");

    hStream.Printf("function timedUpdate() {\n");
    hStream.Printf("var url = \"?RefreshData\";\n");
    hStream.Printf("request.open('GET', url, true);\n");
    hStream.Printf("request.onreadystatechange = plot;\n");
    hStream.Printf("request.send(null);\n");
    if(REFRESH) {
	hStream.Printf("window.setTimeout(timedUpdate, %d);\n", httpRefreshMsecTime);
    }
    hStream.Printf("}\n");
    
    hStream.Printf("function updateTable(freqArray, powArray) {\n");
    hStream.Printf("for(var i = 0 ; i < %d ; i++) {\n", DISPLAY_N_FREQS);
    hStream.Printf("var cell = document.getElementById('myTable').rows[i+1].cells;\n");
    hStream.Printf("cell[0].innerHTML = freqArray[i];\n");
    hStream.Printf("cell[1].innerHTML = powArray[i];\n");
    hStream.Printf("}\n");
    hStream.Printf("}\n");

    hStream.Printf("function plot() {\n");
    hStream.Printf("if(this.readyState != 4 || this.status != 200) {\n");
    hStream.Printf("return;\n");
    hStream.Printf("}\n");
    hStream.Printf("RGraph.Clear(document.getElementById(\"myLine\"));\n");
    hStream.Printf("eval(request.responseText)\n");
    hStream.Printf("%s\n", xTicks.Buffer());
    hStream.Printf("line = new RGraph.Line(\"myLine\", data);\n");
    hStream.Printf("line.Set('chart.tickmarks', 'circle');\n");
    if(yMin) {
	hStream.Printf("line.Set('chart.ymin', %f);\n", yMin[0]);
    } else {
	hStream.Printf("line.Set('chart.ymin', 0);\n");
    }
    if(yMax) {
	hStream.Printf("line.Set('chart.ymax', %f);\n", yMax[0]);
    }
    // hStream.Printf("line.Set('chart.background.barcolor1', 'rgba(255,255,255,1)');\n");
    // hStream.Printf("line.Set('chart.background.barcolor2', 'rgba(255,255,255,1)');\n");
    // hStream.Printf("line.Set('chart.background.grid.color', 'rgba(238,238,238,1)');\n");
    hStream.Printf("line.Set('chart.colors', ['rgba(255,0,0,1)']);\n");
    //hStream.Printf("line.Set('chart.linewidth', 0);\n");
    hStream.Printf("line.Set('chart.filled', true);\n");
    hStream.Printf("line.Set('chart.background.grid.autofit', true);\n");
    hStream.Printf("line.Set('chart.background.grid.autofit.numhlines', %d);\n", N_VERT_TICKS);
    hStream.Printf("line.Set('chart.background.grid.autofit.numvlines', %d);\n", 2*N_HORZ_TICKS);
    hStream.Printf("line.Set('chart.xticks', %d);\n", N_HORZ_TICKS);
    hStream.Printf("line.Set('chart.yticks', %d);\n", N_VERT_TICKS);
    hStream.Printf("line.Set('chart.labels', labels);\n");
    //hStream.Printf("line.Set('chart.crosshairs', true);\n");
    hStream.Printf("line.Set('chart.text.size', 10);\n");
    hStream.Printf("line.Set('chart.title.xaxis', 'Frequency (Hz)');\n");
    hStream.Printf("line.Set('chart.title.yaxis', 'Amplitude');\n");
    hStream.Printf("line.Set('chart.gutter', 50);\n");
    hStream.Printf("line.Set('chart.zoom.mode', 'area');\n");
    hStream.Printf("line.Draw();\n");
    hStream.Printf("updateTable(freq, pow);\n");
    hStream.Printf("}\n");
    hStream.Printf("</script>\n");

    hStream.Printf("<h3>FFT number of points = %d</h3>\n", NFFT);
    hStream.Printf("<h3>Performing FFT every %d cycles</h3>\n", NCYCLES);
    hStream.Printf("<h3>Sampling frequency = %d Hz</h3>\n", Fs);
    hStream.Printf("<h3>Frequency resolution = %f Hz</h3>\n", freqRes);
    hStream.Printf("<table id=\"myTable\" border=\"1\"><tr><th>Frequency (Hz)</th><th>Amplitude</th></tr>\n");
    for(int i = DISPLAY_N_FREQS-1 ; i >= 0 ; i--) {
    	hStream.Printf("<tr bgcolor=\"%s\"><td>%f</td><td>%f</td></tr>\n", "#FFFFFF", dispFreq[i], dispPower[i]);
    }
    
    hStream.Printf("</table>\n");

    hStream.Printf("<br />\n");
    hStream.Printf("<br />\n");

    hStream.Printf("<form>\n");
    if(yMin) {
	hStream.Printf("Min Amplitude:<input type=\"text\" name=\"minY\" size=\"6\" value=\"%f\" /><br />\n", yMin[0]);
    } else {
	hStream.Printf("Min Amplitude:<input type=\"text\" name=\"minY\" size=\"6\" /><br />\n");
    }
    if(yMax) {
	hStream.Printf("Max Amplitude:<input type=\"text\" name=\"maxY\" size=\"6\" value=\"%f\" /><br />\n", yMax[0]);
    } else {
	hStream.Printf("Max Amplitude:<input type=\"text\" name=\"maxY\" size=\"6\" /><br />\n");
    }
    hStream.Printf("<button type=\"submit\" name=\"Update\" value=\"1\">Update</button>\n");
    hStream.Printf("</form>\n");

    hStream.Printf("<form>\n");
    hStream.Printf("<br />");
    if(REFRESH == 1) {
    	hStream.Printf("<button type=\"submit\" name=\"Refresh\" value=\"0\">Stop Auto Refresh</button>\n");
    } else {
    	hStream.Printf("<button type=\"submit\" name=\"Refresh\" value=\"1\">Resume Auto Refresh</button>\n");
    }
    hStream.Printf("</form>\n");

    hStream.Printf("</body>\n");

    hStream.Printf("</html>\n");

    free((void*&)bubu);
    free((void*&)dispFreq);
    free((void*&)dispPower);

    return True;
}
OBJECTLOADREGISTER(FFTContainer, "$Id: FFTContainer.cpp,v 1.3 2012/03/12 14:25:34 dalves Exp $")
