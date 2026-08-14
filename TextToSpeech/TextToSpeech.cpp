#include <conio.h>
#include <sapi.h>
#include <windows.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <algorithm>
#include <cwctype>
#include <filesystem>
// sphelper.h uses GetVersionEx which is marked deprecated in newer SDKs and
// triggers C4996. Silence that deprecation warning only around this include.
#pragma warning(push)
#pragma warning(disable : 4996)
#include <sphelper.h>
#pragma warning(pop)
// Functions
void drawmenu(WCHAR* description, long rate, USHORT volume);
void drawVolume(long rate);
void drawspeak();
void drawVoice();
void drawSave();
void drawSpeed(long rate);
namespace fs = std::filesystem;

int main() {
	std::wstring text;
	std::wstring path;
	std::wstring route;
	std::wstring ext = L".wav";
	std::wstring truepath;
	CoInitialize(nullptr);
	ISpVoice* voice = nullptr;

	CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice,
		(void**)&voice);

	IEnumSpObjectTokens* voicenum = nullptr;
	ISpObjectToken* selectedVice = nullptr;

	// Get all the installed voices

	SpEnumTokens(SPCAT_VOICES, nullptr, nullptr, &voicenum);

	ULONG num = 0;
	/*
					for (ULONG i = 0; i < count; i++)
					{
									ISpObjectToken* token = nullptr;
									voicenum->Item(i, &token);

									WCHAR* description = nullptr;
									SpGetDescription(token, &description);

									std::wcout << i << L". " << description <<std::endl;


									CoTaskMemFree(description);
									token->Release();
					}
					*/


	long rate;
	voice->GetRate(&rate);
	long userrate = rate;
	USHORT volume;

	voice->GetVolume(&volume);
	long sound = volume;
	auto menu = drawmenu;

	while (true) {
		ISpObjectToken* token = nullptr;
		voicenum->Item(num, &token);

		WCHAR* description = nullptr;
		SpGetDescription(token, &description);
		system("cls");
		menu(description, rate, volume);
		char key = _getch();
		if (key == 's' || key == 'S') {
			system("cls");
			drawspeak();
			COORD position;
			position.X = 19;
			position.Y = 4;

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
			std::getline(std::wcin, text);


			
			char key2;
			char key3;

			bool exitSection = false;

			while (!exitSection)
			{
				key2 = _getch();

				if (key2 == 's' || key2 == 'S')
				{
					voicenum->Item(num, &selectedVice);
					voice->SetVoice(selectedVice);

					voice->Speak(text.c_str(), SPF_ASYNC, nullptr);

					bool speechStopped = false;

					while (!speechStopped)
					{
						if (_kbhit())
						{
							key3 = _getch();

							if (key3 == 'b' || key3 == 'B')
							{
								voice->Speak(NULL, SPF_PURGEBEFORESPEAK, NULL);

								exitSection = true;
								speechStopped = true;
							}

							// Other keys are ignored
						}

						// Check whether SAPI has finished.
						// WaitUntilDone(0) does NOT block.
						if (!exitSection)
						{
							if (voice->WaitUntilDone(0) == S_OK)
							{
								speechStopped = true;
							}
						}

						Sleep(10);
					}
				}
				else if (key2 == 'b' || key2 == 'B')
				{
					exitSection = true;
				}

				// Anything else is ignored.
			}

			
		}

		if (key == 'v' || key == 'V') {
			system("cls");
			drawVoice();
			char key2 = _getch();
			while (true) {

				if (key2 == '0') {
					num = 0;
					std::cout << num << std::endl;
					break;

				}
				else if (key2 == '1') {
					num = 1;
					std::cout << num << std::endl;
					break;

				}
				if (key2 == 'B' || key2 == 'b') {
					system("cls");
					break;
				}

			}
		}
		if (key == 'A' || key == 'a') {
			system("cls");
			drawSave();
			COORD position;
			position.X = 5;
			position.Y = 6;

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
			std::getline(std::wcin, path);
			position.X = 19;
			position.Y = 8;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
			std::getline(std::wcin, route);
			while (true) {
				char key = _getch();
				if (!path.empty() && key == 'S' || key == 's' && fs::is_directory(route)) {
					std::wstring lastFour = path.substr(
						path.length() >= 4 ? path.length() - 4 : 0
					);
					std::transform(lastFour.begin(), lastFour.end(), lastFour.begin(), towlower);
					if (lastFour != ext) {
						path = path + L".wav";
					}
					truepath = route + L"\\" + path;

					ISpStreamFormat* streamFormat = nullptr;
					WAVEFORMATEX* waveFormat = nullptr;

					voice->GetOutputStream(&streamFormat);

					GUID formatID = SPDFID_WaveFormatEx;

					streamFormat->GetFormat(
						&formatID,
						&waveFormat
					);

					ISpStream* stream = nullptr;

					SPBindToFile(
						truepath.c_str(),
						SPFM_CREATE_ALWAYS,
						&stream,
						&SPDFID_WaveFormatEx,
						waveFormat
					);

					voice->SetOutput(stream, TRUE);

					voice->Speak(
						text.c_str(),
						SPF_DEFAULT,
						nullptr
					);

					voice->SetOutput(nullptr, FALSE);

					stream->Release();

					CoTaskMemFree(waveFormat);
					streamFormat->Release();

					break;
				}
				else if (key == 'B' || key == 'b') {
					break;
				}
			}



		}
		if (key == 'R' || key == 'r') {
			while (true) {
				system("cls");
				drawSpeed(userrate);
				COORD position;
				position.X = 38;
				position.Y = 4;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
				std::cin >> userrate;
				std::cin.ignore();
				if (userrate > -10 and userrate < 10) {
					voice->SetRate(userrate);
					voice->GetRate(&rate);
				}
				else {
					continue;
				}
				char key = _getch();

				if (key == 'B' || key == 'b') {
					break;
				}
				else {
					continue;

				}
			}


		}
		if (key == 'l' || key == 'L') {
			while (true) {
				system("cls");
				drawVolume(sound);
				COORD position;
				position.X = 38;
				position.Y = 4;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
				std::cin >> sound;
				std::cin.ignore();
				if (sound > 0 and sound < 100) {
					voice->SetVolume(sound);
					voice->GetVolume(&volume);
				}
				else {
					continue;
				}
				
				char key = _getch();

				if (key == 'B' || key == 'b') {
					break;
				}
				else {
					continue;

				}
			}


		}
		if (key == 'Q' || key == 'q') {
			break;
		}

		CoTaskMemFree(description);
		token->Release();
	}

	voice->Release();
	CoUninitialize();
	return 0;
}
void drawmenu(WCHAR* description, long rate, USHORT volume) {
	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "|                    TEXT TO SPEECH                          |\n";
	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "|                                                            |\n";
	std::wcout << L"|  Current Voice : " << description << "\n";
	std::cout << "|  Speed         : " << rate << "        " << "                                 |\n";
	std::cout << "|  Volume        : " << volume << "        " << "                               |\n";
	std::cout << "|                                                            |\n";
	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "|                                                            |\n";
	std::cout << "|  [S]  Speak Text                                           |\n";
	std::cout << "|  [V]  Change Voice                                         |\n";
	std::cout << "|  [R]  Change Speech Rate                                   |\n";
	std::cout << "|  [L]  Change Volume                                        |\n";
	std::cout << "|  [A]  Save Audio                                           |\n";
	std::cout << "|  [Q]  Quit                                                 |\n";
	std::cout << "|                                                            |\n";
	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "\n";
	std::cout << "Press a key: ";
	
}
void drawspeak() {
	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "|                      SPEAKING...                           |\n";
	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "|                                                            |\n";
	std::cout << "|  Enter the text:                                           |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|    [B] Back                            [S] Speak           |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "+------------------------------------------------------------+\n";
}
void drawVoice() {
	ULONG count = 0;
	IEnumSpObjectTokens* voicenum = nullptr;

	// 1. Initialize the enumerator properly
	HRESULT hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &voicenum);
	if (FAILED(hr)) {
		std::cout << "Error: Could not enumerate voices.\n";
		return;
	}

	// 2. Safe to get count now
	voicenum->GetCount(&count);

	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "|                        SELECT VOICE                        |\n";
	std::cout << "+------------------------------------------------------------+\n";

	for (ULONG i = 0; i < count; i++) {
		ISpObjectToken* token = nullptr;

		if (SUCCEEDED(voicenum->Item(i, &token))) {
			WCHAR* description = nullptr;

			if (SUCCEEDED(SpGetDescription(token, &description))) {
				// Keep layout clean without breaking standard output streams
				
				std::wcout << L"| " << i << L". " << description <<L"|" << L"\n";
				CoTaskMemFree(description);
			}
			token->Release();
		}
	}

	std::cout << "+------------------------------------------------------------+\n";

	// 3. Clean up the enumerator
	voicenum->Release();
}
void drawSave() {
	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "|                       SAVE AUDIO                            |\n";
	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "|                                                            |\n";
	std::cout << "|  Enter file name:                                          |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|  > ";
	// User types the filename here
	std::cout << "\n";
	std::cout << "|                                                            |\n";
	std::cout << "|  Enter the path:                                           |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|  [S] Save          [B] Back                                |\n";
	std::cout << "|                                                            |\n";
	std::cout << "+------------------------------------------------------------+\n";
}
void drawSpeed(long rate){	
	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "|                      CHANGING SPEED                        |\n";
	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "|                                                            |\n";
	std::cout << "|  ENTER THE SPEED RATE(-10 TO 10):                          |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                       [B] Exit                             |\n";;
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "+------------------------------------------------------------+\n";
}
void drawVolume(long rate) {
	
	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "|                      CHANGING VOLUME                       |\n";
	std::cout << "+------------------------------------------------------------+\n";
	std::cout << "|                                                            |\n";
	std::cout << "|  ENTER THE VOLUME RATE(1 TO 100):                          |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                       [B] Exit                             |\n";
	std::cout << "|                                                            |\n";
	std::cout << "|                                                            |\n";
	std::cout << "+------------------------------------------------------------+\n";
}