// RS_SampleVoiceRecongnition.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include "strconv.h"

class MyHandler : public PXCSpeechRecognition::Handler {
public:
	virtual void PXCAPI OnRecognition(const PXCSpeechRecognition::RecognitionData *data) {
		wprintf_s(L"Output: %s\n", data->scores[0].sentence);
	}
	virtual void PXCAPI OnAlert(const PXCSpeechRecognition::AlertData *data) {
		if (data->label == PXCSpeechRecognition::ALERT_SPEECH_BEGIN)
			wprintf_s(L"Alert: SPEECH_BEGIN\n");
		else if (data->label == PXCSpeechRecognition::ALERT_SPEECH_END)
			wprintf_s(L"Alert: SPEECH_END\n");
	}
};

int main()
{
	// Create and retrieve a session
	PXCSession *session = PXCSession::CreateInstance();
	if (session == NULL) {
		wprintf_s(L"Session not created by PXCSession\n");
		return 1;
	}
	//Create an instance of the PXCSpeechRecognition
	PXCSpeechRecognition *sr = 0;
	pxcStatus sts = session->CreateImpl<PXCSpeechRecognition>(&sr);
	if (sts != pxcStatus::PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to create an instance of the PXCSpeechRecognition\n");
		return 2;
	}
	//Initialize the Module
	PXCSpeechRecognition::ProfileInfo pinfo;
	sr->QueryProfile(0, &pinfo);
	sts = sr->SetProfile(&pinfo);
	if (sts != pxcStatus::PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to Configure the Module\n");
		return 3;
	}
	//Set the Recognition mode
	//command and control mode or dictation mode
	sts = sr->SetDictation();
	if (sts != pxcStatus::PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to Set the Recognition mode \n");
		return 4;
	}

	MyHandler handler; // handler for PXCSpeechRecognition

	//Start the speech recognition with the event handler
	sts = sr->StartRec(NULL, &handler);
	if (sts != pxcStatus::PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to Start the handler \n");
		return 5;
	}
	while (true) { if (GetAsyncKeyState(VK_ESCAPE)) break; } //looping infinitely until escape is pressed
	//Stop the event handler that handles the speech recognition
	sr->StopRec();

	return 0;
}
