// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
/* 追加ライブラリへのリンク */
#ifdef _DEBUG
#define RSSDK_EXT "_d.lib"
#else
#define RSSDK_EXT ".lib"
#endif
#pragma comment( lib, "libpxc" RSSDK_EXT)
#pragma comment( lib, "libpxcutils" RSSDK_EXT)

#ifndef PCH_H
#define PCH_H

// TODO: ここでプリコンパイルするヘッダーを追加します
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

/* Real Sense SDK 関連 */
#include "pxcsensemanager.h"
#include "pxcspeechrecognition.h"

#endif //PCH_H
