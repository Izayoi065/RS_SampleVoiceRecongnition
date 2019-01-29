// RS_SampleVoiceRecongnition.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"

int main()
{
	std::locale::global(std::locale("japanese"));

	PXCSession *session = PXCSession::CreateInstance();
	if (session == nullptr) {
		wprintf_s(L"Session not created by PXCSession\n");
		return 1;
	}

	PXCSpeechRecognition *sr = nullptr;
	pxcStatus sts = session->CreateImpl<PXCSpeechRecognition>(&sr);
	if (sts != pxcStatus::PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to create an instance of the PXCSpeechRecognition\n");
		return 2;
	}

	PXCSpeechRecognition::ProfileInfo pinfo;
	sr->QueryProfile(0, &pinfo);

	pinfo.language = PXCSpeechRecognition::LanguageType::LANGUAGE_JP_JAPANESE;

	sts = sr->SetProfile(&pinfo);

	if (sts != pxcStatus::PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to Configure the Module\n");
		return 3;
	}

	sts = sr->SetDictation();
	if (sts != pxcStatus::PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to set the recognition mode\n");
		return 4;
	}

	class MyHandler : public PXCSpeechRecognition::Handler {
	public:
		virtual void PXCAPI OnRecognition(const PXCSpeechRecognition::RecognitionData *data) {
			wprintf_s(L"Output %s\n", data->scores[0].sentence);
		}

		virtual void PXCAPI OnAlert(const PXCSpeechRecognition::AlertData *data) {
			if (data->label == PXCSpeechRecognition::ALERT_SPEECH_BEGIN) {
				wprintf_s(L"Alert: SPEECH_BEGIN\n");
			}
			else if (data->label == PXCSpeechRecognition::ALERT_SPEECH_END) {
				wprintf_s(L"Alert: SPEECH_END\n");
			}
			else if (data->label == PXCSpeechRecognition::ALERT_VOLUME_LOW) {
				wprintf_s(L"Alert: VOLUME_LOW\n");
			}
		}
	};

	MyHandler handler;

	sts = sr->StartRec(nullptr, &handler);
	if (sts != pxcStatus::PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to start the handler\n");
		return 5;
	}

	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) break;
	}

	sr->StopRec();
	return 0;
}
