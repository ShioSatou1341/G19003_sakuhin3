//########## ヘッダーファイル読み込み ##########
#include "DxLib.h"

//########## マクロ定義 ##########
#define GAME_WIDTH			800	//画面の横の大きさ
#define GAME_HEIGHT			600	//画面の縦の大きさ
#define GAME_COLOR			32	//画面のカラービット

#define GAME_WINDOW_BAR		0					//タイトルバーはデフォルトにする
#define GAME_WINDOW_NAME	"GAME TITLE"		//ウィンドウのタイトル

#define GAME_FPS			    60	//FPSの数値	

#define IMAGE_TITLE_BK_PATH    ("IMAGE.\\タイトル背景参考.png")
#define IMAGE_TITLE_ROGO_PATH  ("IMAGE.\\Dissapeared.png")
#define IMAGE_TITLE_PUSH_PATH  ("IMAGE.\\タイトルpush.png")

#define IMAGE_PLAY_MAP_PATH    ("IMAGE.\\map3.png")
#define GAME_MAP_TATE_MAX     19
#define GAME_MAP_YOKO_MAX     25
#define GAME_MAP_KIND         1

#define IMAGE_MENU_BK_PATH     ("IMAGE.\\space1.png")

#define IMAGE_BATTLE_BK_PATH   ("IMAGE.\\戦闘画面レイアウト.png")

#define IMAGE_END_BK_PATH      ("IMAGE.\\エンド背景.png")
#define IMAGE_END_PUSH_PATH    ("IMAGE.\\エンド背景.png")

//#define MAP_DIV_WIDTH		48
//#define MAP_DIV_HEIGHT		48
//#define MAP_DIV_TATE		19
//#define MAP_DIV_YOKO		25
//#define MAP_DIV_NUM	MAP_DIV_TATE * MAP_DIV_YOKO

#define IMAGE_PLAYER_PATH            ("IMAGE.\\村娘.png")

#define IMAGE_ENEMY_PATH             ("IMAGE.\\九尾2.png")

//マウスのボタン
#define MOUSE_BUTTON_CODE	129	//0x0000〜0x0080まで

//キーボードの種類
#define KEY_CODE_KIND		256	//256種類

//パスの長さ
#define PATH_MAX			255	//255文字まで
#define NAME_MAX			255	//255文字まで

//エラーメッセージ
#define IMAGE_LOAD_ERR_TITLE	TEXT("画像読み込みエラー")

//エラーメッセージ
#define MUSIC_LOAD_ERR_TITLE	TEXT("音楽読み込みエラー")

//閉じるボタンを押したとき
#define MSG_CLOSE_TITLE			TEXT("終了メッセージ")
#define MSG_CLOSE_CAPTION		TEXT("ゲームを終了しますか？")

#define IDI_ICON1	1001

#define MSG_TITLE_BACK          TEXT("ゲーム中断")
#define MSG_TITLE_BACK_MESSAGE  TEXT("タイトルにもどりますか？")


enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_MENU,
	GAME_SCENE_BATTLE,
	GAME_SCENE_END
};	//ゲームのシーン

enum GAME_CHARA 
{

	n = -1,
	s = 1


};

enum CHARA_SPEED {
	CHARA_SPEED_LOW = 1,
	CHARA_SPEED_MIDI = 2,
	CHARA_SPEED_HIGH = 3
};

//int型のPOINT構造体
typedef struct STRUCT_I_POINT
{
	int x = -1;	//座標を初期化
	int y = -1;	//座標を初期化
}iPOINT;

//マウス構造体
typedef struct STRUCT_MOUSE
{
	int InputValue = 0;	//GetMouseInputの値を入れる
	int WheelValue = 0;	//マウスホイールの回転量(奥はプラス値 / 手前はマイナス値)
	iPOINT Point;		//マウスの座標が入る
	iPOINT OldPoint;	//マウスの座標(直前)が入る
	int OldButton[MOUSE_BUTTON_CODE] = { 0 };	//マウスのボタン入力(直前)が入る
	int Button[MOUSE_BUTTON_CODE] = { 0 };	//マウスのボタン入力が入る
}MOUSE;

typedef struct STRUCT_IMAGE
{
	char path[PATH_MAX];		//パス
	int handle;					//ハンドル
	int x;						//X位置
	int y;						//Y位置
	int width;					//幅
	int height;					//高さ
	BOOL IsDraw = FALSE;		//描画できるか
}IMAGE;	//画像構造体

typedef struct STRUCT_MUSIC
{
	char path[PATH_MAX];		//パス
	int handle;					//ハンドル
}MUSIC;	//音楽構造体

typedef struct STRUCT_CHARA
{
	IMAGE image;
	int speed;
	int CenterX;
	int CenterY;

	int Kaiso;
	int Muki;		//0：上　1：右　2：下　3：左

	int SeCou = 0;

	BOOL CanShot;
	int ShotReLoadCnt;
	int ShotReLoadCntMAX;

	

	RECT coll;
}CHARA;

//typedef struct STRUCT_MAP
//{
//	GAME_MAP_KIND kind;			//マップの種類
//	int x;						//X位置
//	int y;						//Y位置
//	int width;					//幅
//	int height;					//高さ
//}MAP;	//MAP構造体

//typedef struct STRUCT_MAP_IMAGE
//{
//	char path[PATH_MAX];
//	int handle[MAP_DIV_NUM];
//	int kind[MAP_DIV_NUM];
//	int width;
//	int height;
//}MAPCHIP;


//########## グローバル変数 ##########
//FPS関連
int StartTimeFps;				//測定開始時刻
int CountFps;					//カウンタ
float CalcFps;					//計算結果
int SampleNumFps = GAME_FPS;	//平均を取るサンプル数

//キーボードの入力を取得
char AllKeyState[KEY_CODE_KIND] = { '\0' };		//すべてのキーの状態が入る
char OldAllKeyState[KEY_CODE_KIND] = { '\0' };	//すべてのキーの状態(直前)が入る

//マウスの座標を取得
MOUSE mouse;

//ゲーム関連
int GameScene;		//ゲームシーンを管理


//########## プロトタイプ宣言 ##########
VOID MY_FPS_UPDATE(VOID);			//FPS値を計測、更新する
VOID MY_FPS_DRAW(VOID);				//FPS値を描画する
VOID MY_FPS_WAIT(VOID);				//FPS値を計測し、待つ

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//キーの入力状態を更新する
BOOL MY_KEY_DOWN(int);				//キーを押しているか、キーコードで判断する
BOOL MY_KEY_UP(int);				//キーを押し上げたか、キーコードで判断する
BOOL MY_KEYDOWN_KEEP(int, int);		//キーを押し続けているか、キーコードで判断する

VOID MY_MOUSE_UPDATE(VOID);			//マウスの入力情報を更新する
BOOL MY_MOUSE_DOWN(int);			//ボタンを押しているか、マウスコードで判断する
BOOL MY_MOUSE_UP(int);				//ボタンを押し上げたか、マウスコードで判断する
BOOL MY_MOUSEDOWN_KEEP(int, int);	//ボタンを押し続けているか、マウスコードで判断する

VOID MY_START(VOID);		//スタート画面
VOID MY_START_PROC(VOID);	//スタート画面の処理
VOID MY_START_DRAW(VOID);	//スタート画面の描画

VOID MY_PLAY_INIT(VOID);	//プレイ画面初期化
VOID MY_PLAY(VOID);			//プレイ画面
VOID MY_PLAY_PROC(VOID);	//プレイ画面の処理
VOID MY_PLAY_DRAW(VOID);	//プレイ画面の描画

VOID MY_MENU(VOID);
VOID MY_MENU_PROC(VOID);
VOID MY_MENU_DRAW(VOID);

VOID MY_BATTLE(VOID);
VOID MY_BATTLE_PROC(VOID);
VOID MY_BATTLE_DRAW(VOID);

VOID MY_END(VOID);			//エンド画面
VOID MY_END_PROC(VOID);		//エンド画面の処理
VOID MY_END_DRAW(VOID);		//エンド画面の描画

BOOL MY_LOAD_IMAGE(VOID);		//画像をまとめて読み込む関数
VOID MY_DELETE_IMAGE(VOID);		//画像をまとめて削除する関数

BOOL MY_LOAD_MUSIC(VOID);		//音楽をまとめて読み込む関数
VOID MY_DELETE_MUSIC(VOID);		//音楽をまとめて削除する関数
//CHARA player;

IMAGE ImageTitleBK;
IMAGE ImageTitleROGO;
IMAGE ImageTitlePUSH;

IMAGE ImageMapBK;

//MAPCHIP mapChip;

//MAP map[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

IMAGE ImageMenuBK;

IMAGE ImageBattleBK;
IMAGE ImageEnemy;

IMAGE ImageEndBK;
IMAGE ImageEndPush;

int DrawX = 0;	//表示位置X
int DrawY = 0;	//表示位置Y

//########## プログラムで最初に実行される関数 ##########
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txtを出力しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//指定の数値でウィンドウを表示する
	SetWindowStyleMode(GAME_WINDOW_BAR);				//タイトルバーはデフォルトにする
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));			//ウィンドウのタイトルの文字
	SetAlwaysRunFlag(TRUE);								//非アクティブでも実行する
	SetWindowIconID(IDI_ICON1);							//アイコンファイルを読込
	SetWindowUserCloseEnableFlag(FALSE);				//閉じるボタンで、勝手にウィンドウが閉じないようにする

	if (DxLib_Init() == -1) { return -1; }	//ＤＸライブラリ初期化処理

	//画像を読み込む
	if (MY_LOAD_IMAGE() == FALSE) { return -1; }

	//音楽を読み込む
	if (MY_LOAD_MUSIC() == FALSE) { return -1; }

	GameScene = GAME_SCENE_START;	//ゲームシーンはスタート画面から
	SetDrawScreen(DX_SCREEN_BACK);	//Draw系関数は裏画面に描画

	//無限ループ
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージ処理の結果がエラーのとき、強制終了
		if (ClearDrawScreen() != 0) { break; }	//画面を消去できなかったとき、強制終了

		//画面の閉じるボタンを押されたとき
		if (GetWindowUserCloseFlag(TRUE))
		{
			//終了ダイアログを表示
			int Ret = MessageBox(GetMainWindowHandle(), MSG_CLOSE_CAPTION, MSG_CLOSE_TITLE, MB_YESNO | MB_ICONASTERISK);
			if (Ret == IDYES) { break; }	//YESなら、ゲームを中断する
		}

		MY_ALL_KEYDOWN_UPDATE();				//押しているキー状態を取得

		MY_MOUSE_UPDATE();						//マウスの状態を取得

		MY_FPS_UPDATE();	//FPSの処理[更新]

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_START:
			MY_START();	//スタート画面
			break;
		case GAME_SCENE_PLAY:
			MY_PLAY();	//プレイ画面
			
			int image[76];

			LoadDivGraph("IMAGE.\\村娘.png", 48, 8, 12, 48, 48, image);

			DrawGraph(DrawX, DrawY, image[0], TRUE);      // 画像を表示

			
			if (MY_KEY_DOWN(KEY_INPUT_UP) == TRUE) {
				DrawY--; DrawGraph(DrawX, DrawY, image[0], TRUE);
			}
			if (MY_KEY_DOWN(KEY_INPUT_DOWN) == TRUE) {
				DrawY++; DrawGraph(DrawX, DrawY, image[50], TRUE);
			}
			if (MY_KEY_DOWN(KEY_INPUT_LEFT) == TRUE) {
				DrawX--; DrawGraph(DrawX, DrawY, image[13], TRUE);
			}
			if (MY_KEY_DOWN(KEY_INPUT_RIGHT) == TRUE) {
				DrawX++; DrawGraph(DrawX, DrawY, image[13], TRUE);
			}

			break;
		case GAME_SCENE_MENU:
			MY_MENU();  //メニュー画面
			break;
		case GAME_SCENE_BATTLE:
			MY_BATTLE();
			break;
		case GAME_SCENE_END:
			MY_END();	//エンド画面
			break;
		}

		MY_FPS_DRAW();		//FPSの処理[描画]

		ScreenFlip();		//モニタのリフレッシュレートの速さで裏画面を再描画

		MY_FPS_WAIT();		//FPSの処理[待つ]

		

	}

	//画像ハンドルを破棄
	MY_DELETE_IMAGE();

	//音楽ハンドルを破棄
	MY_DELETE_MUSIC();

	DxLib_End();	//ＤＸライブラリ使用の終了処理

	return 0;
}

//########## FPS値を計測、更新する関数 ##########
VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1フレーム目なら時刻を記憶
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps) //60フレーム目なら平均を計算
	{
		int now = GetNowCount();
		//現在の時間から、0フレーム目の時間を引き、FPSの数値で割る＝1FPS辺りの平均時間が計算される
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

//########## FPS値を描画する関数 ##########
VOID MY_FPS_DRAW(VOID)
{
	//文字列を描画
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

//########## FPS値を計測し、待つ関数 ##########
VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//かかった時間
	int waitTime = CountFps * 1000 / GAME_FPS - resultTime;	//待つべき時間

	if (waitTime > 0)		//指定のFPS値よりも早い場合
	{
		WaitTimer(waitTime);	//待つ
	}
	return;
}

//########## キーの入力状態を更新する関数 ##########
VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{

	//一時的に、現在のキーの入力状態を格納する
	char TempKey[KEY_CODE_KIND];

	//直前のキー入力をとっておく
	for (int i = 0; i < KEY_CODE_KIND; i++)
	{
		OldAllKeyState[i] = AllKeyState[i];
	}

	GetHitKeyStateAll(TempKey); // 全てのキーの入力状態を得る

	for (int i = 0; i < KEY_CODE_KIND; i++)
	{
		if (TempKey[i] != 0)	//押されているキーのキーコードを押しているとき
		{
			AllKeyState[i]++;	//押されている
		}
		else
		{
			AllKeyState[i] = 0;	//押されていない
		}
	}
	return;
}

//キーを押しているか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	//キーコードのキーを押している時
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;	//キーを押している
	}
	else
	{
		return FALSE;	//キーを押していない
	}
}

//キーを押し上げたか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1	//直前は押していて
		&& AllKeyState[KEY_INPUT_] == 0)	//今は押していないとき
	{
		return TRUE;	//キーを押し上げている
	}
	else
	{
		return FALSE;	//キーを押し上げていない
	}
}

//キーを押し続けているか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
//引　数：int：キーを押し続ける時間
BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	//押し続ける時間=秒数×FPS値
	//例）60FPSのゲームで、1秒間押し続けるなら、1秒×60FPS
	int UpdateTime = DownTime * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}
	else
	{
		return FALSE;	//押し続けていない
	}
}

//マウスの入力情報を更新する
VOID MY_MOUSE_UPDATE(VOID)
{
	//マウスの以前の座標を取得
	mouse.OldPoint = mouse.Point;

	//マウスの以前のボタン入力を取得
	for (int i = 0; i < MOUSE_BUTTON_CODE; i++) { mouse.OldButton[i] = mouse.Button[i]; }

	//マウスの座標を取得
	GetMousePoint(&mouse.Point.x, &mouse.Point.y);

	//マウスの押しているボタンを取得
	mouse.InputValue = GetMouseInput();

	//左ボタンを押しているかチェック(TRUEは0以外なので、2も4もTRUE)
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]++; }		//押している
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) != MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT] = 0; }		//押していない

	//中ボタンを押しているかチェック(TRUEは0以外なので、2も4もTRUE)
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) == MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE]++; }	//押している
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) != MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE] = 0; }	//押していない

	//右ボタンを押しているかチェック(TRUEは0以外なので、2も4もTRUE)
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) == MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT]++; }		//押している
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) != MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT] = 0; }	//押していない

	//ホイールの回転量を取得
	mouse.WheelValue = GetMouseWheelRotVol();

	return;
}

//ボタンを押しているか、マウスーコードで判断する
//引　数：int：マウスコード：MOUSE_INPUT_???
BOOL MY_MOUSE_DOWN(int MOUSE_INPUT_)
{
	//マウスコードのボタンを押している時
	if (mouse.Button[MOUSE_INPUT_] != 0)
	{
		return TRUE;	//ボタンを押している
	}
	else
	{
		return FALSE;	//ボタンを押していない
	}
}

//ボタンを押し上げたか、マウスコード判断する
//引　数：int：マウスコード：MOUSE_INPUT_???
BOOL MY_MOUSE_UP(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >= 1	//直前は押していて
		&& mouse.Button[MOUSE_INPUT_] == 0)	//今は押していないとき
	{
		return TRUE;	//ボタンを押し上げている
	}
	else
	{
		return FALSE;	//ボタンを押し上げていない
	}
}

//ボタンを押し続けているか、マウスコード判断する
//引　数：int：マウスコード：MOUSE_INPUT_???
//引　数：int：ボタンを押し続ける時間
BOOL MY_MOUSEDOWN_KEEP(int MOUSE_INPUT_, int DownTime)
{
	//押し続ける時間=秒数×FPS値
	//例）60FPSのゲームで、1秒間押し続けるなら、1秒×60FPS
	int UpdateTime = DownTime * GAME_FPS;

	if (mouse.Button[MOUSE_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}
	else
	{
		return FALSE;	//押し続けていない
	}
}

//フォントをこのソフト用に、一時的にインストール


//スタート画面
VOID MY_START(VOID)
{
	MY_START_PROC();	//スタート画面の処理
	MY_START_DRAW();	//スタート画面の描画

	DrawString(0, 0, "スタート画面(エンターキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

//スタート画面の処理
VOID MY_START_PROC(VOID)
{
	//エンターキーを押したら、プレイシーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
	{
		MY_PLAY_INIT();	//ゲーム初期化

		//ゲームのシーンをプレイ画面にする
		GameScene = GAME_SCENE_PLAY;

		return;
	}

	return;
}

//スタート画面の描画
VOID MY_START_DRAW(VOID)
{
	DrawGraph(ImageTitleBK.x, ImageTitleBK.y, ImageTitleBK.handle, TRUE);

	DrawGraph(ImageTitleROGO.x, ImageTitleROGO.y, ImageTitleROGO.handle, TRUE);

	DrawGraph(ImageTitlePUSH.x, ImageTitlePUSH.y, ImageTitlePUSH.handle, TRUE);

	return;
}

//プレイ画面初期化
VOID MY_PLAY_INIT(VOID)
{

	return;
}

//プレイ画面
VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();	//プレイ画面の処理
	MY_PLAY_DRAW();	//プレイ画面の描画

	return;
}

//プレイ画面の処理
VOID MY_PLAY_PROC(VOID)
{
	//スペースキーを押したら、エンドシーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_SPACE) == TRUE)
	{
		//ゲームのシーンをエンド画面にする
		GameScene = GAME_SCENE_END;

		return;
	}

	if (MY_KEY_DOWN(KEY_INPUT_M) == TRUE)
	{
		//ゲームのシーンをメニュー画面にする
		GameScene = GAME_SCENE_MENU;

		return;
	}

	if (MY_KEY_DOWN(KEY_INPUT_B) == TRUE)
	{
		//ゲームのシーンを戦闘画面にする
		GameScene = GAME_SCENE_BATTLE;

		return;
	}

	if (MY_KEY_DOWN(KEY_INPUT_S) == TRUE)
	{
		//ゲームのシーンをスタート画面にする
		GameScene = GAME_SCENE_START;

		return;
	}
	
	
	

	return;
}

//プレイ画面の描画
VOID MY_PLAY_DRAW(VOID)
{
	

	DrawString(0, 0, "スタート画面:S,メニュー画面:M,戦闘画面:B,エンド画面:スペース", GetColor(0, 0, 0));

	

	DrawGraph(ImageMapBK.x, ImageMapBK.y, ImageMapBK.handle, TRUE);


	

	return;
}

VOID MY_MENU(VOID)
{
	MY_MENU_PROC();
	MY_MENU_DRAW();

	

	
	return;
}

VOID MY_MENU_PROC(VOID)
{
	if (MY_KEY_DOWN(KEY_INPUT_N) == TRUE)
	{
		GameScene = GAME_SCENE_PLAY;

		return;
	}

	return;
}

VOID MY_MENU_DRAW(VOID)
{
	DrawString(0, 0, "メニュー画面(Nキーを押して下さい)", GetColor(255, 255, 255));

	DrawString(GAME_WIDTH / 2, GAME_HEIGHT / 2,"メニュー",GetColor(255, 255, 255));
	//DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 255), TRUE);


	return;
}

VOID MY_BATTLE(VOID)
{
	MY_BATTLE_PROC();
	MY_BATTLE_DRAW();

	return;
}

VOID MY_BATTLE_PROC(VOID)
{
	if (MY_KEY_DOWN(KEY_INPUT_V) == TRUE)
	{
		GameScene = GAME_SCENE_PLAY;
	}



	return;
}

VOID MY_BATTLE_DRAW(VOID)
{
	DrawString(0, 0, "戦闘画面Vキーを押して下さい)", GetColor(255, 255, 255));

	DrawGraph(ImageBattleBK.x, ImageBattleBK.y, ImageBattleBK.handle, TRUE);

	DrawGraph(ImageEnemy.x, ImageEnemy.y, ImageEnemy.handle, TRUE);

	return;
}


//エンド画面
VOID MY_END(VOID)
{
	MY_END_PROC();	//エンド画面の処理
	MY_END_DRAW();	//エンド画面の描画


	return;
}

//エンド画面の処理
VOID MY_END_PROC(VOID)
{
	//エスケープキーを押したら、スタートシーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
	{
		GameScene = GAME_SCENE_START;
	}

	DrawString(0, 0, "エンド画面(エスケープキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

//エンド画面の描画
VOID MY_END_DRAW(VOID)
{
	DrawGraph(ImageEndBK.x, ImageEndBK.y, ImageEndBK.handle, TRUE);
	DrawGraph(ImageEndPush.x, ImageEndPush.y, ImageEndPush.handle, TRUE);

	return;
}

//画像をまとめて読み込む関数
BOOL MY_LOAD_IMAGE(VOID)
{
	strcpy(ImageTitleBK.path, IMAGE_TITLE_BK_PATH);
	ImageTitleBK.handle = LoadGraph(ImageTitleBK.path);
	if (ImageTitleBK.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_BK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
	}

	GetGraphSize(ImageTitleBK.handle, &ImageTitleBK.width, &ImageTitleBK.height);
	ImageTitleBK.x = GAME_WIDTH / 2 - ImageTitleBK.width / 2;
	ImageTitleBK.y = GAME_HEIGHT / 2 - ImageTitleBK.height / 2;

	strcpy(ImageTitleROGO.path, IMAGE_TITLE_ROGO_PATH);
	ImageTitleROGO.handle = LoadGraph(ImageTitleROGO.path);
	if (ImageTitleROGO.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
	}

	GetGraphSize(ImageTitleROGO.handle, &ImageTitleROGO.width, &ImageTitleROGO.height);
	ImageTitleROGO.x = GAME_WIDTH / 2 - ImageTitleROGO.width / 2;
	ImageTitleROGO.y = GAME_HEIGHT / 2 - ImageTitleROGO.height / 2;

	strcpy(ImageMapBK.path, IMAGE_PLAY_MAP_PATH);
	ImageMapBK.handle = LoadGraph(ImageMapBK.path);
	if (ImageMapBK.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_PLAY_MAP_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
	}

	GetGraphSize(ImageMapBK.handle, &ImageMapBK.width, &ImageMapBK.height);
	ImageMapBK.x = GAME_WIDTH / 2 - ImageMapBK.width / 2;
	ImageMapBK.y = GAME_HEIGHT / 3 - ImageMapBK.height / 3;
	
	strcpy(ImageBattleBK.path, IMAGE_BATTLE_BK_PATH);
	ImageBattleBK.handle = LoadGraph(ImageBattleBK.path);
	if (ImageBattleBK.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_BATTLE_BK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
	}

	GetGraphSize(ImageEnemy.handle, &ImageEnemy.width, &ImageEnemy.height);
	ImageEnemy.x = GAME_WIDTH / 2 - ImageEnemy.width / 2;
	ImageEnemy.y = GAME_HEIGHT / 2 - ImageEnemy.height / 2;

	strcpy(ImageEnemy.path, IMAGE_ENEMY_PATH);
	ImageEnemy.handle = LoadGraph(ImageEnemy.path);
	if (ImageEnemy.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_ENEMY_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
	}

	GetGraphSize(ImageEnemy.handle, &ImageEnemy.width, &ImageEnemy.height);
	ImageEnemy.x = GAME_WIDTH / 2 - ImageEnemy.width / 2;
	ImageEnemy.y = GAME_HEIGHT / 2 - ImageEnemy.height / 2;
	

	strcpy(ImageEndBK.path, IMAGE_END_BK_PATH);
	ImageEndBK.handle = LoadGraph(ImageEndBK.path);
	if (ImageEndBK.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_END_BK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
	}

	GetGraphSize(ImageEndBK.handle, &ImageEndBK.width, &ImageEndBK.height);
	ImageEndBK.x = GAME_WIDTH / 2 - ImageEndBK.width / 2;
	ImageEndBK.y = GAME_HEIGHT / 2 - ImageEndBK.height / 2;

	strcpy(ImageEndPush.path, IMAGE_END_PUSH_PATH);
	ImageEndPush.handle = LoadGraph(ImageEndPush.path);
	if (ImageEndPush.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_END_PUSH_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
	}

	GetGraphSize(ImageEndPush.handle, &ImageEndPush.width, &ImageEndPush.height);
	ImageEndPush.x = GAME_WIDTH / 2 - ImageEndPush.width / 2;
	ImageEndPush.y = GAME_HEIGHT / 2 - ImageEndPush.height / 2;


	return TRUE;
}

//画像をまとめて削除する関数
VOID MY_DELETE_IMAGE(VOID)
{

	return;
}

//音楽をまとめて読み込む関数
BOOL MY_LOAD_MUSIC(VOID)
{

	return TRUE;
}

//音楽をまとめて削除する関数
VOID MY_DELETE_MUSIC(VOID)
{

	return;
}

