// RS_SampleVoiceRecongnition.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"

int main()
{
	//Japanese shown
	std::locale::global(std::locale("japanese"));

	/*
	Create Session instance
	After, use to get instance of PXCSpeechRecognition
	 */
	PXCSession *session = PXCSession::CreateInstance();
	if (session == nullptr) {
		wprintf_s(L"Session not created by PXCSession\n");
		return 1;
	}

	/*
	The instance of PXCSpeechRecognition is gotten by CreateImpl method
	Return value is pxcStatus type, error processing in "if(sts != pxcStatus::PXC_STATUS_NO_ERROR)"
	*/
	PXCSpeechRecognition *sr = nullptr;
	pxcStatus sts = session->CreateImpl<PXCSpeechRecognition>(&sr);
	if (sts != pxcStatus::PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to create an instance of the PXCSpeechRecognition\n");
		return 2;
	}

	/*
	Read QueryProfile and initiallize module
	PXCSpeechRecognition::ProfileInfo conatins the setting item such as language
	Set this, reflect using SetProfile
	*/
	PXCSpeechRecognition::ProfileInfo pinfo;
	sr->QueryProfile(0, &pinfo);

	pinfo.language = PXCSpeechRecognition::LanguageType::LANGUAGE_JP_JAPANESE;

	sts = sr->SetProfile(&pinfo);

	if (sts != pxcStatus::PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to Configure the Module\n");
		return 3;
	}

	//Command mode or Dictation mode
	sts = sr->SetDictation();
	if (sts != pxcStatus::PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to set the recognition mode\n");
		return 4;
	}


	/*
	Override OnRecognition() and OnAlert() method

	OnRecognition() is called when got result of recognition,
	@Param PXCSpeechRecognition::RecognitionData

	OnAlert() is called when happend caution,
	@Param PXCSpeechRecognition::AlertData
	In the AlertData has information of speak start-end and sound small
	*/
	/*音声認識部分*/
	class MyHandler : public PXCSpeechRecognition::Handler {
	public:
		std::string path = "C:/Users/user/Dropbox/workspace/G5 Graduate Study/smartspeaker/text/input.txt";

		virtual void PXCAPI OnRecognition(const PXCSpeechRecognition::RecognitionData *data) {
			wprintf_s(L"Output %s\n", wide_to_ansi(data->scores[0].sentence));
			std::ofstream ofs(path);
			ofs << wide_to_ansi(data->scores[0].sentence);  //2018.05.02 問題の箇所
			ofs.close();
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
